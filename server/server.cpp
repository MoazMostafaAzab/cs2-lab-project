#include <QCoreApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QMap>

#include "MessageRouter.h"

class SocketOutput : public IClientOutput {
public:
    explicit SocketOutput(QTcpSocket* socket) : m_socket(socket) {}

    void send(const QByteArray& data) override
    {
        if (m_socket && m_socket->isOpen())
            m_socket->write(data);
    }

private:
    QTcpSocket* m_socket;
};

class ChatServer : public QTcpServer {
    Q_OBJECT

public:
    explicit ChatServer(QObject* parent = nullptr) : QTcpServer(parent) {}

protected:
    void incomingConnection(qintptr socketDescriptor) override
    {
        QTcpSocket* client = new QTcpSocket(this);
        client->setSocketDescriptor(socketDescriptor);

        const int connId = static_cast<int>(socketDescriptor);
        auto* output = new SocketOutput(client);
        m_outputs[connId] = output;
        m_router.addConnection(connId, output);

        qDebug() << "[Server] New connection id=" << connId
                 << "from" << client->peerAddress().toString();

        connect(client, &QTcpSocket::readyRead, this, [this, client, connId]() {
            onReadyRead(client, connId);
        });

        connect(client, &QTcpSocket::disconnected, this, [this, client, connId]() {
            QString user = m_router.usernameForConnection(connId);
            qDebug() << "[Server] Disconnected id=" << connId
                     << (user.isEmpty() ? "" : ("user=" + user));
            m_router.removeConnection(connId);
            delete m_outputs.take(connId);
            client->deleteLater();
        });
    }

private slots:
    void onReadyRead(QTcpSocket* client, int connId)
    {
        while (client->canReadLine()) {
            QByteArray line = client->readLine().trimmed();
            QJsonDocument doc = QJsonDocument::fromJson(line);

            if (doc.isNull() || !doc.isObject()) {
                qWarning() << "[Server] Invalid JSON from conn" << connId;
                continue;
            }

            QString error = m_router.routeMessage(connId, doc.object());
            if (!error.isEmpty()) {
                qWarning() << "[Server] Routing error from conn" << connId << ":" << error;
                QJsonObject errMsg;
                errMsg["type"]    = "error";
                errMsg["payload"] = QJsonObject{{ "message", error }};
                client->write(QJsonDocument(errMsg).toJson(QJsonDocument::Compact) + "\n");
            }
        }
    }

private:
    MessageRouter            m_router;
    QMap<int, SocketOutput*> m_outputs;
};

#include "server.moc"

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    ChatServer server;
    const quint16 PORT = 12345;

    if (!server.listen(QHostAddress::Any, PORT)) {
        qCritical() << "[Server] Failed to listen on port" << PORT
                    << ":" << server.errorString();
        return 1;
    }

    qDebug() << "[Server] Listening on port" << PORT;
    return app.exec();
}
