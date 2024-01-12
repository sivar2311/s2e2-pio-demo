#include "../test_utils.hpp"

#include <utils.hpp>

#include <s2e2/error.hpp>
#include <s2e2/functions/function_now.hpp>

#include <gtest/gtest.h>

#include <ctime>
#include <typeinfo>


TEST(FunctionNowTests, positiveTest_StackSize)
{
	s2e2::FunctionNow function;
    auto stack = TestUtils::createStack();

    function.invoke(stack);

    ASSERT_EQ(1, stack.size());
}

TEST(FunctionNowTests, positiveTest_ResultType)
{
	s2e2::FunctionNow function;
    auto stack = TestUtils::createStack();

    function.invoke(stack);

    ASSERT_TRUE(stack.top().has_value());
    ASSERT_EQ(typeid(std::tm), stack.top().type());
}

TEST(FunctionNowTests, positiveTest_ResultValue)
{
	s2e2::FunctionNow function;
    auto stack = TestUtils::createStack();

    function.invoke(stack);

    const auto now = std::time(nullptr);
    const double maxDifferenceInSeconds = 2.0;

    ASSERT_TRUE(stack.top().has_value());

    auto functionResult = std::any_cast<std::tm>(stack.top());
    ASSERT_EQ(0, functionResult.tm_isdst);

    const auto functionTs = s2e2::utcTs(&functionResult);
    const auto actualDifference = std::difftime(now, functionTs);

    ASSERT_GE(actualDifference, 0.0);
    ASSERT_LE(actualDifference, maxDifferenceInSeconds);
}

TEST(FunctionNowTests, positiveTest_MoreArguments_StackSize)
{
	s2e2::FunctionNow function;
    auto stack = TestUtils::createStack(false, std::string{"A"}, std::string{"B"});

    function.invoke(stack);

    ASSERT_EQ(4, stack.size());
}
