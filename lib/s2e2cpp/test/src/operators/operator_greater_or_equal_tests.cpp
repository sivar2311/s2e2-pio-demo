#include "../test_utils.hpp"

#include <s2e2/error.hpp>
#include <s2e2/operators/operator_greater_or_equal.hpp>

#include <gtest/gtest.h>

#include <typeinfo>


TEST(OperatorGreaterOrEqualTests, positiveTest_GoodArguments_StackSize)
{
	s2e2::OperatorGreaterOrEqual op;
    auto stack = TestUtils::createStack(std::string{"String1"}, std::string{"String2"});

    op.invoke(stack);

    ASSERT_EQ(1, stack.size());
}

TEST(OperatorGreaterOrEqualTests, positiveTest_GoodArguments_ResultType)
{
	s2e2::OperatorGreaterOrEqual op;
    auto stack = TestUtils::createStack(std::string{"String1"}, std::string{"String2"});

    op.invoke(stack);

    ASSERT_TRUE(stack.top().has_value());
    ASSERT_EQ(typeid(bool), stack.top().type());
}

TEST(OperatorGreaterOrEqualTests, positiveTest_EqualStrings_ResultValue)
{
	s2e2::OperatorGreaterOrEqual op;
    auto stack = TestUtils::createStack(std::string{"String1"}, std::string{"String1"});

    op.invoke(stack);

    ASSERT_TRUE(std::any_cast<bool>(stack.top()));
}

TEST(OperatorGreaterOrEqualTests, positiveTest_FirstArgumentGreater_ResultValue)
{
	s2e2::OperatorGreaterOrEqual op;
    auto stack = TestUtils::createStack(std::string{"String2"}, std::string{"String1"});

    op.invoke(stack);

    ASSERT_TRUE(std::any_cast<bool>(stack.top()));
}

TEST(OperatorGreaterOrEqualTests, positiveTest_SecondArgumentGreater_ResultValue)
{
	s2e2::OperatorGreaterOrEqual op;
    auto stack = TestUtils::createStack(std::string{"String1"}, std::string{"String2"});

    op.invoke(stack);

    ASSERT_FALSE(std::any_cast<bool>(stack.top()));
}

TEST(OperatorGreaterOrEqualTests, positiveTest_BothArgumentsNull_ResultValue)
{
	s2e2::OperatorGreaterOrEqual op;
    auto stack = TestUtils::createStack(std::any{}, std::any{});

    op.invoke(stack);

    ASSERT_TRUE(std::any_cast<bool>(stack.top()));
}

TEST(OperatorGreaterOrEqualTests, positiveTest_MoreArguments_StackSize)
{
	s2e2::OperatorGreaterOrEqual op;
    auto stack = TestUtils::createStack(std::string{"String1"}, std::string{"String2"}, std::string{"String3"});

    op.invoke(stack);

    ASSERT_EQ(2, stack.size());
}

TEST(OperatorGreaterOrEqualTests, negativeTest_FewerArguments)
{
	s2e2::OperatorGreaterOrEqual op;
    auto stack = TestUtils::createStack(std::string{"String1"});

    ASSERT_THROW({
        try
        {
            op.invoke(stack);
        }
        catch (const s2e2::Error& e)
        {
            ASSERT_EQ("Not enough arguments for operator " + op.name, e.what());
            throw;
        }
    }, s2e2::Error);
}

TEST(OperatorGreaterOrEqualTests, negativeTest_FirstArgumentWrongType)
{
	s2e2::OperatorGreaterOrEqual op;
    auto stack = TestUtils::createStack(5, std::string{"5"});

    ASSERT_THROW({
        try
        {
            op.invoke(stack);
        }
        catch (const s2e2::Error& e)
        {
            ASSERT_EQ("Invalid arguments for operator " + op.name, e.what());
            throw;
        }
    }, s2e2::Error);
}

TEST(OperatorGreaterOrEqualTests, negativeTest_SecondArgumentWrongType)
{
	s2e2::OperatorGreaterOrEqual op;
    auto stack = TestUtils::createStack(std::string{"1"}, 2);

    ASSERT_THROW({
        try
        {
            op.invoke(stack);
        }
        catch (const s2e2::Error& e)
        {
            ASSERT_EQ("Invalid arguments for operator " + op.name, e.what());
            throw;
        }
    }, s2e2::Error);
}

TEST(OperatorGreaterOrEqualTests, negativeTest_BothArgumentsWrongType)
{
	s2e2::OperatorGreaterOrEqual op;
    auto stack = TestUtils::createStack(1, 2);

    ASSERT_THROW({
        try
        {
            op.invoke(stack);
        }
        catch (const s2e2::Error& e)
        {
            ASSERT_EQ("Invalid arguments for operator " + op.name, e.what());
            throw;
        }
    }, s2e2::Error);
}

TEST(OperatorGreaterOrEqualTests, negativeTest_FirstArgumentNull)
{
	s2e2::OperatorGreaterOrEqual op;
    auto stack = TestUtils::createStack(std::any{}, std::string{"String2"});

    ASSERT_THROW({
        try
        {
            op.invoke(stack);
        }
        catch (const s2e2::Error& e)
        {
            ASSERT_EQ("Invalid arguments for operator " + op.name, e.what());
            throw;
        }
    }, s2e2::Error);
}

TEST(OperatorGreaterOrEqualTests, negativeTest_SecondArgumentNull)
{
	s2e2::OperatorGreaterOrEqual op;
    auto stack = TestUtils::createStack(std::string{"String1"}, std::any{});

    ASSERT_THROW({
        try
        {
            op.invoke(stack);
        }
        catch (const s2e2::Error& e)
        {
            ASSERT_EQ("Invalid arguments for operator " + op.name, e.what());
            throw;
        }
    }, s2e2::Error);
}
