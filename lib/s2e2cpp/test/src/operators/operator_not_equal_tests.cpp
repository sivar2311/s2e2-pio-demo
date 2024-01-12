#include "../test_utils.hpp"

#include <s2e2/error.hpp>
#include <s2e2/operators/operator_not_equal.hpp>

#include <gtest/gtest.h>

#include <typeinfo>


TEST(OperatorNotEqualTests, positiveTest_GoodArguments_StackSize)
{
	s2e2::OperatorNotEqual op;
    auto stack = TestUtils::createStack(std::string{"String1"}, std::string{"String2"});

    op.invoke(stack);

    ASSERT_EQ(1, stack.size());
}

TEST(OperatorNotEqualTests, positiveTest_GoodArguments_ResultType)
{
	s2e2::OperatorNotEqual op;
    auto stack = TestUtils::createStack(std::string{"String1"}, std::string{"String2"});

    op.invoke(stack);

    ASSERT_TRUE(stack.top().has_value());
    ASSERT_EQ(typeid(bool), stack.top().type());
}

TEST(OperatorNotEqualTests, positiveTest_EqualStrings_ResultValue)
{
	s2e2::OperatorNotEqual op;
    auto stack = TestUtils::createStack(std::string{"String1"}, std::string{"String1"});

    op.invoke(stack);

    ASSERT_FALSE(std::any_cast<bool>(stack.top()));
}

TEST(OperatorNotEqualTests, positiveTest_DifferentStrings_ResultValue)
{
	s2e2::OperatorNotEqual op;
    auto stack = TestUtils::createStack(std::string{"String1"}, std::string{"String2"});

    op.invoke(stack);

    ASSERT_TRUE(std::any_cast<bool>(stack.top()));
}

TEST(OperatorNotEqualTests, positiveTest_FirstArgumentNull_ResultValue)
{
	s2e2::OperatorNotEqual op;
    auto stack = TestUtils::createStack(std::any{}, std::string{"String2"});

    op.invoke(stack);

    ASSERT_TRUE(std::any_cast<bool>(stack.top()));
}

TEST(OperatorNotEqualTests, positiveTest_SecondArgumentNull_ResultValue)
{
	s2e2::OperatorNotEqual op;
    auto stack = TestUtils::createStack(std::string{"String1"}, std::any{});

    op.invoke(stack);

    ASSERT_TRUE(std::any_cast<bool>(stack.top()));
}

TEST(OperatorNotEqualTests, positiveTest_BothArgumentsNull_ResultValue)
{
	s2e2::OperatorNotEqual op;
    auto stack = TestUtils::createStack(std::any{}, std::any{});

    op.invoke(stack);

    ASSERT_FALSE(std::any_cast<bool>(stack.top()));
}

TEST(OperatorNotEqualTests, positiveTest_MoreArguments_StackSize)
{
	s2e2::OperatorNotEqual op;
    auto stack = TestUtils::createStack(std::string{"String1"}, std::string{"String2"}, std::string{"String3"});

    op.invoke(stack);

    ASSERT_EQ(2, stack.size());
}

TEST(OperatorNotEqualTests, negativeTest_FewerArguments)
{
	s2e2::OperatorNotEqual op;
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

TEST(OperatorNotEqualTests, negativeTest_FirstArgumentWrongType)
{
	s2e2::OperatorNotEqual op;
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

TEST(OperatorNotEqualTests, negativeTest_SecondArgumentWrongType)
{
	s2e2::OperatorNotEqual op;
    auto stack = TestUtils::createStack(std::string{"1"}, 1);

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

TEST(OperatorNotEqualTests, negativeTest_BothArgumentsWrongType)
{
	s2e2::OperatorNotEqual op;
    auto stack = TestUtils::createStack(1, 1);

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
