#include "MessageRouter.h"
#include <QJsonDocument>
#include <QMap>
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

    const QString senderName = m_connections[fromConnectionId].username;
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

        QJsonArray allUsers;
        for (auto it = m_userIndex.begin(); it != m_userIndex.end(); ++it) {
            allUsers.append(it.key());
        }

        for (auto it = m_connections.begin(); it != m_connections.end(); ++it) {
            if (it.value().username.isEmpty()) continue;


            QJsonArray personalList;
            for (const QJsonValue& u : allUsers) {
                if (u.toString() != it.value().username)
                    personalList.append(u);


                QJsonObject listMsg;
                listMsg["type"] = "user_list";
            //    QJsonObject listPayload;
            //    listPayload["users"] = personalList;
                listMsg["payload"] = QJsonObject{{ "users", personalList }};
                it.value().output->send(toLine(listMsg));
            }
        }

        for (auto it = m_groups.begin(); it != m_groups.end(); ++it) {
            if (it.value().contains(username)) {
                QJsonObject notify;
                notify["type"] = "added_to_group";
                notify["payload"] = QJsonObject{{ "groupName", it.key() }};
                m_connections[fromConnectionId].output->send(toLine(notify));
            }
        }


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
        envelope["sender"] = senderName;
        envelope["payload"] = QJsonObject{{ "text", text }};
        deliverTo(toUser, envelope);
        return QString();
    }
    if (type == "create_group") {
        const QString groupName = payload["groupName"].toString();
        if (groupName.isEmpty())        return "create_group: missing groupName";
        if (m_groups.contains(groupName)) return "create_group: group already exists";

        m_groups[groupName] = { senderName };
        qDebug() << "[Router] Group created:" << groupName << "by" << senderName;


        QJsonObject ack;
        ack["type"]    = "group_created";
        ack["payload"] = QJsonObject{{ "groupName", groupName }};
        m_connections[fromConnectionId].output->send(toLine(ack));
        return QString();
    }

    if (type == "add_to_group") {
        const QString groupName = payload["groupName"].toString();
        const QString target    = payload["username"].toString();
        if (groupName.isEmpty() || target.isEmpty())
            return "add_to_group: missing fields";
        if (!m_groups.contains(groupName))
            return "add_to_group: group does not exist";
        if (m_groups[groupName].contains(target))
            return "add_to_group: user already in group";

        m_groups[groupName].append(target);


        if (m_userIndex.contains(target)) {
            QJsonObject notify;
            notify["type"]    = "added_to_group";
            notify["payload"] = QJsonObject{{ "groupName", groupName }};
            deliverTo(target, notify);
        }
        return QString();
    }

    if (type == "remove_from_group") {
        const QString groupName = payload["groupName"].toString();
        const QString target    = payload["username"].toString();
        if (groupName.isEmpty() || target.isEmpty())
            return "remove_from_group: missing fields";
        if (!m_groups.contains(groupName))
            return "remove_from_group: group does not exist";

        m_groups[groupName].removeAll(target);

        if (m_userIndex.contains(target)) {
            QJsonObject notify;
            notify["type"]    = "removed_from_group";
            notify["payload"] = QJsonObject{{ "groupName", groupName }};
            deliverTo(target, notify);
        }
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
        if (!m_groups.contains(groupName))
            return "group_message: group does not exist";


        QJsonObject envelope;
        envelope["type"]    = "group_message";
        envelope["sender"]  = senderName;
        envelope["payload"] = QJsonObject{{ "group", groupName }, { "text", text }};

        for (const QString& member : m_groups[groupName]) {
            if (member != senderName)
                deliverTo(member, envelope);
        }

        return QString();
    }

    if (type == "request_user_list") {
        QJsonArray users;
        for (auto it = m_userIndex.begin(); it != m_userIndex.end(); ++it) {
            if (it.key() != senderName)
                users.append(it.key());
        }
        QJsonObject response;
        response["type"] = "user_list";
     // QJsonObject payload;
    //  payload["users"] = users;
        response["payload"] = QJsonObject{{ "users", users }};;
        m_connections[fromConnectionId].output->send(toLine(response));
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
