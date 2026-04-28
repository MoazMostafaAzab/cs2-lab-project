#include <QCoreApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QList>
#include <QDebug>

class ChatServer : public QTcpServer {
    Q_OBJECT

public:
    explicit ChatServer(QObject* parent = nullptr) : QTcpServer(parent) {}

protected:
    void incomingConnection(qintptr socketDescriptor) override
    {
        QTcpSocket* client = new QTcpSocket(this);
        client->setSocketDescriptor(socketDescriptor);

        qDebug() << "[Server] New client connected from"
                 << client->peerAddress().toString();

        connect(client, &QTcpSocket::readyRead, this, [this, client]() {
            onReadyRead(client);
        });

        connect(client, &QTcpSocket::disconnected, this, [client]() {
            qDebug() << "[Server] Client disconnected.";
            client->deleteLater();
        });

        m_clients.append(client);
    }

private slots:
    void onReadyRead(QTcpSocket* client)
    {
        while (client->canReadLine()) {
            QByteArray line = client->readLine().trimmed();
            QJsonDocument doc = QJsonDocument::fromJson(line);

            if (doc.isNull()) {
                qWarning() << "[Server] Invalid JSON received:" << line;
                continue;
            }

            QJsonObject msg = doc.object();
            QString type    = msg["type"].toString();
            QString sender  = msg["sender"].toString();
            QJsonObject pl  = msg["payload"].toObject();

            qDebug() << "──────────────────────────────";
            qDebug() << "Type   :" << type;
            qDebug() << "Sender :" << sender;
            qDebug() << "Payload:" << QJsonDocument(pl).toJson(QJsonDocument::Compact);
            qDebug() << "──────────────────────────────";
        }
    }

private:
    QList<QTcpSocket*> m_clients;
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
