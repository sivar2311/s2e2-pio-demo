#include "converter.hpp"
#include "evaluator_impl.hpp"
#include "token_type.hpp"
#include "token.hpp"
#include "tokenizer.hpp"

#include <s2e2/error.hpp>

#include <s2e2/functions/function_add_days.hpp>
#include <s2e2/functions/function_format_date.hpp>
#include <s2e2/functions/function_if.hpp>
#include <s2e2/functions/function_now.hpp>
#include <s2e2/functions/function_replace.hpp>

#include <s2e2/operators/operator_and.hpp>
#include <s2e2/operators/operator_equal.hpp>
#include <s2e2/operators/operator_greater_or_equal.hpp>
#include <s2e2/operators/operator_greater.hpp>
#include <s2e2/operators/operator_less_or_equal.hpp>
#include <s2e2/operators/operator_less.hpp>
#include <s2e2/operators/operator_not_equal.hpp>
#include <s2e2/operators/operator_not.hpp>
#include <s2e2/operators/operator_or.hpp>
#include <s2e2/operators/operator_plus.hpp>

#include <algorithm>
#include <stdexcept>


namespace
{
    /// @brief Null value in an input expression.
    const std::string NULL_VALUE = "NULL";

    /// @brief Expected stack size after processing all tokens.
    const size_t FINAL_STACK_SIZE = 1;
}

s2e2::EvaluatorImpl::EvaluatorImpl()
    : EvaluatorImpl(std::make_unique<Converter>(), std::make_unique<Tokenizer>())
{
}

s2e2::EvaluatorImpl::EvaluatorImpl(std::unique_ptr<IConverter>&& converter, std::unique_ptr<ITokenizer>&& tokenizer)
    : converter_(std::move(converter))
    , tokenizer_(std::move(tokenizer))
{
    if (!converter_)
    {
        throw std::invalid_argument("Evaluator: pointer to converter is empty");
    }
    if (!tokenizer_)
    {
        throw std::invalid_argument("Evaluator: pointer to tokenizer is empty");
    }
}

void s2e2::EvaluatorImpl::addFunction(std::unique_ptr<Function>&& fn)
{
    if (!fn)
    {
        throw std::invalid_argument("Evaluator: pointer to adding function is empty");
    }
    checkUniqueness(fn->name);

    tokenizer_->addFunction(fn->name);
    functions_.emplace(fn->name, std::move(fn));
}

void s2e2::EvaluatorImpl::addOperator(std::unique_ptr<Operator>&& op)
{
    if (!op)
    {
        throw std::invalid_argument("Evaluator: pointer to adding operator is empty");
    }
    checkUniqueness(op->name);

    converter_->addOperator(op->name, op->priority);
    tokenizer_->addOperator(op->name);
    operators_.emplace(op->name, std::move(op));
}

void s2e2::EvaluatorImpl::addStandardFunctions()
{
    addFunction(std::make_unique<FunctionAddDays>());
    addFunction(std::make_unique<FunctionFormatDate>());
    addFunction(std::make_unique<FunctionIf>());
    addFunction(std::make_unique<FunctionNow>());
    addFunction(std::make_unique<FunctionReplace>());
}

void s2e2::EvaluatorImpl::addStandardOperators()
{
    addOperator(std::make_unique<OperatorAnd>());
    addOperator(std::make_unique<OperatorEqual>());
    addOperator(std::make_unique<OperatorGreaterOrEqual>());
    addOperator(std::make_unique<OperatorGreater>());
    addOperator(std::make_unique<OperatorLessOrEqual>());
    addOperator(std::make_unique<OperatorLess>());
    addOperator(std::make_unique<OperatorNotEqual>());
    addOperator(std::make_unique<OperatorNot>());
    addOperator(std::make_unique<OperatorOr>());
    addOperator(std::make_unique<OperatorPlus>());
}

std::unordered_set<const s2e2::Function*> s2e2::EvaluatorImpl::getFunctions() const
{
    std::unordered_set<const Function*> result(functions_.bucket_count());
    for (const auto& pair : functions_)
    {
        result.insert(pair.second.get());
    }
    return result;
}

std::unordered_set<const s2e2::Operator*> s2e2::EvaluatorImpl::getOperators() const
{
    std::unordered_set<const Operator*> result(operators_.bucket_count());
    for (const auto& pair : operators_)
    {
        result.insert(pair.second.get());
    }
    return result;
}

std::optional<std::string> s2e2::EvaluatorImpl::evaluate(const std::string& expression) const
{
    const auto infixExpression = tokenizer_->tokenize(expression);

    // a bit of syntax sugar: if expression contains only atoms
    // consider it as just a string literal
    if (std::all_of(infixExpression.begin(), infixExpression.end(), [](const auto& e){ return e.type == TokenType::ATOM; }))
    {
        return {expression};
    }

    const auto postfixExpression = converter_->convert(infixExpression);
    return evaluateExpression(postfixExpression);
}

void s2e2::EvaluatorImpl::checkUniqueness(const std::string& entityName) const
{
    if (functions_.count(entityName) != 0)
    {
        throw Error("Evaluator: function " + entityName + " is already added");
    }
    if (operators_.count(entityName) != 0)
    {
        throw Error("Evaluator: operator " + entityName + " is already added");
    }
}

std::optional<std::string> s2e2::EvaluatorImpl::evaluateExpression(const std::list<Token>& postfixExpression) const
{
    std::stack<std::any>{}.swap(stack_);

    for (const auto& token : postfixExpression)
    {
        switch (token.type)
        {
            case TokenType::ATOM:
                processAtom(token);
                break;

            case TokenType::OPERATOR:
                processOperator(token);
                break;

            case TokenType::FUNCTION:
                processFunction(token);
                break;

            default:
                throw Error("Evaluator: unexpected token type " + std::to_string(static_cast<int>(token.type)));
        }
    }

    auto result = getResultValueFromStack();
    std::stack<std::any>{}.swap(stack_);
    return result;
}

void s2e2::EvaluatorImpl::processAtom(const Token& token) const
{
    auto value = (token.value == NULL_VALUE) ? std::any{} : std::any{token.value};
    stack_.push(std::move(value));
}

void s2e2::EvaluatorImpl::processOperator(const Token& token) const
{
    try
    {
        const auto& op = operators_.at(token.value);
        op->invoke(stack_);
    }
    catch (const std::out_of_range&)
    {
        throw Error("Evaluator: unsupported operator " + token.value);
    }
}

void s2e2::EvaluatorImpl::processFunction(const Token& token) const
{
    try
    {
        const auto& fn = functions_.at(token.value);
        fn->invoke(stack_);
    }
    catch (const std::out_of_range&)
    {
        throw Error("Evaluator: unsupported function " + token.value);
    }
}

std::optional<std::string> s2e2::EvaluatorImpl::getResultValueFromStack() const
{
    if (stack_.size() != FINAL_STACK_SIZE)
    {
        throw Error("Evaluator: invalid expression");
    }

    const auto& result = stack_.top();

    if (!result.has_value())
    {
        return {};
    }

    static const auto& stringType = typeid(std::string);
    if (result.type() != stringType)
    {
        throw Error("Evaluator: expression value is not a string");
    }

    return {std::any_cast<std::string>(std::move(result))};
}
