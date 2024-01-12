#include "../test_utils.hpp"

#include <s2e2/error.hpp>
#include <s2e2/operators/operator_not.hpp>

#include <gtest/gtest.h>

#include <typeinfo>


TEST(OperatorNotTests, positiveTest_GoorArguments_StackSize)
{
	s2e2::OperatorNot op;
    auto stack = TestUtils::createStack(true);

    op.invoke(stack);

    ASSERT_EQ(1, stack.size());
}

TEST(OperatorNotTests, positiveTest_GoorArguments_ResultType)
{
	s2e2::OperatorNot op;
    auto stack = TestUtils::createStack(true);

    op.invoke(stack);

    ASSERT_TRUE(stack.top().has_value());
    ASSERT_EQ(typeid(bool), stack.top().type());
}

TEST(OperatorNotTests, positiveTest_ArgumentTrue_ResultValue)
{
	s2e2::OperatorNot op;
    auto stack = TestUtils::createStack(true);

    op.invoke(stack);

    ASSERT_FALSE(std::any_cast<bool>(stack.top()));
}

TEST(OperatorNotTests, positiveTest_ArgumentFalse_ResultValue)
{
	s2e2::OperatorNot op;
    auto stack = TestUtils::createStack(false);

    op.invoke(stack);

    ASSERT_TRUE(std::any_cast<bool>(stack.top()));
}

TEST(OperatorNotTests, positiveTest_MoreArguments_StackSize)
{
	s2e2::OperatorNot op;
    auto stack = TestUtils::createStack(false, true);

    op.invoke(stack);

    ASSERT_EQ(2, stack.size());
}

TEST(OperatorNotTests, negativeTest_FewerArguments)
{
	s2e2::OperatorNot op;
    auto stack = TestUtils::createStack();

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

TEST(OperatorNotTests, negativeTest_ArgumentWrongType)
{
	s2e2::OperatorNot op;
    auto stack = TestUtils::createStack(1);

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

TEST(OperatorNotTests, negativeTest_ArgumentNull)
{
	s2e2::OperatorNot op;
    auto stack = TestUtils::createStack(std::any{});

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
