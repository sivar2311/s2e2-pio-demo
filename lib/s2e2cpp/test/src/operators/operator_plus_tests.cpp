#include "../test_utils.hpp"

#include <s2e2/error.hpp>
#include <s2e2/operators/operator_plus.hpp>

#include <gtest/gtest.h>

#include <typeinfo>


TEST(OperatorPlusTests, positiveTest_StringString_StackSize)
{
	s2e2::OperatorPlus op;
    auto stack = TestUtils::createStack(std::string{"A"}, std::string{"B"});

    op.invoke(stack);

    ASSERT_EQ(1, stack.size());
}

TEST(OperatorPlusTests, positiveTest_StringString_ResultType)
{
	s2e2::OperatorPlus op;
    auto stack = TestUtils::createStack(std::string{"A"}, std::string{"B"});

    op.invoke(stack);

    ASSERT_TRUE(stack.top().has_value());
    ASSERT_EQ(typeid(std::string), stack.top().type());
}

TEST(OperatorPlusTests, positiveTest_StringString_ResultValue)
{
	s2e2::OperatorPlus op;
    auto stack = TestUtils::createStack(std::string{"A"}, std::string{"B"});

    op.invoke(stack);

    ASSERT_EQ(std::string{"AB"}, std::any_cast<std::string>(stack.top()));
}

TEST(OperatorPlusTests, positiveTest_StringNull_ResultValue)
{
	s2e2::OperatorPlus op;
    auto stack = TestUtils::createStack(std::string{"A"}, std::any{});

    op.invoke(stack);

    ASSERT_TRUE(stack.top().has_value());
    ASSERT_EQ(std::string{"A"}, std::any_cast<std::string>(stack.top()));
}

TEST(OperatorPlusTests, positiveTest_NullString_ResultValue)
{
	s2e2::OperatorPlus op;
    auto stack = TestUtils::createStack(std::any{}, std::string{"B"});

    op.invoke(stack);

    ASSERT_TRUE(stack.top().has_value());
    ASSERT_EQ(std::string{"B"}, std::any_cast<std::string>(stack.top()));
}

TEST(OperatorPlusTests, positiveTest_NullNull_ResultValue)
{
	s2e2::OperatorPlus op;
    auto stack = TestUtils::createStack(std::any{}, std::any{});

    op.invoke(stack);

    ASSERT_FALSE(stack.top().has_value());
}

TEST(OperatorPlusTests, positiveTest_MoreArguments_StackSize)
{
	s2e2::OperatorPlus op;
    auto stack = TestUtils::createStack(std::string{"A"}, std::string{"B"}, std::string{"C"});

    op.invoke(stack);

    ASSERT_EQ(2, stack.size());
}

TEST(OperatorPlusTests, negativeTest_FewerArguments)
{
	s2e2::OperatorPlus op;
    auto stack = TestUtils::createStack(std::string{"A"});

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

TEST(OperatorPlusTests, negativeTest_FirstArgumentWrongType)
{
	s2e2::OperatorPlus op;
    auto stack = TestUtils::createStack(5, std::string{"B"});

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

TEST(OperatorPlusTests, negativeTest_SecondArgumentWrongType)
{
	s2e2::OperatorPlus op;
    auto stack = TestUtils::createStack(std::string{"A"}, 1);

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
