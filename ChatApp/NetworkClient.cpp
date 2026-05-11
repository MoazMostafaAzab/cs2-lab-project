#include "NetworkClient.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>

const QString NetworkClient::SERVER_HOST = "127.0.0.1";
const quint16 NetworkClient::SERVER_PORT = 12345;

NetworkClient::NetworkClient(QObject* parent)
    : QObject(parent),
    m_socket(new QTcpSocket(this))
{
    connect(m_socket, &QTcpSocket::connected,    this, &NetworkClient::onConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &NetworkClient::onDisconnected);
    connect(m_socket, &QTcpSocket::readyRead,    this, &NetworkClient::onReadyRead);
    connect(m_socket, &QAbstractSocket::errorOccurred,
            this, &NetworkClient::onErrorOccurred);
}

void NetworkClient::sendJson(const QString& type, const QJsonObject& payload)
{
    if (m_socket->state() != QAbstractSocket::ConnectedState) {
        qWarning() << "[NetworkClient] Not connected — cannot send type:" << type;
        return;
    }

    QJsonObject msg;
    msg["type"]    = type;
    msg["sender"]  = m_username;
    msg["payload"] = payload;

    QByteArray data = QJsonDocument(msg).toJson(QJsonDocument::Compact) + "\n";
    m_socket->write(data);
    m_socket->flush();

    qDebug() << "[NetworkClient] Sent:" << data.trimmed();
}

void NetworkClient::onConnected()
{
    qDebug() << "[NetworkClient] Connected to server.";
    emit connectedToServer();

    QJsonObject payload;
    payload["username"] = m_username;
    sendJson("login", payload);
}

void NetworkClient::onDisconnected()
{
    qDebug() << "[NetworkClient] Disconnected from server.";
    emit disconnectedFromServer();
}

void NetworkClient::onReadyRead()
{
    while (m_socket->canReadLine()) {
        QByteArray line = m_socket->readLine().trimmed();
        QJsonDocument doc = QJsonDocument::fromJson(line);
        if (doc.isNull()) {
            qWarning() << "[NetworkClient] Received invalid JSON:" << line;
            continue;
        }

        QJsonObject msg = doc.object();
        QString type = msg["type"].toString();

        qDebug() << "[NetworkClient] Received:" << line;

        if (type == "user_list") {
            QJsonArray arr = msg["payload"].toObject()["users"].toArray();
            QList<QString> users;
            const QJsonArray arrCopy = arr;
            for (const QJsonValue& v : arrCopy)
                users << v.toString();
            emit userListReceived(users);
        }
        else if (type == "private_message") {
            QString sender = msg["sender"].toString();
            QString text = msg["payload"].toObject()["text"].toString();
            emit privateMessageReceived(sender, text);
            emit dataReceived(line);
        }
        else if (type == "group_message") {
            QString sender    = msg["sender"].toString();
            QString groupName = msg["payload"].toObject()["group"].toString();
            QString text      = msg["payload"].toObject()["text"].toString();
            emit groupMessageReceived(groupName, sender, text);
        }
        else if (type == "added_to_group") {
            QString groupName = msg["payload"].toObject()["groupName"].toString();
            emit addedToGroup(groupName);
        }
        else if (type == "removed_from_group") {
            QString groupName = msg["payload"].toObject()["groupName"].toString();
            emit removedFromGroup(groupName);
        }
        else if (type == "group_created") {
            QString groupName = msg["payload"].toObject()["groupName"].toString();
            emit groupCreated(groupName);
        }

    }
}

void NetworkClient::onErrorOccurred(QAbstractSocket::SocketError error)
{
    qWarning() << "[NetworkClient] Socket error:" << error << m_socket->errorString();
}

void NetworkClient::connectToServer(const QString& username)
{
    m_username = username;
    qDebug() << "[NetworkClient] Connecting as" << username;
    m_socket->connectToHost(SERVER_HOST, SERVER_PORT);
}

void NetworkClient::disconnect()
{
    QJsonObject payload;
    payload["username"] = m_username;
    sendJson("logout", payload);
    m_socket->disconnectFromHost();
}

void NetworkClient::requestUserList()
{
    sendJson("request_user_list", QJsonObject());
}

void NetworkClient::sendPrivateMessage(const QString& toUser, const QString& message    )
{
    QJsonObject payload;
    payload["to"]      = toUser;
    payload["text"] = message;
    sendJson("private_message", payload);
}

void NetworkClient::sendBroadcastMessage(const QString& message)
{
    QJsonObject payload;
    payload["message"] = message;
    sendJson("broadcast", payload);
}

void NetworkClient::createGroup(const QString& groupName)
{
    QJsonObject payload;
    payload["groupName"] = groupName;
    sendJson("create_group", payload);
}

void NetworkClient::addUserToGroup(const QString& groupName, const QString& username)
{
    QJsonObject payload;
    payload["groupName"] = groupName;
    payload["username"]  = username;
    sendJson("add_to_group", payload);
}

void NetworkClient::removeUserFromGroup(const QString& groupName, const QString& username)
{
    QJsonObject payload;
    payload["groupName"] = groupName;
    payload["username"]  = username;
    sendJson("remove_from_group", payload);
}

void NetworkClient::sendGroupMessage(const QString& groupName, const QString& message)
{
    QJsonObject payload;
    payload["group"] = groupName;
    payload["text"]   = message;
    payload["members"] = QJsonArray();
    sendJson("group_message", payload);
}

void NetworkClient::sendData(const QByteArray& data)
{
    if (m_socket->state() != QAbstractSocket::ConnectedState) {
        qWarning() << "[NetworkClient] Not connected — cannot sendData";
        return;
    }
    m_socket->write(data);
    m_socket->flush();
}
