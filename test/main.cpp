#include "gtest/gtest.h"

TEST(example, add)
{
    ASSERT_EQ(2, 1);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}