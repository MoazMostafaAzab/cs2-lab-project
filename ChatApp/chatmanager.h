#ifndef CHATMANAGER_H
#define CHATMANAGER_H

#include <QObject>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include "INetworkClient.h"

class ChatManager : public QObject {
    Q_OBJECT

public:
    explicit ChatManager(INetworkClient* network, QObject* parent = nullptr);

    bool sendMessage(const QString& senderUsername, const QString& messageText);
    void handleIncomingData(const QByteArray& rawData);
    QString lastError() const;

signals:
    void messageSent(const QString& sender, const QString& text);
    void messageReceived(const QString& sender, const QString& text);
    void errorOccurred(const QString& errorMessage);

private:
    INetworkClient* m_network;
    QString m_lastError;

    QByteArray formatMessageAsJson(const QString& sender, const QString& text);
    bool validateMessage(const QString& text);
};

#endif // CHATMANAGER_H
