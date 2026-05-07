#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "INetworkClient.h"
#include "loginmanager.h"
#include "chatmanager.h"


TEST(LoginManagerTest, emptyUsernameTest) {
    MockNetworkClient mockNetwork;
    LoginManager manager(&mockNetwork);

    bool result = manager.login("");
    EXPECT_FALSE(result);
}

TEST(LoginManagerTest, validUsername) {
    MockNetworkClient mockNetwork;
    LoginManager manager(&mockNetwork);

    EXPECT_CALL(mockNetwork, connectToServer(QString("Mazen")))
        .Times(1);

    bool result = manager.login("Mazen");
    EXPECT_TRUE(result);
}

TEST(LoginManagerTest, logout) {
    MockNetworkClient mockNetwork;
    LoginManager manager(&mockNetwork);

    EXPECT_CALL(mockNetwork, connectToServer(QString("Mazen")))
        .Times(1);
    EXPECT_CALL(mockNetwork, disconnect()).Times(1);

    manager.login("Mazen");
    manager.logout();

    EXPECT_EQ(manager.getCurrentUser(), "");
}

TEST(ChatManagerTest, emptyMessage) {
    MockNetworkClient mockNetwork;
    ChatManager chat(&mockNetwork);

    bool result = chat.sendMessage("Mazen", "");
    EXPECT_FALSE(result);
}

TEST(ChatManagerTest, sendMessage) {
    MockNetworkClient mockNetwork;
    ChatManager chat(&mockNetwork);

    EXPECT_CALL(mockNetwork, sendPrivateMessage(QString("Mazen"), QString("Hello")))
        .Times(1);

    bool result = chat.sendMessage("Mazen", "Hello");
    EXPECT_TRUE(result);
}

TEST(ChatManagerTest, messageTooLongReturnsFalse) {
    MockNetworkClient mockNetwork;
    ChatManager chat(&mockNetwork);

    QString longMessage(501, 'a');
    bool result = chat.sendMessage("Mazen", longMessage);
    EXPECT_FALSE(result);
} // we could remove this later if we decide to, but for now the message length limit will be 500