#include "converter_mock.hpp"
#include "tokenizer_mock.hpp"

#include <evaluator_impl.hpp>
#include <token_type.hpp>
#include <token.hpp>

#include <s2e2/error.hpp>
#include <s2e2/function.hpp>
#include <s2e2/operator.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <list>
#include <memory>
#include <stdexcept>


using namespace ::testing;

namespace
{
    const std::string DUMMY_FUNCTION_NAME = "Function";
    const std::string DUMMY_OPERATOR_NAME = "Operator";
    const uint_fast16_t DUMMY_OPERATOR_PRIORITY = 1;
}

class EvaluatorTests : public testing::Test
{
protected:
	void SetUp()
	{
        converterMock = nullptr;
        tokenizerMock = nullptr;
		evaluator.reset();
	}

	void makeRealEvaluator()
	{
		evaluator = std::make_unique<s2e2::EvaluatorImpl>();
	}

	void makeMockedEvaluator()
	{
        auto cmock = std::make_unique<ConverterNiceMock>();
        converterMock = cmock.get();

        auto tmock = std::make_unique<TokenizerNiceMock>();
        tokenizerMock = tmock.get();

		evaluator = std::make_unique<s2e2::EvaluatorImpl>(std::move(cmock), std::move(tmock));
	}

    std::unique_ptr<s2e2::Function> dummyFunction() const
    {
        return std::make_unique<DummyFunction>();
    }

    std::unique_ptr<s2e2::Operator> dummyOperator() const
    {
        return std::make_unique<DummyOperator>();
    }

protected:
    ConverterMock* converterMock = nullptr;
    TokenizerMock* tokenizerMock = nullptr;
	std::unique_ptr<s2e2::EvaluatorImpl> evaluator;

private:
    class DummyFunction final : public s2e2::Function
    {
    public:
        DummyFunction()
            : s2e2::Function(DUMMY_FUNCTION_NAME, 2)
        {}

    private:
        bool checkArguments() const
        {
            return true;
        }

        std::any result() const
        {
            return {std::string{"FunctionResult"}};
        }
    };

    class DummyOperator final : public s2e2::Operator
    {
    public:
        DummyOperator()
            : s2e2::Operator(DUMMY_OPERATOR_NAME, DUMMY_OPERATOR_PRIORITY, 2)
        {}

    private:
        bool checkArguments() const
        {
            return true;
        }

        std::any result() const
        {
            return {std::string{"OperatorResult"}};
        }
    };
};

TEST_F(EvaluatorTests, positiveTest_NothingAdded_SupportedFunctionsSize)
{
	makeRealEvaluator();

    ASSERT_TRUE(evaluator->getFunctions().empty());
}

TEST_F(EvaluatorTests, positiveTest_NothingAdded_SupportedOperatorsSize)
{
	makeRealEvaluator();

    ASSERT_TRUE(evaluator->getOperators().empty());
}

TEST_F(EvaluatorTests, positiveTest_NothingAdded_EvaluationResult)
{
	makeRealEvaluator();

    const std::string inputExpression = "A B C";
    const auto evaluatedExpression = evaluator->evaluate(inputExpression);

    ASSERT_TRUE(evaluatedExpression);
    ASSERT_EQ(inputExpression, *evaluatedExpression);
}

TEST_F(EvaluatorTests, positiveTest_AddFunction_SupportedFunctionsSize)
{
	makeRealEvaluator();

    evaluator->addFunction(dummyFunction());

    ASSERT_EQ(1, evaluator->getFunctions().size());
}

TEST_F(EvaluatorTests, positiveTest_AddFunction_VerifyTokenizer)
{
	makeMockedEvaluator();

    EXPECT_CALL(*tokenizerMock, addFunction(DUMMY_FUNCTION_NAME)).Times(1);

    evaluator->addFunction(dummyFunction());
}

TEST_F(EvaluatorTests, positiveTest_AddOperator_SupportedOperatorsSize)
{
	makeRealEvaluator();

    evaluator->addOperator(dummyOperator());

    ASSERT_EQ(1, evaluator->getOperators().size());
}

TEST_F(EvaluatorTests, positiveTest_AddOperator_VerifyConverter)
{
	makeMockedEvaluator();

    EXPECT_CALL(*converterMock, addOperator(DUMMY_OPERATOR_NAME, DUMMY_OPERATOR_PRIORITY)).Times(1);

    evaluator->addOperator(dummyOperator());
}

TEST_F(EvaluatorTests, positiveTest_AddOperator_VerifyTokenizer)
{
	makeMockedEvaluator();

    EXPECT_CALL(*tokenizerMock, addOperator(DUMMY_OPERATOR_NAME)).Times(1);

    evaluator->addOperator(dummyOperator());
}

TEST_F(EvaluatorTests, positiveTest_AddStandardFunctions_SupportedFunctionsSize)
{
	makeRealEvaluator();

    evaluator->addStandardFunctions();

    ASSERT_FALSE(evaluator->getFunctions().empty());
}

TEST_F(EvaluatorTests, positiveTest_AddStandardOperators_SupportedFunctionsSize)
{
	makeRealEvaluator();

    evaluator->addStandardOperators();

    ASSERT_FALSE(evaluator->getOperators().empty());
}

TEST_F(EvaluatorTests, positiveTest_Evaluate_VerifyConverter)
{
	makeMockedEvaluator();

    const auto expression = "A " + DUMMY_OPERATOR_NAME + " B";

    const auto infixTokens = std::list<s2e2::Token>{s2e2::Token{s2e2::TokenType::ATOM, "A"},
                                                    s2e2::Token{s2e2::TokenType::OPERATOR, DUMMY_OPERATOR_NAME},
                                                    s2e2::Token{s2e2::TokenType::ATOM, "B"}};

    const auto postfixTokens = std::list<s2e2::Token>{s2e2::Token{s2e2::TokenType::ATOM, "A"},
                                                      s2e2::Token{s2e2::TokenType::ATOM, "B"},
                                                      s2e2::Token{s2e2::TokenType::OPERATOR, DUMMY_OPERATOR_NAME}};

    ON_CALL(*tokenizerMock, tokenize(_)).WillByDefault(Return(infixTokens));

    EXPECT_CALL(*converterMock, convert(infixTokens)).Times(1);
    ON_CALL(*converterMock, convert(_)).WillByDefault(Return(postfixTokens));
    
    evaluator->addOperator(dummyOperator());
    evaluator->evaluate(expression);
}

TEST_F(EvaluatorTests, positiveTest_Evaluate_VerifyTokenizer)
{
	makeMockedEvaluator();

    const auto expression = "A " + DUMMY_OPERATOR_NAME + " B";

    const auto postfixTokens = std::list<s2e2::Token>{s2e2::Token{s2e2::TokenType::ATOM, "A"},
                                                      s2e2::Token{s2e2::TokenType::ATOM, "B"},
                                                      s2e2::Token{s2e2::TokenType::OPERATOR, DUMMY_OPERATOR_NAME}};

    EXPECT_CALL(*tokenizerMock, tokenize(expression)).Times(1);

    ON_CALL(*converterMock, convert(_)).WillByDefault(Return(postfixTokens));
    
    evaluator->addOperator(dummyOperator());
    evaluator->evaluate(expression);
}

TEST_F(EvaluatorTests, positiveTest_OneOperator_EvaluationResult)
{
	makeRealEvaluator();

    evaluator->addStandardFunctions();
    evaluator->addStandardOperators();

    const auto result = evaluator->evaluate("A + B");

    ASSERT_TRUE(result);
    ASSERT_EQ("AB", *result);
}

TEST_F(EvaluatorTests, positiveTest_TwoOperator_EvaluationResult)
{
	makeRealEvaluator();

    evaluator->addStandardFunctions();
    evaluator->addStandardOperators();

    const auto result = evaluator->evaluate("A + B + C");

    ASSERT_TRUE(result);
    ASSERT_EQ("ABC", *result);
}

TEST_F(EvaluatorTests, positiveTest_OneFunction_EvaluationResult)
{
	makeRealEvaluator();

    evaluator->addStandardFunctions();
    evaluator->addStandardOperators();

    const auto result = evaluator->evaluate("IF(A < B, 1, 2)");

    ASSERT_TRUE(result);
    ASSERT_EQ("1", *result);
}

TEST_F(EvaluatorTests, positiveTest_NestedFunction_EvaluationResult)
{
	makeRealEvaluator();

    evaluator->addStandardFunctions();
    evaluator->addStandardOperators();

    const auto result = evaluator->evaluate("IF(A > B, 1, REPLACE(ABC, A, E))");

    ASSERT_TRUE(result);
    ASSERT_EQ("EBC", *result);
}

TEST_F(EvaluatorTests, positiveTest_TwoFunctionsOneOperator_EvaluationResult)
{
	makeRealEvaluator();

    evaluator->addStandardFunctions();
    evaluator->addStandardOperators();

    const auto result = evaluator->evaluate("IF(A < B, 1, 2) + IF(A > B, 3, 4)");

    ASSERT_TRUE(result);
    ASSERT_EQ("14", *result);
}

TEST_F(EvaluatorTests, positiveTest_RedundantBrackets_EvaluationResult)
{
	makeRealEvaluator();

    evaluator->addStandardFunctions();
    evaluator->addStandardOperators();

    const auto result = evaluator->evaluate("(((A + B)))");

    ASSERT_TRUE(result);
    ASSERT_EQ("AB", *result);
}

TEST_F(EvaluatorTests, positiveTest_CompareWithNull_EvaluationResult)
{
	makeRealEvaluator();

    evaluator->addStandardFunctions();
    evaluator->addStandardOperators();

    const auto result = evaluator->evaluate("IF(A == NULL, Wrong, Correct)");

    ASSERT_TRUE(result);
    ASSERT_EQ("Correct", *result);
}

TEST_F(EvaluatorTests, positiveTest_NullAsResult_EvaluationResult)
{
	makeRealEvaluator();

    evaluator->addStandardFunctions();
    evaluator->addStandardOperators();

    const auto result = evaluator->evaluate("IF(A == B, Wrong, NULL)");

    ASSERT_FALSE(result);
}

TEST_F(EvaluatorTests, negativeTest_AddEmptyFunctionPointer)
{
    makeRealEvaluator();

    ASSERT_THROW({
        try
        {
            evaluator->addFunction(std::unique_ptr<s2e2::Function>());
        }
        catch (const std::invalid_argument& e)
        {
            ASSERT_STREQ("Evaluator: pointer to adding function is empty", e.what());
            throw;
        }
    }, std::invalid_argument);
}

TEST_F(EvaluatorTests, negativeTest_AddEmptyOperatorPointer)
{
    makeRealEvaluator();

    ASSERT_THROW({
        try
        {
            evaluator->addOperator(std::unique_ptr<s2e2::Operator>());
        }
        catch (const std::invalid_argument& e)
        {
            ASSERT_STREQ("Evaluator: pointer to adding operator is empty", e.what());
            throw;
        }
    }, std::invalid_argument);
}

TEST_F(EvaluatorTests, negativeTest_TwoFunctionsWithTheSameName)
{
    makeRealEvaluator();
    evaluator->addFunction(dummyFunction());

    ASSERT_THROW({
        try
        {
            evaluator->addFunction(dummyFunction());
        }
        catch (const s2e2::Error& e)
        {
            ASSERT_EQ("Evaluator: function " + DUMMY_FUNCTION_NAME + " is already added", e.what());
            throw;
        }
    }, s2e2::Error);
}

TEST_F(EvaluatorTests, negativeTest_TwoOperatorsWithTheSameName)
{
    makeRealEvaluator();
    evaluator->addOperator(dummyOperator());

    ASSERT_THROW({
        try
        {
            evaluator->addOperator(dummyOperator());
        }
        catch (const s2e2::Error& e)
        {
            ASSERT_EQ("Evaluator: operator " + DUMMY_OPERATOR_NAME + " is already added", e.what());
            throw;
        }
    }, s2e2::Error);
}

TEST_F(EvaluatorTests, negativeTest_UnpairedBracket)
{
    makeRealEvaluator();
    evaluator->addStandardFunctions();
    evaluator->addStandardOperators();

    ASSERT_THROW({
        try
        {
            evaluator->evaluate("A + (B + C");
        }
        catch (const s2e2::Error& e)
        {
            ASSERT_STREQ("Converter: unpaired bracket", e.what());
            throw;
        }
    }, s2e2::Error);
}

TEST_F(EvaluatorTests, negativeTest_UnexpectedTokenType)
{
    makeMockedEvaluator();

    const auto wrongTokens = std::list<s2e2::Token>{s2e2::Token{s2e2::TokenType::ATOM, "A"},
                                                    s2e2::Token{s2e2::TokenType::ATOM, "B"},
                                                    s2e2::Token{s2e2::TokenType::LEFT_BRACKET, "("}};

    ON_CALL(*tokenizerMock, tokenize(_)).WillByDefault(Return(wrongTokens));
    ON_CALL(*converterMock, convert(_)).WillByDefault(Return(wrongTokens));

    ASSERT_THROW({
        try
        {
            evaluator->evaluate("A + B");
        }
        catch (const s2e2::Error& e)
        {
            ASSERT_EQ("Evaluator: unexpected token type " + std::to_string(static_cast<int>(s2e2::TokenType::LEFT_BRACKET)), e.what());
            throw;
        }
    }, s2e2::Error);
}

TEST_F(EvaluatorTests, negativeTest_UnsupportedOperator)
{
    makeMockedEvaluator();
    evaluator->addStandardFunctions();
    evaluator->addStandardOperators();

    const auto wrongTokens = std::list<s2e2::Token>{s2e2::Token{s2e2::TokenType::ATOM, "A"},
                                                    s2e2::Token{s2e2::TokenType::ATOM, "B"},
                                                    s2e2::Token{s2e2::TokenType::OPERATOR, "<>"}};

    ON_CALL(*tokenizerMock, tokenize(_)).WillByDefault(Return(wrongTokens));
    ON_CALL(*converterMock, convert(_)).WillByDefault(Return(wrongTokens));

    ASSERT_THROW({
        try
        {
            evaluator->evaluate("A <> B");
        }
        catch (const s2e2::Error& e)
        {
            ASSERT_STREQ("Evaluator: unsupported operator <>", e.what());
            throw;
        }
    }, s2e2::Error);
}

TEST_F(EvaluatorTests, negativeTest_UnsupportedFunction)
{
    makeMockedEvaluator();
    evaluator->addStandardFunctions();
    evaluator->addStandardOperators();

    const auto wrongTokens = std::list<s2e2::Token>{s2e2::Token{s2e2::TokenType::ATOM, "A"},
                                                    s2e2::Token{s2e2::TokenType::ATOM, "B"},
                                                    s2e2::Token{s2e2::TokenType::FUNCTION, "FUNC"}};

    ON_CALL(*tokenizerMock, tokenize(_)).WillByDefault(Return(wrongTokens));
    ON_CALL(*converterMock, convert(_)).WillByDefault(Return(wrongTokens));

    ASSERT_THROW({
        try
        {
            evaluator->evaluate("A + B");
        }
        catch (const s2e2::Error& e)
        {
            ASSERT_STREQ("Evaluator: unsupported function FUNC", e.what());
            throw;
        }
    }, s2e2::Error);
}

TEST_F(EvaluatorTests, negativeTest_FewArguments)
{
    makeRealEvaluator();
    evaluator->addStandardFunctions();
    evaluator->addStandardOperators();

    ASSERT_THROW({
        try
        {
            evaluator->evaluate("A + ");
        }
        catch (const s2e2::Error& e)
        {
            ASSERT_STREQ("Not enough arguments for operator +", e.what());
            throw;
        }
    }, s2e2::Error);
}

TEST_F(EvaluatorTests, negativeTest_FewOperators)
{
    makeRealEvaluator();
    evaluator->addStandardFunctions();
    evaluator->addStandardOperators();

    ASSERT_THROW({
        try
        {
            evaluator->evaluate("A + B C");
        }
        catch (const s2e2::Error& e)
        {
            ASSERT_STREQ("Evaluator: invalid expression", e.what());
            throw;
        }
    }, s2e2::Error);
}
