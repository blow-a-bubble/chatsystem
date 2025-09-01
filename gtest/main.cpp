#include <gtest/gtest.h>
#include <iostream>
using namespace std;
int add(int a, int b)
{
    return a + b;
}
TEST(测试, 使用GTEST测试1)
{
    ASSERT_EQ(add(1, 2), 3);
    // ASSERT_EQ(add(1, 2), 4);
    // EXPECT_EQ(add(4, 2), 4);

}
TEST(测试, 使用GTEST测试2)
{
    ASSERT_EQ(add(1, 2), 3);
    // EXPECT_EQ(add(4, 2), 4);
    // ASSERT_EQ(add(1, 2), 4);
}
int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}