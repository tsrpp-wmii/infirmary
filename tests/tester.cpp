#include <gtest/gtest.h>
#include "server/server.h"

TEST(DummyTests, Dummytest)
{
    const auto multiplication = 28*10*2000;
    const auto expected = 560000;
    ASSERT_EQ(expected, multiplication);
}

// TODO: unit test testing connection with a database

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}