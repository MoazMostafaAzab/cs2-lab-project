#include "chatmanager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

ChatManager::ChatManager(INetworkClient* network, QObject* parent)
    : QObject(parent), m_network(network)
{
}

bool ChatManager::sendMessage(const QString& toUser, const QString& messageText)
{
    if (!validateMessage(messageText)) {
        emit errorOccurred(m_lastError);
        return false;
    }

    if (m_network) {

        m_network->sendPrivateMessage(toUser, messageText);
    }

    emit messageSent(toUser, messageText);
    return true;
}

void ChatManager::handleIncomingData(const QByteArray& rawData)
{

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(rawData, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        emit errorOccurred("Invalid JSON: " + parseError.errorString());
        return;
    }

    QJsonObject obj = doc.object();

    QString sender = obj.value("sender").toString();
    QString payload = obj.value("payload").toString();

    if (sender.isEmpty() || payload.isEmpty()) {
        emit errorOccurred("Malformed message");
        return;
    }

    emit messageReceived(sender, payload);
}

QString ChatManager::lastError() const
{
    return m_lastError;
}

bool ChatManager::validateMessage(const QString& text)
{
    if (text.trimmed().isEmpty()) {
        m_lastError = "Message cannot be empty.";
        return false;
    }

    if (text.length() > 500) {
        m_lastError = "Message too long (max 500 characters).";
        return false;
    }

    return true;
}
