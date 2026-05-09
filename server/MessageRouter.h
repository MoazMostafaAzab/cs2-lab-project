#ifndef MESSAGEROUTER_H
#define MESSAGEROUTER_H

#include <QString>
#include <QJsonObject>
#include <QMap>
#include <QList>

class IClientOutput {
public:
    virtual ~IClientOutput() = default;
    virtual void send(const QByteArray& data) = 0;
};

class MessageRouter {
public:
    MessageRouter() = default;

    bool addConnection(int connectionId, IClientOutput* output);
    bool registerUser(int connectionId, const QString& username);
    void removeConnection(int connectionId);
    QString routeMessage(int fromConnectionId, const QJsonObject& msg);

    bool isUserConnected(const QString& username) const;
    int connectedUserCount() const;
    QString usernameForConnection(int connectionId) const;

private:
    struct ClientRecord {
        IClientOutput* output = nullptr;
        QString username;
    };

    QMap<int, ClientRecord> m_connections;
    QMap<QString, int> m_userIndex;
    QMap<QString, QList<QString>>    m_groups;

    void deliverTo(const QString& targetUser, const QJsonObject& envelope);
    QByteArray toLine(const QJsonObject& obj) const;
};

#endif
