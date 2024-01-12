#include "../test_utils.hpp"

#include <s2e2/error.hpp>
#include <s2e2/functions/function_replace.hpp>

#include <gtest/gtest.h>

#include <typeinfo>


TEST(FunctionReplaceTests, positiveTest_StringReplace_StackSize)
{
	s2e2::FunctionReplace function;
    auto stack = TestUtils::createStack(std::string{"ABA"}, std::string{"A"}, std::string{"B"});

    function.invoke(stack);

    ASSERT_EQ(1, stack.size());
}

TEST(FunctionReplaceTests, positiveTest_StringReplace_ResultType)
{
	s2e2::FunctionReplace function;
    auto stack = TestUtils::createStack(std::string{"ABA"}, std::string{"A"}, std::string{"B"});

    function.invoke(stack);

    ASSERT_TRUE(stack.top().has_value());
    ASSERT_EQ(typeid(std::string), stack.top().type());
}

TEST(FunctionReplaceTests, positiveTest_StringReplace_ResultValue)
{
	s2e2::FunctionReplace function;
    auto stack = TestUtils::createStack(std::string{"ABA"}, std::string{"A"}, std::string{"B"});

    function.invoke(stack);

    ASSERT_TRUE(stack.top().has_value());
    ASSERT_EQ(std::string{"BBB"}, std::any_cast<std::string>(stack.top()));
}

TEST(FunctionReplaceTests, positiveTest_RegexReplace_ResultValue)
{
	s2e2::FunctionReplace function;
    auto stack = TestUtils::createStack(std::string{"ABCABA"}, std::string{"A.*?C"}, std::string{"D"});

    function.invoke(stack);

    ASSERT_TRUE(stack.top().has_value());
    ASSERT_EQ(std::string{"DABA"}, std::any_cast<std::string>(stack.top()));
}

TEST(FunctionReplaceTests, positiveTest_SpecialSymbolReplace_ResultValue)
{
	s2e2::FunctionReplace function;
    auto stack = TestUtils::createStack(std::string{"A * B == C"}, std::string{"\\*"}, std::string{"+"});

    function.invoke(stack);

    ASSERT_TRUE(stack.top().has_value());
    ASSERT_EQ(std::string{"A + B == C"}, std::any_cast<std::string>(stack.top()));
}

TEST(FunctionReplaceTests, positiveTest_FirstArgumentNull_ResultValue)
{
	s2e2::FunctionReplace function;
    auto stack = TestUtils::createStack(std::any{}, std::string{"A"}, std::string{"B"});

    function.invoke(stack);

    ASSERT_FALSE(stack.top().has_value());
}

TEST(FunctionReplaceTests, positiveTest_FirstArgumentEmptyString_ResultValue)
{
	s2e2::FunctionReplace function;
    auto stack = TestUtils::createStack(std::string{""}, std::string{"A"}, std::string{"B"});

    function.invoke(stack);

    ASSERT_TRUE(stack.top().has_value());
    ASSERT_EQ(std::string{""}, std::any_cast<std::string>(stack.top()));
}

TEST(FunctionReplaceTests, negativeTest_FirstArgument_WrongType)
{
	s2e2::FunctionReplace function;
    auto stack = TestUtils::createStack(5, std::string{"A"}, std::string{"B"});

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

TEST(FunctionReplaceTests, negativeTest_SecondArgument_EmptyString)
{
	s2e2::FunctionReplace function;
    auto stack = TestUtils::createStack(std::string{"ABA"}, std::string{""}, std::string{"B"});

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

TEST(FunctionReplaceTests, negativeTest_SecondArgument_Null)
{
	s2e2::FunctionReplace function;
    auto stack = TestUtils::createStack(std::string{"ABA"}, std::any{}, std::string{"B"});

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

TEST(FunctionReplaceTests, negativeTest_SecondArgument_WrongType)
{
	s2e2::FunctionReplace function;
    auto stack = TestUtils::createStack(std::string{"AB5"}, 5, std::string{"B"});

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

TEST(FunctionReplaceTests, positiveTest_ThirdArgumentEmptyString_ResultValue)
{
	s2e2::FunctionReplace function;
    auto stack = TestUtils::createStack(std::string{"ABA"}, std::string{"B"}, std::string{""});

    function.invoke(stack);

    ASSERT_TRUE(stack.top().has_value());
    ASSERT_EQ(std::string{"AA"}, std::any_cast<std::string>(stack.top()));
}

TEST(FunctionReplaceTests, negativeTest_ThirdArgument_Null)
{
	s2e2::FunctionReplace function;
    auto stack = TestUtils::createStack(std::string{"ABA"}, std::string{"B"}, std::any{});

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

TEST(FunctionReplaceTests, negativeTest_ThirdArgument_WrongType)
{
	s2e2::FunctionReplace function;
    auto stack = TestUtils::createStack(std::string{"ABA"}, std::string{"B"}, 5);

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

TEST(FunctionReplaceTests, positiveTest_MoreArguments_StackSize)
{
	s2e2::FunctionReplace function;
    auto stack = TestUtils::createStack(false, std::string{"ABA"}, std::string{"A"}, std::string{"B"});

    function.invoke(stack);

    ASSERT_EQ(2, stack.size());
}

TEST(FunctionReplaceTests, negativeTest_FewerArguments)
{
	s2e2::FunctionReplace function;
    auto stack = TestUtils::createStack(std::string{"ABA"}, std::string{"A"});

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
