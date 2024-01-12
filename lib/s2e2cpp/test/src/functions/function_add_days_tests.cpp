#include "../test_utils.hpp"

#include <s2e2/error.hpp>
#include <s2e2/functions/function_add_days.hpp>

#include <gtest/gtest.h>

#include <ctime>
#include <typeinfo>


namespace
{
    std::tm currentTm()
    {
        const auto now = std::time(nullptr);
        return *std::gmtime(&now);
    }
}

TEST(FunctionAddDaysTests, positiveTest_GoodArguments_StackSize)
{
	s2e2::FunctionAddDays function;
    auto stack = TestUtils::createStack(currentTm(), std::string{"1"});

    function.invoke(stack);

    ASSERT_EQ(1, stack.size());
}

TEST(FunctionAddDaysTests, positiveTest_GoodArguments_ResultType)
{
	s2e2::FunctionAddDays function;
    auto stack = TestUtils::createStack(currentTm(), std::string{"1"});

    function.invoke(stack);

    ASSERT_TRUE(stack.top().has_value());
    ASSERT_EQ(typeid(std::tm), stack.top().type());
}

TEST(FunctionAddDaysTests, positiveTest_SecondArgumentPositive_ResultValue)
{
    std::tm firstAgument;
    firstAgument.tm_year = 119;
    firstAgument.tm_mon = 6;
    firstAgument.tm_mday = 13;
    firstAgument.tm_hour = 12;
    firstAgument.tm_min = 15;
    firstAgument.tm_sec = 0;
    firstAgument.tm_isdst = 0;

    auto stack = TestUtils::createStack(firstAgument, std::string{"35"});
    s2e2::FunctionAddDays function;

    function.invoke(stack);

    ASSERT_TRUE(stack.top().has_value());

    const auto result = std::any_cast<std::tm>(stack.top());
    ASSERT_EQ(119, result.tm_year);
    ASSERT_EQ(7, result.tm_mon);
    ASSERT_EQ(17, result.tm_mday);
    ASSERT_EQ(12, result.tm_hour);
    ASSERT_EQ(15, result.tm_min);
    ASSERT_EQ(0, result.tm_sec);
}

TEST(FunctionAddDaysTests, positiveTest_SecondArgumentZero_ResultValue)
{
    std::tm firstAgument;
    firstAgument.tm_year = 119;
    firstAgument.tm_mon = 6;
    firstAgument.tm_mday = 13;
    firstAgument.tm_hour = 12;
    firstAgument.tm_min = 15;
    firstAgument.tm_sec = 0;
    firstAgument.tm_isdst = 0;

    auto stack = TestUtils::createStack(firstAgument, std::string{"0"});
    s2e2::FunctionAddDays function;

    function.invoke(stack);

    ASSERT_TRUE(stack.top().has_value());

    const auto result = std::any_cast<std::tm>(stack.top());
    ASSERT_EQ(119, result.tm_year);
    ASSERT_EQ(6, result.tm_mon);
    ASSERT_EQ(13, result.tm_mday);
    ASSERT_EQ(12, result.tm_hour);
    ASSERT_EQ(15, result.tm_min);
    ASSERT_EQ(0, result.tm_sec);
}

TEST(FunctionAddDaysTests, positiveTest_SecondArgumentNegative_ResultValue)
{
    std::tm firstAgument;
    firstAgument.tm_year = 119;
    firstAgument.tm_mon = 6;
    firstAgument.tm_mday = 13;
    firstAgument.tm_hour = 12;
    firstAgument.tm_min = 15;
    firstAgument.tm_sec = 0;
    firstAgument.tm_isdst = 0;

    auto stack = TestUtils::createStack(firstAgument, std::string{"-35"});
    s2e2::FunctionAddDays function;

    function.invoke(stack);

    ASSERT_TRUE(stack.top().has_value());

    const auto result = std::any_cast<std::tm>(stack.top());
    ASSERT_EQ(119, result.tm_year);
    ASSERT_EQ(5, result.tm_mon);
    ASSERT_EQ(8, result.tm_mday);
    ASSERT_EQ(12, result.tm_hour);
    ASSERT_EQ(15, result.tm_min);
    ASSERT_EQ(0, result.tm_sec);
}

TEST(FunctionAddDaysTests, positiveTest_MoreArguments_StackSize)
{
	s2e2::FunctionAddDays function;
    auto stack = TestUtils::createStack(std::string{"Arg"}, currentTm(), std::string{"1"});

    function.invoke(stack);

     ASSERT_EQ(2, stack.size());
}

TEST(FunctionAddDaysTests, negativeTest_FewerArguments)
{
	s2e2::FunctionAddDays function;
    auto stack = TestUtils::createStack(currentTm());

    ASSERT_THROW({
        try
        {
            function.invoke(stack);
        }
        catch (const s2e2::Error& e)
        {
            ASSERT_EQ("Not enough arguments for function " + function.name, e.what());
            throw;
        }
    }, s2e2::Error);
}

TEST(FunctionAddDaysTests, negativeTest_FirstArgumentWrongType)
{
	s2e2::FunctionAddDays function;
    auto stack = TestUtils::createStack(std::string{"2019-07-13 00:00:00"}, std::string{"1"});

    ASSERT_THROW({
        try
        {
            function.invoke(stack);
        }
        catch (const s2e2::Error& e)
        {
            ASSERT_EQ("Invalid arguments for function " + function.name, e.what());
            throw;
        }
    }, s2e2::Error);
}

TEST(FunctionAddDaysTests, negativeTest_FirstArgumentNull)
{
	s2e2::FunctionAddDays function;
    auto stack = TestUtils::createStack(std::any{}, std::string{"1"});

    ASSERT_THROW({
        try
        {
            function.invoke(stack);
        }
        catch (const s2e2::Error& e)
        {
            ASSERT_EQ("Invalid arguments for function " + function.name, e.what());
            throw;
        }
    }, s2e2::Error);
}

TEST(FunctionAddDaysTests, negativeTest_SecondArgumentWrongType)
{
	s2e2::FunctionAddDays function;
    auto stack = TestUtils::createStack(currentTm(), 1);

    ASSERT_THROW({
        try
        {
            function.invoke(stack);
        }
        catch (const s2e2::Error& e)
        {
            ASSERT_EQ("Invalid arguments for function " + function.name, e.what());
            throw;
        }
    }, s2e2::Error);
}

TEST(FunctionAddDaysTests, negativeTest_SecondArgumentWrongValue)
{
	s2e2::FunctionAddDays function;
    auto stack = TestUtils::createStack(currentTm(), std::string{"A"});

    ASSERT_THROW({
        try
        {
            function.invoke(stack);
        }
        catch (const s2e2::Error& e)
        {
            ASSERT_EQ("Invalid arguments for function " + function.name, e.what());
            throw;
        }
    }, s2e2::Error);
}

TEST(FunctionAddDaysTests, negativeTest_SecondArgumentNull)
{
	s2e2::FunctionAddDays function;
    auto stack = TestUtils::createStack(currentTm(), std::any{});

    ASSERT_THROW({
        try
        {
            function.invoke(stack);
        }
        catch (const s2e2::Error& e)
        {
            ASSERT_EQ("Invalid arguments for function " + function.name, e.what());
            throw;
        }
    }, s2e2::Error);
}
