#include "../test_utils.hpp"

#include <s2e2/error.hpp>
#include <s2e2/operators/operator_and.hpp>

#include <gtest/gtest.h>

#include <typeinfo>


TEST(OperatorAndTests, positiveTest_TrueTrue_StackSize)
{
	s2e2::OperatorAnd op;
    auto stack = TestUtils::createStack(true, true);

    op.invoke(stack);

    ASSERT_EQ(1, stack.size());
}

TEST(OperatorAndTests, positiveTest_TrueTrue_ResultType)
{
	s2e2::OperatorAnd op;
    auto stack = TestUtils::createStack(true, true);

    op.invoke(stack);

    ASSERT_TRUE(stack.top().has_value());
    ASSERT_EQ(typeid(bool), stack.top().type());
}

TEST(OperatorAndTests, positiveTest_TrueTrue_ResultValue)
{
	s2e2::OperatorAnd op;
    auto stack = TestUtils::createStack(true, true);

    op.invoke(stack);

    ASSERT_TRUE(std::any_cast<bool>(stack.top()));
}

TEST(OperatorAndTests, positiveTest_TrueFalse_ResultValue)
{
	s2e2::OperatorAnd op;
    auto stack = TestUtils::createStack(true, false);

    op.invoke(stack);

    ASSERT_FALSE(std::any_cast<bool>(stack.top()));
}

TEST(OperatorAndTests, positiveTest_FalseTrue_ResultValue)
{
	s2e2::OperatorAnd op;
    auto stack = TestUtils::createStack(false, true);

    op.invoke(stack);

    ASSERT_FALSE(std::any_cast<bool>(stack.top()));
}

TEST(OperatorAndTests, positiveTest_FalseFalse_ResultValue)
{
	s2e2::OperatorAnd op;
    auto stack = TestUtils::createStack(false, false);

    op.invoke(stack);

    ASSERT_FALSE(std::any_cast<bool>(stack.top()));
}

TEST(OperatorAndTests, positiveTest_MoreArguments_StackSize)
{
	s2e2::OperatorAnd op;
    auto stack = TestUtils::createStack(std::string{"Arg"}, false, true);

    op.invoke(stack);

    ASSERT_EQ(2, stack.size());
}

TEST(OperatorAndTests, negativeTest_FewerArguments)
{
	s2e2::OperatorAnd op;
    auto stack = TestUtils::createStack(true);

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

TEST(OperatorAndTests, negativeTest_FirstArgumentWrongType)
{
	s2e2::OperatorAnd op;
    auto stack = TestUtils::createStack("true", true);

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

TEST(OperatorAndTests, negativeTest_FirstArgumentNull)
{
	s2e2::OperatorAnd op;
    auto stack = TestUtils::createStack(std::any{}, true);

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

TEST(OperatorAndTests, negativeTest_SecondArgumentWrongType)
{
	s2e2::OperatorAnd op;
    auto stack = TestUtils::createStack(true, 1);

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

TEST(OperatorAndTests, negativeTest_SecondArgumentNull)
{
	s2e2::OperatorAnd op;
    auto stack = TestUtils::createStack(true, std::any{});

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
