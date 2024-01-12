#include "../test_utils.hpp"

#include <s2e2/error.hpp>
#include <s2e2/operators/operator_or.hpp>

#include <gtest/gtest.h>

#include <typeinfo>


TEST(OperatorOrTests, positiveTest_GoorArguments_StackSize)
{
	s2e2::OperatorOr op;
    auto stack = TestUtils::createStack(true, true);

    op.invoke(stack);

    ASSERT_EQ(1, stack.size());
}

TEST(OperatorOrTests, positiveTest_GoorArguments_ResultType)
{
	s2e2::OperatorOr op;
    auto stack = TestUtils::createStack(true, true);

    op.invoke(stack);

    ASSERT_TRUE(stack.top().has_value());
    ASSERT_EQ(typeid(bool), stack.top().type());
}

TEST(OperatorOrTests, positiveTest_TrueTrue_ResultValue)
{
	s2e2::OperatorOr op;
    auto stack = TestUtils::createStack(true, true);

    op.invoke(stack);

    ASSERT_TRUE(std::any_cast<bool>(stack.top()));
}

TEST(OperatorOrTests, positiveTest_TrueFalse_ResultValue)
{
	s2e2::OperatorOr op;
    auto stack = TestUtils::createStack(true, false);

    op.invoke(stack);

    ASSERT_TRUE(std::any_cast<bool>(stack.top()));
}

TEST(OperatorOrTests, positiveTest_FalseTrue_ResultValue)
{
	s2e2::OperatorOr op;
    auto stack = TestUtils::createStack(false, true);

    op.invoke(stack);

    ASSERT_TRUE(std::any_cast<bool>(stack.top()));
}

TEST(OperatorOrTests, positiveTest_FalseFalse_ResultValue)
{
	s2e2::OperatorOr op;
    auto stack = TestUtils::createStack(false, false);

    op.invoke(stack);

    ASSERT_FALSE(std::any_cast<bool>(stack.top()));
}

TEST(OperatorOrTests, positiveTest_MoreArguments_StackSize)
{
	s2e2::OperatorOr op;
    auto stack = TestUtils::createStack(std::string{"Arg"}, false, true);

    op.invoke(stack);

    ASSERT_EQ(2, stack.size());
}

TEST(OperatorOrTests, negativeTest_FewerArguments)
{
	s2e2::OperatorOr op;
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

TEST(OperatorOrTests, negativeTest_FirstArgumentWrongType)
{
	s2e2::OperatorOr op;
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

TEST(OperatorOrTests, negativeTest_FirstArgumentNull)
{
	s2e2::OperatorOr op;
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

TEST(OperatorOrTests, negativeTest_SecondArgumentWrongType)
{
	s2e2::OperatorOr op;
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

TEST(OperatorOrTests, negativeTest_SecondArgumentNull)
{
	s2e2::OperatorOr op;
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
