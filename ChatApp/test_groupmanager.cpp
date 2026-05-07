#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "GroupManager.h"
#include "INetworkClient.h"

class MockNetworkClient : public INetworkClient {
public:
    MOCK_METHOD(void, connectToServer, (const QString& username), (override));
    MOCK_METHOD(void, disconnect, (), (override));
    MOCK_METHOD(void, requestUserList, (), (override));
    MOCK_METHOD(void, sendPrivateMessage, (const QString& toUser, const QString& message), (override));
    MOCK_METHOD(void, sendBroadcastMessage, (const QString& message), (override));
    MOCK_METHOD(void, createGroup, (const QString& groupName), (override));
    MOCK_METHOD(void, addUserToGroup, (const QString& groupName, const QString& username), (override));
    MOCK_METHOD(void, removeUserFromGroup, (const QString& groupName, const QString& username), (override));
    MOCK_METHOD(void, sendGroupMessage, (const QString& groupName, const QString& message), (override));
};


TEST(GroupManagerTest, CreateGroupEmptyName) {
    MockNetworkClient mockNetwork;
    GroupManager gm(&mockNetwork, "Abdurrahman");

    QString result = gm.createGroup("");

    EXPECT_EQ(result, "Error! Group Name can't be empty!");
}

TEST(GroupManagerTest, CreateGroupSuccessfully) {
    MockNetworkClient mockNetwork;
    GroupManager gm(&mockNetwork, "Abdurrahman");

    EXPECT_CALL(mockNetwork, createGroup("CS2")).Times(1);

    QString result = gm.createGroup("CS2");

    EXPECT_EQ(result, "Group created successfully!");
}

TEST(GroupManagerTest, CreateDuplicateGroup) {
    MockNetworkClient mockNetwork;
    GroupManager gm(&mockNetwork, "Abdurrahman");

    EXPECT_CALL(mockNetwork, createGroup("CS2")).Times(2);

    gm.createGroup("CS2");
    QString result = gm.createGroup("CS2");

    EXPECT_EQ(result, "Error! Group already exists!");
}

TEST(GroupManagerTest, AddUsertoNonexistentGroup) {
    MockNetworkClient mockNetwork;
    GroupManager gm(&mockNetwork, "Abdurrahman");


    QString result = gm.addUserToGroup("FakeGroup", "Karim");

    EXPECT_EQ(result, "Error! Group does not exist!");
}

TEST(GroupManagerTest, RemovingAdminFromGroup) {
    MockNetworkClient mockNetwork;
    GroupManager gm(&mockNetwork, "Abdurrahman");

    EXPECT_CALL(mockNetwork, createGroup("CS2")).Times(1);

    gm.createGroup("CS2");
    QString result = gm.removeUserFromGroup("CS2", "Abdurrahman");

    EXPECT_EQ(result, "Error! Admin can't be removed from the group");
}




