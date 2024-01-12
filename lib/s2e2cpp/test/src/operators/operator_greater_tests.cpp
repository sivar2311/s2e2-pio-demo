#include "../test_utils.hpp"

#include <s2e2/error.hpp>
#include <s2e2/operators/operator_greater.hpp>

#include <gtest/gtest.h>

#include <typeinfo>


TEST(OperatorGreaterTests, positiveTest_GoodArguments_StackSize)
{
	s2e2::OperatorGreater op;
    auto stack = TestUtils::createStack(std::string{"String1"}, std::string{"String2"});

    op.invoke(stack);

    ASSERT_EQ(1, stack.size());
}

TEST(OperatorGreaterTests, positiveTest_GoodArguments_ResultType)
{
	s2e2::OperatorGreater op;
    auto stack = TestUtils::createStack(std::string{"String1"}, std::string{"String2"});

    op.invoke(stack);

    ASSERT_TRUE(stack.top().has_value());
    ASSERT_EQ(typeid(bool), stack.top().type());
}

TEST(OperatorGreaterTests, positiveTest_EqualStrings_ResultValue)
{
	s2e2::OperatorGreater op;
    auto stack = TestUtils::createStack(std::string{"String1"}, std::string{"String1"});

    op.invoke(stack);

    ASSERT_FALSE(std::any_cast<bool>(stack.top()));
}

TEST(OperatorGreaterTests, positiveTest_FirstArgumentGreater_ResultValue)
{
	s2e2::OperatorGreater op;
    auto stack = TestUtils::createStack(std::string{"String2"}, std::string{"String1"});

    op.invoke(stack);

    ASSERT_TRUE(std::any_cast<bool>(stack.top()));
}

TEST(OperatorGreaterTests, positiveTest_SecondArgumentGreater_ResultValue)
{
	s2e2::OperatorGreater op;
    auto stack = TestUtils::createStack(std::string{"String1"}, std::string{"String2"});

    op.invoke(stack);

    ASSERT_FALSE(std::any_cast<bool>(stack.top()));
}

TEST(OperatorGreaterTests, positiveTest_MoreArguments_StackSize)
{
	s2e2::OperatorGreater op;
    auto stack = TestUtils::createStack(std::string{"String1"}, std::string{"String2"}, std::string{"String3"});

    op.invoke(stack);

    ASSERT_EQ(2, stack.size());
}

TEST(OperatorGreaterTests, negativeTest_FewerArguments)
{
	s2e2::OperatorGreater op;
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

TEST(OperatorGreaterTests, negativeTest_FirstArgumentWrongType)
{
	s2e2::OperatorGreater op;
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

TEST(OperatorGreaterTests, negativeTest_SecondArgumentWrongType)
{
	s2e2::OperatorGreater op;
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

TEST(OperatorGreaterTests, negativeTest_BothArgumentsWrongType)
{
	s2e2::OperatorGreater op;
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

TEST(OperatorGreaterTests, negativeTest_FirstArgumentNull)
{
	s2e2::OperatorGreater op;
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

TEST(OperatorGreaterTests, negativeTest_SecondArgumentNull)
{
	s2e2::OperatorGreater op;
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

TEST(OperatorGreaterTests, negativeTest_BothArgumentsNull)
{
	s2e2::OperatorGreater op;
    auto stack = TestUtils::createStack(std::any{}, std::any{});

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
