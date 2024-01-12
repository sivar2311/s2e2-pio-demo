#include <converter.hpp>
#include <token_type.hpp>
#include <token.hpp>

#include <s2e2/error.hpp>

#include <gtest/gtest.h>

#include <memory>


class ConverterTests : public testing::Test
{
protected:
	void SetUp()
	{
		converter = std::make_unique<s2e2::Converter>();
	}

	void TearDown()
	{
		converter.reset();
	}

protected:
	std::unique_ptr<s2e2::Converter> converter;
};

TEST_F(ConverterTests, positiveTest_OneBinaryOperator_ResultValue)
{
	converter->addOperator("+", 1);

    const auto inputTokens = std::list<s2e2::Token>{s2e2::Token{s2e2::TokenType::ATOM, "A"},
                                                    s2e2::Token{s2e2::TokenType::OPERATOR, "+"},
                                                    s2e2::Token{s2e2::TokenType::ATOM, "B"}};

    const auto actualTokens = converter->convert(inputTokens);

    const auto expectedTokens = std::list<s2e2::Token>{s2e2::Token{s2e2::TokenType::ATOM, "A"},
                                                       s2e2::Token{s2e2::TokenType::ATOM, "B"},
                                                       s2e2::Token{s2e2::TokenType::OPERATOR, "+"}};

    ASSERT_EQ(expectedTokens, actualTokens);
}

TEST_F(ConverterTests, positiveTest_TwoBinaryOperatorsSamePriority_ResultValue)
{
	converter->addOperator("+", 1);
    converter->addOperator("-", 1);

    const auto inputTokens = std::list<s2e2::Token>{s2e2::Token{s2e2::TokenType::ATOM, "A"},
                                                    s2e2::Token{s2e2::TokenType::OPERATOR, "+"},
                                                    s2e2::Token{s2e2::TokenType::ATOM, "B"},
                                                    s2e2::Token{s2e2::TokenType::OPERATOR, "-"},
                                                    s2e2::Token{s2e2::TokenType::ATOM, "C"}};

    const auto actualTokens = converter->convert(inputTokens);

    const auto expectedTokens = std::list<s2e2::Token>{s2e2::Token{s2e2::TokenType::ATOM, "A"},
                                                       s2e2::Token{s2e2::TokenType::ATOM, "B"},
                                                       s2e2::Token{s2e2::TokenType::OPERATOR, "+"},
                                                       s2e2::Token{s2e2::TokenType::ATOM, "C"},
                                                       s2e2::Token{s2e2::TokenType::OPERATOR, "-"}};

    ASSERT_EQ(expectedTokens, actualTokens);
}

TEST_F(ConverterTests, positiveTest_TwoOperatorsDifferentPriorities_ResultValue)
{
	converter->addOperator("+", 1);
    converter->addOperator("*", 2);

    const auto inputTokens = std::list<s2e2::Token>{s2e2::Token{s2e2::TokenType::ATOM, "A"},
                                                    s2e2::Token{s2e2::TokenType::OPERATOR, "+"},
                                                    s2e2::Token{s2e2::TokenType::ATOM, "B"},
                                                    s2e2::Token{s2e2::TokenType::OPERATOR, "*"},
                                                    s2e2::Token{s2e2::TokenType::ATOM, "C"}};

    const auto actualTokens = converter->convert(inputTokens);

    const auto expectedTokens = std::list<s2e2::Token>{s2e2::Token{s2e2::TokenType::ATOM, "A"},
                                                       s2e2::Token{s2e2::TokenType::ATOM, "B"},
                                                       s2e2::Token{s2e2::TokenType::ATOM, "C"},
                                                       s2e2::Token{s2e2::TokenType::OPERATOR, "*"},
                                                       s2e2::Token{s2e2::TokenType::OPERATOR, "+"}};

    ASSERT_EQ(expectedTokens, actualTokens);
}

TEST_F(ConverterTests, positiveTest_UnaryOperatorAndBinaryOperator_ResultValue)
{
	converter->addOperator("!=", 1);
    converter->addOperator("!", 2);

    const auto inputTokens = std::list<s2e2::Token>{s2e2::Token{s2e2::TokenType::OPERATOR, "!"},
                                                    s2e2::Token{s2e2::TokenType::ATOM, "A"},
                                                    s2e2::Token{s2e2::TokenType::OPERATOR, "!="},
                                                    s2e2::Token{s2e2::TokenType::ATOM, "B"}};

    const auto actualTokens = converter->convert(inputTokens);

    const auto expectedTokens = std::list<s2e2::Token>{s2e2::Token{s2e2::TokenType::ATOM, "A"},
                                                       s2e2::Token{s2e2::TokenType::OPERATOR, "!"},
                                                       s2e2::Token{s2e2::TokenType::ATOM, "B"},
                                                       s2e2::Token{s2e2::TokenType::OPERATOR, "!="}};

    ASSERT_EQ(expectedTokens, actualTokens);
}

TEST_F(ConverterTests, positiveTest_OneFunctionWithoutArguments_ResultValue)
{
    const auto inputTokens = std::list<s2e2::Token>{s2e2::Token{s2e2::TokenType::FUNCTION, "FUN"},
                                                    s2e2::Token{s2e2::TokenType::LEFT_BRACKET, "("},
                                                    s2e2::Token{s2e2::TokenType::RIGHT_BRACKET, ")"}};

    const auto actualTokens = converter->convert(inputTokens);

    const auto expectedTokens = std::list<s2e2::Token>{s2e2::Token{s2e2::TokenType::FUNCTION, "FUN"}};

    ASSERT_EQ(expectedTokens, actualTokens);
}

TEST_F(ConverterTests, positiveTest_OneFunctionOneArgument_ResultValue)
{
    const auto inputTokens = std::list<s2e2::Token>{s2e2::Token{s2e2::TokenType::FUNCTION, "FUN"},
                                                    s2e2::Token{s2e2::TokenType::LEFT_BRACKET, "("},
                                                    s2e2::Token{s2e2::TokenType::ATOM, "Arg1"},
                                                    s2e2::Token{s2e2::TokenType::RIGHT_BRACKET, ")"}};

    const auto actualTokens = converter->convert(inputTokens);

    const auto expectedTokens = std::list<s2e2::Token>{s2e2::Token{s2e2::TokenType::ATOM, "Arg1"},
                                                       s2e2::Token{s2e2::TokenType::FUNCTION, "FUN"}};

    ASSERT_EQ(expectedTokens, actualTokens);
}

TEST_F(ConverterTests, positiveTest_OneFunctionThreeArguments_ResultValue)
{
    const auto inputTokens = std::list<s2e2::Token>{s2e2::Token{s2e2::TokenType::FUNCTION, "FUN"},
                                                    s2e2::Token{s2e2::TokenType::LEFT_BRACKET, "("},
                                                    s2e2::Token{s2e2::TokenType::ATOM, "Arg1"},
                                                    s2e2::Token{s2e2::TokenType::ATOM, "Arg2"},
                                                    s2e2::Token{s2e2::TokenType::ATOM, "Arg3"},
                                                    s2e2::Token{s2e2::TokenType::RIGHT_BRACKET, ")"}};

    const auto actualTokens = converter->convert(inputTokens);

    const auto expectedTokens = std::list<s2e2::Token>{s2e2::Token{s2e2::TokenType::ATOM, "Arg1"},
                                                       s2e2::Token{s2e2::TokenType::ATOM, "Arg2"},
                                                       s2e2::Token{s2e2::TokenType::ATOM, "Arg3"},
                                                       s2e2::Token{s2e2::TokenType::FUNCTION, "FUN"}};

    ASSERT_EQ(expectedTokens, actualTokens);
}

TEST_F(ConverterTests, positiveTest_FunctionAndExernalOperator_ResultValue)
{
    converter->addOperator("+", 1);

    const auto inputTokens = std::list<s2e2::Token>{s2e2::Token{s2e2::TokenType::FUNCTION, "FUN"},
                                                    s2e2::Token{s2e2::TokenType::LEFT_BRACKET, "("},
                                                    s2e2::Token{s2e2::TokenType::ATOM, "Arg1"},
                                                    s2e2::Token{s2e2::TokenType::RIGHT_BRACKET, ")"},
                                                    s2e2::Token{s2e2::TokenType::OPERATOR, "+"},
                                                    s2e2::Token{s2e2::TokenType::FUNCTION, "FUN"},
                                                    s2e2::Token{s2e2::TokenType::LEFT_BRACKET, "("},
                                                    s2e2::Token{s2e2::TokenType::ATOM, "Arg2"},
                                                    s2e2::Token{s2e2::TokenType::RIGHT_BRACKET, ")"}};

    const auto actualTokens = converter->convert(inputTokens);

    const auto expectedTokens = std::list<s2e2::Token>{s2e2::Token{s2e2::TokenType::ATOM, "Arg1"},
                                                       s2e2::Token{s2e2::TokenType::FUNCTION, "FUN"},
                                                       s2e2::Token{s2e2::TokenType::ATOM, "Arg2"},
                                                       s2e2::Token{s2e2::TokenType::FUNCTION, "FUN"},
                                                       s2e2::Token{s2e2::TokenType::OPERATOR, "+"}};

    ASSERT_EQ(expectedTokens, actualTokens);
}

TEST_F(ConverterTests, positiveTest_FunctionAndInternalOperator_ResultValue)
{
    converter->addOperator("+", 1);

    const auto inputTokens = std::list<s2e2::Token>{s2e2::Token{s2e2::TokenType::FUNCTION, "FUN"},
                                                    s2e2::Token{s2e2::TokenType::LEFT_BRACKET, "("},
                                                    s2e2::Token{s2e2::TokenType::ATOM, "Arg1"},
                                                    s2e2::Token{s2e2::TokenType::OPERATOR, "+"},
                                                    s2e2::Token{s2e2::TokenType::ATOM, "Arg2"},
                                                    s2e2::Token{s2e2::TokenType::COMMA, ","},
                                                    s2e2::Token{s2e2::TokenType::ATOM, "Arg3"},
                                                    s2e2::Token{s2e2::TokenType::OPERATOR, "+"},
                                                    s2e2::Token{s2e2::TokenType::ATOM, "Arg4"},
                                                    s2e2::Token{s2e2::TokenType::RIGHT_BRACKET, ")"}};

    const auto actualTokens = converter->convert(inputTokens);

    const auto expectedTokens = std::list<s2e2::Token>{s2e2::Token{s2e2::TokenType::ATOM, "Arg1"},
                                                       s2e2::Token{s2e2::TokenType::ATOM, "Arg2"},
                                                       s2e2::Token{s2e2::TokenType::OPERATOR, "+"},
                                                       s2e2::Token{s2e2::TokenType::ATOM, "Arg3"},
                                                       s2e2::Token{s2e2::TokenType::ATOM, "Arg4"},
                                                       s2e2::Token{s2e2::TokenType::OPERATOR, "+"},
                                                       s2e2::Token{s2e2::TokenType::FUNCTION, "FUN"}};

    ASSERT_EQ(expectedTokens, actualTokens);
}

TEST_F(ConverterTests, positiveTest_NestedFunctions_ResultValue)
{
    const auto inputTokens = std::list<s2e2::Token>{s2e2::Token{s2e2::TokenType::FUNCTION, "FUN1"},
                                                    s2e2::Token{s2e2::TokenType::LEFT_BRACKET, "("},
                                                    s2e2::Token{s2e2::TokenType::FUNCTION, "FUN2"},
                                                    s2e2::Token{s2e2::TokenType::LEFT_BRACKET, "("},
                                                    s2e2::Token{s2e2::TokenType::RIGHT_BRACKET, ")"},
                                                    s2e2::Token{s2e2::TokenType::COMMA, ","},
                                                    s2e2::Token{s2e2::TokenType::FUNCTION, "FUN3"},
                                                    s2e2::Token{s2e2::TokenType::LEFT_BRACKET, "("},
                                                    s2e2::Token{s2e2::TokenType::ATOM, "Arg1"},
                                                    s2e2::Token{s2e2::TokenType::COMMA, ","},
                                                    s2e2::Token{s2e2::TokenType::ATOM, "Arg2"},
                                                    s2e2::Token{s2e2::TokenType::RIGHT_BRACKET, ")"},
                                                    s2e2::Token{s2e2::TokenType::RIGHT_BRACKET, ")"}};

    const auto actualTokens = converter->convert(inputTokens);

    const auto expectedTokens = std::list<s2e2::Token>{s2e2::Token{s2e2::TokenType::FUNCTION, "FUN2"},
                                                       s2e2::Token{s2e2::TokenType::ATOM, "Arg1"},
                                                       s2e2::Token{s2e2::TokenType::ATOM, "Arg2"},
                                                       s2e2::Token{s2e2::TokenType::FUNCTION, "FUN3"},
                                                       s2e2::Token{s2e2::TokenType::FUNCTION, "FUN1"}};

    ASSERT_EQ(expectedTokens, actualTokens);
}

TEST_F(ConverterTests, positiveTest_OperatorsWithoutArguments_ResultValue)
{
    converter->addOperator("+", 1);

    const auto inputTokens = std::list<s2e2::Token>{s2e2::Token{s2e2::TokenType::OPERATOR, "+"},
                                                    s2e2::Token{s2e2::TokenType::OPERATOR, "+"},
                                                    s2e2::Token{s2e2::TokenType::OPERATOR, "+"}};

    const auto actualTokens = converter->convert(inputTokens);

    const auto expectedTokens = std::list<s2e2::Token>{s2e2::Token{s2e2::TokenType::OPERATOR, "+"},
                                                       s2e2::Token{s2e2::TokenType::OPERATOR, "+"},
                                                       s2e2::Token{s2e2::TokenType::OPERATOR, "+"}};

    ASSERT_EQ(expectedTokens, actualTokens);
}

TEST_F(ConverterTests, positiveTest_FunctionWithoutCommas_ResultValue)
{
    const auto inputTokens = std::list<s2e2::Token>{s2e2::Token{s2e2::TokenType::FUNCTION, "FUN"},
                                                    s2e2::Token{s2e2::TokenType::LEFT_BRACKET, "("},
                                                    s2e2::Token{s2e2::TokenType::ATOM, "Arg1"},
                                                    s2e2::Token{s2e2::TokenType::ATOM, "Arg2"},
                                                    s2e2::Token{s2e2::TokenType::RIGHT_BRACKET, ")"}};

    const auto actualTokens = converter->convert(inputTokens);

    const auto expectedTokens = std::list<s2e2::Token>{s2e2::Token{s2e2::TokenType::ATOM, "Arg1"},
                                                       s2e2::Token{s2e2::TokenType::ATOM, "Arg2"},
                                                       s2e2::Token{s2e2::TokenType::FUNCTION, "FUN"}};

    ASSERT_EQ(expectedTokens, actualTokens);
}

TEST_F(ConverterTests, positiveTest_FunctionOfOperators_ResultValue)
{
    converter->addOperator("+", 1);

    const auto inputTokens = std::list<s2e2::Token>{s2e2::Token{s2e2::TokenType::FUNCTION, "FUN"},
                                                    s2e2::Token{s2e2::TokenType::LEFT_BRACKET, "("},
                                                    s2e2::Token{s2e2::TokenType::OPERATOR, "+"},
                                                    s2e2::Token{s2e2::TokenType::OPERATOR, "+"},
                                                    s2e2::Token{s2e2::TokenType::RIGHT_BRACKET, ")"}};

    const auto actualTokens = converter->convert(inputTokens);

    const auto expectedTokens = std::list<s2e2::Token>{s2e2::Token{s2e2::TokenType::OPERATOR, "+"},
                                                       s2e2::Token{s2e2::TokenType::OPERATOR, "+"},
                                                       s2e2::Token{s2e2::TokenType::FUNCTION, "FUN"}};

    ASSERT_EQ(expectedTokens, actualTokens);
}

TEST_F(ConverterTests, negativeTest_UnpairedLeftBracket)
{
    const auto inputTokens = std::list<s2e2::Token>{s2e2::Token{s2e2::TokenType::FUNCTION, "FUN"},
                                                    s2e2::Token{s2e2::TokenType::LEFT_BRACKET, "("},
                                                    s2e2::Token{s2e2::TokenType::ATOM, "Arg1"}};

    ASSERT_THROW({
        try
        {
            converter->convert(inputTokens);
        }
        catch (const s2e2::Error& e)
        {
            ASSERT_STREQ("Converter: unpaired bracket", e.what());
            throw;
        }
    }, s2e2::Error);
}

TEST_F(ConverterTests, negativeTest_UnpairedRightBracket)
{
    const auto inputTokens = std::list<s2e2::Token>{s2e2::Token{s2e2::TokenType::FUNCTION, "FUN"},
                                                    s2e2::Token{s2e2::TokenType::ATOM, "Arg1"},
                                                    s2e2::Token{s2e2::TokenType::RIGHT_BRACKET, ")"}};

    ASSERT_THROW({
        try
        {
            converter->convert(inputTokens);
        }
        catch (const s2e2::Error& e)
        {
            ASSERT_STREQ("Converter: unpaired bracket", e.what());
            throw;
        }
    }, s2e2::Error);
}

TEST_F(ConverterTests, negativeTest_TwoOperatorsWithTheSameName)
{
    converter->addOperator("+", 1);

    ASSERT_THROW({
        try
        {
            converter->addOperator("+", 1);
        }
        catch (const s2e2::Error& e)
        {
            ASSERT_STREQ("Converter: operator + is already added", e.what());
            throw;
        }
    }, s2e2::Error);
}

TEST_F(ConverterTests, negativeTest_UnknownOperator)
{
    converter->addOperator("+", 1);

    const auto inputTokens = std::list<s2e2::Token>{s2e2::Token{s2e2::TokenType::ATOM, "Arg1"},
                                                    s2e2::Token{s2e2::TokenType::OPERATOR, "+"},
                                                    s2e2::Token{s2e2::TokenType::ATOM, "Arg2"},
                                                    s2e2::Token{s2e2::TokenType::OPERATOR, "*"},
                                                    s2e2::Token{s2e2::TokenType::ATOM, "Arg3"}};

    ASSERT_THROW({
        try
        {
            converter->convert(inputTokens);
        }
        catch (const s2e2::Error& e)
        {
            ASSERT_STREQ("Converter: unknown operator *", e.what());
            throw;
        }
    }, s2e2::Error);
}
