#include <gtest/gtest.h>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include "../MessageRouter.h"

class MockOutput : public IClientOutput {
public:
    void send(const QByteArray& data) override { received.append(data); }

    QJsonObject lastMessage() const {
        auto lines = received.split('\n');
        for (int i = lines.size() - 1; i >= 0; --i) {
            if (!lines[i].trimmed().isEmpty())
                return QJsonDocument::fromJson(lines[i]).object();
        }
        return QJsonObject();
    }

    bool hasReceived() const { return !received.isEmpty(); }
    void clear() { received.clear(); }

    QByteArray received;
};

static QJsonObject makeLogin(const QString& username) {
    QJsonObject msg;
    msg["type"]    = "login";
    msg["sender"]  = "";
    msg["payload"] = QJsonObject{{ "username", username }};
    return msg;
}

static QJsonObject makePrivate(const QString& sender, const QString& to, const QString& text) {
    QJsonObject msg;
    msg["type"]   = "private_message";
    msg["sender"] = sender;
    msg["payload"] = QJsonObject{{ "to", to }, { "text", text }};
    return msg;
}

TEST(MessageRouterTest, UserRegistrationStoresUsername)
{
    MessageRouter router;
    MockOutput output;

    router.addConnection(1, &output);
    QString err = router.routeMessage(1, makeLogin("Alice"));

    EXPECT_TRUE(err.isEmpty()) << err.toStdString();
    EXPECT_TRUE(router.isUserConnected("Alice"));
    EXPECT_EQ(router.usernameForConnection(1), "Alice");
    EXPECT_EQ(router.connectedUserCount(), 1);
}

TEST(MessageRouterTest, DuplicateUsernameRejected)
{
    MessageRouter router;
    MockOutput out1, out2;

    router.addConnection(1, &out1);
    router.addConnection(2, &out2);

    router.routeMessage(1, makeLogin("Alice"));
    QString err = router.routeMessage(2, makeLogin("Alice"));

    EXPECT_FALSE(err.isEmpty());
    EXPECT_EQ(router.connectedUserCount(), 1);
}

TEST(MessageRouterTest, PrivateMessageDeliveredToCorrectRecipient)
{
    MessageRouter router;
    MockOutput outAlice, outBob, outCarol;

    router.addConnection(1, &outAlice);
    router.addConnection(2, &outBob);
    router.addConnection(3, &outCarol);

    router.routeMessage(1, makeLogin("Alice"));
    router.routeMessage(2, makeLogin("Bob"));
    router.routeMessage(3, makeLogin("Carol"));

    outBob.clear();
    outCarol.clear();

    router.routeMessage(1, makePrivate("Alice", "Bob", "Hello Bob!"));

    EXPECT_TRUE(outBob.hasReceived());
    QJsonObject delivered = outBob.lastMessage();
    EXPECT_EQ(delivered["type"].toString(), "private_message");
    EXPECT_EQ(delivered["sender"].toString(), "Alice");
    EXPECT_EQ(delivered["payload"].toObject()["text"].toString(), "Hello Bob!");

    EXPECT_FALSE(outCarol.hasReceived());
}

TEST(MessageRouterTest, PrivateMessageToOfflineUserReturnsError)
{
    MessageRouter router;
    MockOutput outAlice;

    router.addConnection(1, &outAlice);
    router.routeMessage(1, makeLogin("Alice"));

    QString err = router.routeMessage(1, makePrivate("Alice", "Ghost", "are you there?"));

    EXPECT_FALSE(err.isEmpty());
    EXPECT_TRUE(router.isUserConnected("Alice"));
}

TEST(MessageRouterTest, DisconnectionCleansUpState)
{
    MessageRouter router;
    MockOutput out1, out2;

    router.addConnection(1, &out1);
    router.routeMessage(1, makeLogin("Alice"));
    EXPECT_TRUE(router.isUserConnected("Alice"));

    router.removeConnection(1);
    EXPECT_FALSE(router.isUserConnected("Alice"));
    EXPECT_EQ(router.connectedUserCount(), 0);

    router.addConnection(2, &out2);
    QString err = router.routeMessage(2, makeLogin("Alice"));
    EXPECT_TRUE(err.isEmpty()) << err.toStdString();
    EXPECT_TRUE(router.isUserConnected("Alice"));
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
