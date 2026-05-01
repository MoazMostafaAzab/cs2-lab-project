cmake_minimum_required(VERSION 3.16)
project(ServerTests)

set(CMAKE_CXX_STANDARD 17)

# Qt6 (or Qt5 fallback)
find_package(Qt6 COMPONENTS Core QUIET)
if(NOT Qt6_FOUND)
    find_package(Qt5 COMPONENTS Core REQUIRED)
    set(QT_TARGET Qt5::Core)
else()
    set(QT_TARGET Qt6::Core)
endif()

# GoogleTest
find_package(GTest REQUIRED)

add_executable(test_message_router
    tests/test_message_router.cpp
    MessageRouter.cpp
)

target_include_directories(test_message_router PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(test_message_router
    GTest::GTest
    GTest::Main
    ${QT_TARGET}
)

enable_testing()
add_test(NAME ServerRouterTests COMMAND test_message_router)
