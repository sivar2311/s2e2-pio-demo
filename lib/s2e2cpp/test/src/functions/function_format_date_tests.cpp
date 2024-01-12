#include "../test_utils.hpp"

#include <s2e2/error.hpp>
#include <s2e2/functions/function_format_date.hpp>

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

TEST(FunctionFormatDateTests, positiveTest_GoodArguments_StackSize)
{
	s2e2::FunctionFormatDate function;
    auto stack = TestUtils::createStack(currentTm(), std::string{"%Y-%m-%d"});

    function.invoke(stack);

    ASSERT_EQ(1, stack.size());
}

TEST(FunctionFormatDateTests, positiveTest_GoodArguments_ResultType)
{
	s2e2::FunctionFormatDate function;
    auto stack = TestUtils::createStack(currentTm(), std::string{"%Y-%m-%d"});

    function.invoke(stack);

    ASSERT_TRUE(stack.top().has_value());
    ASSERT_EQ(typeid(std::string), stack.top().type());
}

TEST(FunctionFormatDateTests, positiveTest_GoodArguments_ResultValue)
{
    std::tm firstAgument;
    firstAgument.tm_year = 119;
    firstAgument.tm_mon = 6;
    firstAgument.tm_mday = 13;
    firstAgument.tm_hour = 12;
    firstAgument.tm_min = 15;
    firstAgument.tm_sec = 0;
    firstAgument.tm_isdst = 0;

    auto stack = TestUtils::createStack(firstAgument, std::string{"%Y-%m-%d"});
	s2e2::FunctionFormatDate function;

    function.invoke(stack);

    ASSERT_TRUE(stack.top().has_value());
    ASSERT_EQ(std::string{"2019-07-13"}, std::any_cast<std::string>(stack.top()));
}

TEST(FunctionFormatDateTests, positiveTest_MoreArguments_StackSize)
{
	s2e2::FunctionFormatDate function;
    auto stack = TestUtils::createStack(false, currentTm(), std::string{"%Y-%m-%d"});

    function.invoke(stack);

    ASSERT_EQ(2, stack.size());
}

TEST(FunctionFormatDateTests, negativeTest_FewerArguments)
{
	s2e2::FunctionFormatDate function;
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

TEST(FunctionFormatDateTests, negativeTest_FirstArgumentWrongType)
{
	s2e2::FunctionFormatDate function;
    auto stack = TestUtils::createStack(std::string{"2019-07-13"}, std::string{"%Y-%m-%d"});

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

TEST(FunctionFormatDateTests, negativeTest_FirstArgumentNull)
{
	s2e2::FunctionFormatDate function;
    auto stack = TestUtils::createStack(std::any{}, std::string{"%Y-%m-%d"});

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

TEST(FunctionFormatDateTests, negativeTest_SecondArgumentWrongType)
{
	s2e2::FunctionFormatDate function;
    auto stack = TestUtils::createStack(currentTm(), 15);

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

TEST(FunctionFormatDateTests, positiveTest_SecondArgumentWrongValue)
{
	s2e2::FunctionFormatDate function;
    auto stack = TestUtils::createStack(currentTm(), std::string{"year-month-day"});

    function.invoke(stack);

    ASSERT_TRUE(stack.top().has_value());
    ASSERT_EQ(std::string{"year-month-day"}, std::any_cast<std::string>(stack.top()));
}

TEST(FunctionFormatDateTests, negativeTest_SecondArgumentNull)
{
	s2e2::FunctionFormatDate function;
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
