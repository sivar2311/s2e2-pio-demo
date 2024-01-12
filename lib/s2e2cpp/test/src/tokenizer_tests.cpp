#include <token_type.hpp>
#include <token.hpp>
#include <tokenizer.hpp>

#include <s2e2/error.hpp>

#include <gtest/gtest.h>

#include <memory>



class TokenizerTests : public testing::Test
{
protected:
	void SetUp()
	{
		tokenizer = std::make_unique<s2e2::Tokenizer>();
	}

	void TearDown()
	{
		tokenizer.reset();
	}

protected:
	std::unique_ptr<s2e2::Tokenizer> tokenizer;
};

TEST_F(TokenizerTests, positiveTest_OneOperatorWithSpaces_ResultValue)
{
	tokenizer->addOperator("+");

    const auto actualTokens = tokenizer->tokenize("A + B");

    const auto expectedTokens = std::list<s2e2::Token>{s2e2::Token{s2e2::TokenType::ATOM, "A"},
                                                       s2e2::Token{s2e2::TokenType::OPERATOR, "+"},
                                                       s2e2::Token{s2e2::TokenType::ATOM, "B"}};

    ASSERT_EQ(expectedTokens, actualTokens);
}

TEST_F(TokenizerTests, positiveTest_OneOperatorWithoutSpaces_ResultValue)
{
    tokenizer->addOperator("+");

    const auto actualTokens = tokenizer->tokenize("A+B");

    const auto expectedTokens = std::list<s2e2::Token>{s2e2::Token{s2e2::TokenType::ATOM, "A"},
                                                       s2e2::Token{s2e2::TokenType::OPERATOR, "+"},
                                                       s2e2::Token{s2e2::TokenType::ATOM, "B"}};

    ASSERT_EQ(expectedTokens, actualTokens);
}

TEST_F(TokenizerTests, positiveTest_TwoOperatorWithSpaces_ResultValue)
{
    tokenizer->addOperator("+");
    tokenizer->addOperator("&&");

    const auto actualTokens = tokenizer->tokenize("A + B && C");

    const auto expectedTokens = std::list<s2e2::Token>{s2e2::Token{s2e2::TokenType::ATOM, "A"},
                                                       s2e2::Token{s2e2::TokenType::OPERATOR, "+"},
                                                       s2e2::Token{s2e2::TokenType::ATOM, "B"},
                                                       s2e2::Token{s2e2::TokenType::OPERATOR, "&&"},
                                                       s2e2::Token{s2e2::TokenType::ATOM, "C"}};

    ASSERT_EQ(expectedTokens, actualTokens);
}

TEST_F(TokenizerTests, positiveTest_TwoOperatorWithoutSpaces_ResultValue)
{
    tokenizer->addOperator("+");
    tokenizer->addOperator("&&");

    const auto actualTokens = tokenizer->tokenize("A+B&&C");

    const auto expectedTokens = std::list<s2e2::Token>{s2e2::Token{s2e2::TokenType::ATOM, "A"},
                                                       s2e2::Token{s2e2::TokenType::OPERATOR, "+"},
                                                       s2e2::Token{s2e2::TokenType::ATOM, "B"},
                                                       s2e2::Token{s2e2::TokenType::OPERATOR, "&&"},
                                                       s2e2::Token{s2e2::TokenType::ATOM, "C"}};

    ASSERT_EQ(expectedTokens, actualTokens);
}

TEST_F(TokenizerTests, positiveTest_OneOperatorIsSubstringOfAnother_ResultValue)
{
    tokenizer->addOperator("!");
    tokenizer->addOperator("!=");

    const auto actualTokens = tokenizer->tokenize("A != !B");

    const auto expectedTokens = std::list<s2e2::Token>{s2e2::Token{s2e2::TokenType::ATOM, "A"},
                                                       s2e2::Token{s2e2::TokenType::OPERATOR, "!="},
                                                       s2e2::Token{s2e2::TokenType::OPERATOR, "!"},
                                                       s2e2::Token{s2e2::TokenType::ATOM, "B"}};

    ASSERT_EQ(expectedTokens, actualTokens);
}

TEST_F(TokenizerTests, positiveTest_OneFunctionWithoutArguments_ResultValue)
{
    tokenizer->addFunction("FUN1");

    const auto actualTokens = tokenizer->tokenize("FUN1()");

    const auto expectedTokens = std::list<s2e2::Token>{s2e2::Token{s2e2::TokenType::FUNCTION, "FUN1"},
                                                       s2e2::Token{s2e2::TokenType::LEFT_BRACKET, "("},
                                                       s2e2::Token{s2e2::TokenType::RIGHT_BRACKET, ")"}};

    ASSERT_EQ(expectedTokens, actualTokens);
}

TEST_F(TokenizerTests, positiveTest_OneFunctionOneArgument_ResultValue)
{
    tokenizer->addFunction("FUN1");

    const auto actualTokens = tokenizer->tokenize("FUN1(Arg1)");

    const auto expectedTokens = std::list<s2e2::Token>{s2e2::Token{s2e2::TokenType::FUNCTION, "FUN1"},
                                                       s2e2::Token{s2e2::TokenType::LEFT_BRACKET, "("},
                                                       s2e2::Token{s2e2::TokenType::ATOM, "Arg1"},
                                                       s2e2::Token{s2e2::TokenType::RIGHT_BRACKET, ")"}};

    ASSERT_EQ(expectedTokens, actualTokens);
}

TEST_F(TokenizerTests, positiveTest_OneFunctionThreeArguments_ResultValue)
{
    tokenizer->addFunction("FUN1");

    const auto actualTokens = tokenizer->tokenize("FUN1(Arg1, Arg2,Arg3)");

    const auto expectedTokens = std::list<s2e2::Token>{s2e2::Token{s2e2::TokenType::FUNCTION, "FUN1"},
                                                       s2e2::Token{s2e2::TokenType::LEFT_BRACKET, "("},
                                                       s2e2::Token{s2e2::TokenType::ATOM, "Arg1"},
                                                       s2e2::Token{s2e2::TokenType::COMMA, ","},
                                                       s2e2::Token{s2e2::TokenType::ATOM, "Arg2"},
                                                       s2e2::Token{s2e2::TokenType::COMMA, ","},
                                                       s2e2::Token{s2e2::TokenType::ATOM, "Arg3"},
                                                       s2e2::Token{s2e2::TokenType::RIGHT_BRACKET, ")"}};

    ASSERT_EQ(expectedTokens, actualTokens);
}

TEST_F(TokenizerTests, positiveTest_TwoFunctionsOneOperator_ResultValue)
{
    tokenizer->addFunction("FUN1");
    tokenizer->addFunction("FUN2");
    tokenizer->addOperator("+");

    const auto actualTokens = tokenizer->tokenize("FUN1(Arg1) + FUN2(Arg2)");

    const auto expectedTokens = std::list<s2e2::Token>{s2e2::Token{s2e2::TokenType::FUNCTION, "FUN1"},
                                                       s2e2::Token{s2e2::TokenType::LEFT_BRACKET, "("},
                                                       s2e2::Token{s2e2::TokenType::ATOM, "Arg1"},
                                                       s2e2::Token{s2e2::TokenType::RIGHT_BRACKET, ")"},
                                                       s2e2::Token{s2e2::TokenType::OPERATOR, "+"},
                                                       s2e2::Token{s2e2::TokenType::FUNCTION, "FUN2"},
                                                       s2e2::Token{s2e2::TokenType::LEFT_BRACKET, "("},
                                                       s2e2::Token{s2e2::TokenType::ATOM, "Arg2"},
                                                       s2e2::Token{s2e2::TokenType::RIGHT_BRACKET, ")"}};

    ASSERT_EQ(expectedTokens, actualTokens);
}

TEST_F(TokenizerTests, positiveTest_NestedFunctions_ResultValue)
{
    tokenizer->addFunction("FUN1");
    tokenizer->addFunction("FUN2");
    tokenizer->addFunction("FUN3");

    const auto actualTokens = tokenizer->tokenize("FUN1(FUN2(), FUN3())");

    const auto expectedTokens = std::list<s2e2::Token>{s2e2::Token{s2e2::TokenType::FUNCTION, "FUN1"},
                                                       s2e2::Token{s2e2::TokenType::LEFT_BRACKET, "("},
                                                       s2e2::Token{s2e2::TokenType::FUNCTION, "FUN2"},
                                                       s2e2::Token{s2e2::TokenType::LEFT_BRACKET, "("},
                                                       s2e2::Token{s2e2::TokenType::RIGHT_BRACKET, ")"},
                                                       s2e2::Token{s2e2::TokenType::COMMA, ","},
                                                       s2e2::Token{s2e2::TokenType::FUNCTION, "FUN3"},
                                                       s2e2::Token{s2e2::TokenType::LEFT_BRACKET, "("},
                                                       s2e2::Token{s2e2::TokenType::RIGHT_BRACKET, ")"},
                                                       s2e2::Token{s2e2::TokenType::RIGHT_BRACKET, ")"}};

    ASSERT_EQ(expectedTokens, actualTokens);
}

TEST_F(TokenizerTests, positiveTest_NestedBrackets_ResultValue)
{
    tokenizer->addOperator("+");

    const auto actualTokens = tokenizer->tokenize("(((A + B)))");

    const auto expectedTokens = std::list<s2e2::Token>{s2e2::Token{s2e2::TokenType::LEFT_BRACKET, "("},
                                                       s2e2::Token{s2e2::TokenType::LEFT_BRACKET, "("},
                                                       s2e2::Token{s2e2::TokenType::LEFT_BRACKET, "("},
                                                       s2e2::Token{s2e2::TokenType::ATOM, "A"},
                                                       s2e2::Token{s2e2::TokenType::OPERATOR, "+"},
                                                       s2e2::Token{s2e2::TokenType::ATOM, "B"},
                                                       s2e2::Token{s2e2::TokenType::RIGHT_BRACKET, ")"},
                                                       s2e2::Token{s2e2::TokenType::RIGHT_BRACKET, ")"},
                                                       s2e2::Token{s2e2::TokenType::RIGHT_BRACKET, ")"}};

    ASSERT_EQ(expectedTokens, actualTokens);
}

TEST_F(TokenizerTests, positiveTest_OperatorsWithoutArguments_ResultValue)
{
    tokenizer->addOperator("+");

    const auto actualTokens = tokenizer->tokenize("+ + +");

    const auto expectedTokens = std::list<s2e2::Token>{s2e2::Token{s2e2::TokenType::OPERATOR, "+"},
                                                       s2e2::Token{s2e2::TokenType::OPERATOR, "+"},
                                                       s2e2::Token{s2e2::TokenType::OPERATOR, "+"}};

    ASSERT_EQ(expectedTokens, actualTokens);
}

TEST_F(TokenizerTests, positiveTest_UnpairedBrackets_ResultValue)
{
    const auto actualTokens = tokenizer->tokenize("((()");

    const auto expectedTokens = std::list<s2e2::Token>{s2e2::Token{s2e2::TokenType::LEFT_BRACKET, "("},
                                                       s2e2::Token{s2e2::TokenType::LEFT_BRACKET, "("},
                                                       s2e2::Token{s2e2::TokenType::LEFT_BRACKET, "("},
                                                       s2e2::Token{s2e2::TokenType::RIGHT_BRACKET, ")"}};

    ASSERT_EQ(expectedTokens, actualTokens);
}

TEST_F(TokenizerTests, negativeTest_TwoOperatorsWithTheSameName)
{
    tokenizer->addOperator("+");

    ASSERT_THROW({
        try
        {
            tokenizer->addOperator("+");
        }
        catch (const s2e2::Error& e)
        {
            ASSERT_STREQ("Tokenizer: operator + is already added", e.what());
            throw;
        }
    }, s2e2::Error);
}

TEST_F(TokenizerTests, negativeTest_TwoFunctionsWithTheSameName)
{
    tokenizer->addFunction("FUN");

    ASSERT_THROW({
        try
        {
            tokenizer->addFunction("FUN");
        }
        catch (const s2e2::Error& e)
        {
            ASSERT_STREQ("Tokenizer: function FUN is already added", e.what());
            throw;
        }
    }, s2e2::Error);
}

TEST_F(TokenizerTests, negativeTest_FunctionAndOperatorWithTheSameName)
{
    tokenizer->addFunction("FF");

    ASSERT_THROW({
        try
        {
            tokenizer->addOperator("FF");
        }
        catch (const s2e2::Error& e)
        {
            ASSERT_STREQ("Tokenizer: function FF is already added", e.what());
            throw;
        }
    }, s2e2::Error);
}

TEST_F(TokenizerTests, negativeTest_OperatorAndFunctionWithTheSameName)
{
    tokenizer->addOperator("FF");

    ASSERT_THROW({
        try
        {
            tokenizer->addFunction("FF");
        }
        catch (const s2e2::Error& e)
        {
            ASSERT_STREQ("Tokenizer: operator FF is already added", e.what());
            throw;
        }
    }, s2e2::Error);
}
