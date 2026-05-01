#include "MessageRouter.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>

bool MessageRouter::addConnection(int connectionId, IClientOutput* output)
{
    if (m_connections.contains(connectionId)) return false;
    m_connections[connectionId] = { output, QString() };
    return true;
}

bool MessageRouter::registerUser(int connectionId, const QString& username)
{
    if (!m_connections.contains(connectionId)) return false;
    if (username.trimmed().isEmpty())           return false;
    if (m_userIndex.contains(username))         return false;

    m_connections[connectionId].username = username;
    m_userIndex[username] = connectionId;
    qDebug() << "[Router] Registered:" << username << "(conn" << connectionId << ")";
    return true;
}

void MessageRouter::removeConnection(int connectionId)
{
    if (!m_connections.contains(connectionId)) return;
    const QString username = m_connections[connectionId].username;
    if (!username.isEmpty()) m_userIndex.remove(username);
    m_connections.remove(connectionId);
    qDebug() << "[Router] Removed connection" << connectionId
             << (username.isEmpty() ? "" : ("(" + username + ")"));
}

QString MessageRouter::routeMessage(int fromConnectionId, const QJsonObject& msg)
{
    if (!m_connections.contains(fromConnectionId))
        return "Unknown connection";

    const QString type    = msg["type"].toString();
    const QString sender  = msg["sender"].toString();
    const QJsonObject payload = msg["payload"].toObject();

    if (type == "login") {
        const QString username = payload["username"].toString();
        if (!registerUser(fromConnectionId, username))
            return "Login failed: username empty or already taken";

        QJsonObject ack;
        ack["type"]    = "login_ack";
        ack["payload"] = QJsonObject{{ "status", "ok" }, { "username", username }};
        m_connections[fromConnectionId].output->send(toLine(ack));
        return QString();
    }

    if (m_connections[fromConnectionId].username.isEmpty())
        return "Not logged in";

    if (type == "private_message") {
        const QString toUser = payload["to"].toString();
        const QString text   = payload["text"].toString();

        if (toUser.isEmpty() || text.isEmpty())
            return "private_message: missing 'to' or 'text'";

        if (!m_userIndex.contains(toUser))
            return "User not online: " + toUser;

        QJsonObject envelope;
        envelope["type"]   = "private_message";
        envelope["sender"] = m_connections[fromConnectionId].username;
        envelope["payload"] = QJsonObject{{ "text", text }};
        deliverTo(toUser, envelope);
        return QString();
    }

    if (type == "broadcast") {
        const QString text = payload["text"].toString();
        if (text.isEmpty()) return "broadcast: missing 'text'";

        QJsonObject envelope;
        envelope["type"]    = "broadcast";
        envelope["sender"]  = m_connections[fromConnectionId].username;
        envelope["payload"] = QJsonObject{{ "text", text }};

        for (auto it = m_connections.begin(); it != m_connections.end(); ++it) {
            if (it.key() != fromConnectionId)
                it.value().output->send(toLine(envelope));
        }
        return QString();
    }

    if (type == "group_message") {
        const QString groupName = payload["group"].toString();
        const QString text      = payload["text"].toString();
        if (groupName.isEmpty() || text.isEmpty())
            return "group_message: missing 'group' or 'text'";

        const QJsonArray members = payload["members"].toArray();
        if (members.isEmpty()) return "group_message: no members provided";

        QJsonObject envelope;
        envelope["type"]    = "group_message";
        envelope["sender"]  = m_connections[fromConnectionId].username;
        envelope["payload"] = QJsonObject{{ "group", groupName }, { "text", text }};

        for (const QJsonValue& v : members) {
            const QString memberName = v.toString();
            if (memberName != m_connections[fromConnectionId].username)
                deliverTo(memberName, envelope);
        }
        return QString();
    }

    return "Unknown message type: " + type;
}

bool MessageRouter::isUserConnected(const QString& username) const
{
    return m_userIndex.contains(username);
}

int MessageRouter::connectedUserCount() const
{
    return m_userIndex.size();
}

QString MessageRouter::usernameForConnection(int connectionId) const
{
    if (!m_connections.contains(connectionId)) return QString();
    return m_connections[connectionId].username;
}

void MessageRouter::deliverTo(const QString& targetUser, const QJsonObject& envelope)
{
    if (!m_userIndex.contains(targetUser)) return;
    int targetConn = m_userIndex[targetUser];
    m_connections[targetConn].output->send(toLine(envelope));
}

QByteArray MessageRouter::toLine(const QJsonObject& obj) const
{
    return QJsonDocument(obj).toJson(QJsonDocument::Compact) + "\n";
}
