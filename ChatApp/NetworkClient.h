#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H
#include <QObject>
#include <QString>
#include <QTcpSocket>
#include "INetworkClient.h"

class NetworkClient : public QObject, public INetworkClient {
    Q_OBJECT
public:
    explicit NetworkClient(QObject* parent = nullptr);
    ~NetworkClient() = default;

    // INetworkClient interface
    void sendData(const QByteArray& data) override;

    // MAZEN
    void connectToServer(const QString& username)override;
    void disconnect()override;
    void requestUserList()override;

    // KARIM
    void sendPrivateMessage(const QString& toUser, const QString& message)override;
    void sendBroadcastMessage(const QString& message)override;

    // ABDURRAHMAN
    void createGroup(const QString& groupName)override;
    void addUserToGroup(const QString& groupName, const QString& username)override;
    void removeUserFromGroup(const QString& groupName, const QString& username)override;
    void sendGroupMessage(const QString& groupName, const QString& message)override;

signals:
    void userListReceived(QList<QString> userList);
    void connectedToServer();
    void disconnectedFromServer();
    void dataReceived(const QByteArray& rawData);

private slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onErrorOccurred(QAbstractSocket::SocketError error);

private:
    void sendJson(const QString& type, const QJsonObject& payload);
    QTcpSocket* m_socket;
    QString m_username;
    static const QString SERVER_HOST;
    static const quint16 SERVER_PORT;
};
#endif // NETWORKCLIENT_H
