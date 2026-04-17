#include "chatmanager.h"

ChatManager::ChatManager(INetworkClient* network, QObject* parent)
    : QObject(parent), m_network(network)
{
}

bool ChatManager::sendMessage(const QString& senderUsername, const QString& messageText)
{
    if (!validateMessage(messageText)) {
        emit errorOccurred(m_lastError);
        return false;
    }

    QByteArray jsonData = formatMessageAsJson(senderUsername, messageText);

    if (m_network) {
        m_network->sendData(jsonData);
    }

    emit messageSent(senderUsername, messageText);
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

QByteArray ChatManager::formatMessageAsJson(const QString& sender, const QString& text)
{
    QJsonObject obj;
    obj["type"] = "chat_message";
    obj["sender"] = sender;
    obj["payload"] = text;

    return QJsonDocument(obj).toJson(QJsonDocument::Compact);
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
