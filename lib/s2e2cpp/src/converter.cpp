#include "converter.hpp"

#include <s2e2/error.hpp>


namespace // anonymous
{
    /**
     * @brief Move one token from stack of tokens into queue of tokens.
     * @param[in, out] stack - Stack of tokens.
     * @param[out] queue - Queue of tokens.
     */
    void moveTokenFromStackToQueue(std::stack<s2e2::Token>& stack, std::list<s2e2::Token>& queue)
    {
        auto& token = stack.top();
        queue.push_back(std::move(token));
        stack.pop();
    }

} // namespace anonymous 


void s2e2::Converter::addOperator(const std::string& name, const uint_fast16_t priority)
{
    if (operators_.count(name) != 0)
    {
        throw Error("Converter: operator " + name + " is already added");
    }
    operators_[name] = priority;
}

std::list<s2e2::Token> s2e2::Converter::convert(const std::list<Token>& infixExpression) const
{
    std::list<Token>{}.swap(outputQueue_);
    std::stack<Token>{}.swap(operatorStack_);

    processTokens(infixExpression);
    processOperators();

    return std::move(outputQueue_);
}

void s2e2::Converter::processTokens(const std::list<Token>& expression) const
{
    for (const auto& token : expression)
    {
        switch (token.type)
        {
        case TokenType::ATOM:
            processAtom(token);
            break;

        case TokenType::COMMA:
            processComma();
            break;

        case TokenType::FUNCTION:
            processFunction(token);
            break;

        case TokenType::OPERATOR:
            processOperator(token);
            break;

        case TokenType::LEFT_BRACKET:
            processLeftBracket(token);
            break;

        case TokenType::RIGHT_BRACKET:
            processRightBracket();
            break;
        
        default:
            throw Error("Converter: unexpected token type " + std::to_string(static_cast<int>(token.type)));
        }
    }
}

void s2e2::Converter::processAtom(const Token& token) const
{
    outputQueue_.push_back(token);
}

void s2e2::Converter::processComma() const
{
    while (!operatorStack_.empty() &&
           operatorStack_.top().type != TokenType::LEFT_BRACKET)
    {
        moveTokenFromStackToQueue(operatorStack_, outputQueue_);
    }
}

void s2e2::Converter::processFunction(const Token& token) const
{
    operatorStack_.push(token);
}

void s2e2::Converter::processOperator(const Token& token) const
{
    const auto it = operators_.find(token.value);
    if (it == operators_.end())
    {
        throw Error("Converter: unknown operator " + token.value);
    }
    const auto& priority = it->second;

    while (!operatorStack_.empty() &&
           operatorStack_.top().type == TokenType::OPERATOR &&
           priority <= operators_.at(operatorStack_.top().value))
    {
        moveTokenFromStackToQueue(operatorStack_, outputQueue_);
    }

    operatorStack_.push(token);
}

void s2e2::Converter::processLeftBracket(const Token& token) const
{
    operatorStack_.push(token);
}

void s2e2::Converter::processRightBracket() const
{
    while (!operatorStack_.empty() &&
           operatorStack_.top().type != TokenType::LEFT_BRACKET)
    {
        moveTokenFromStackToQueue(operatorStack_, outputQueue_);
    }

    if (operatorStack_.empty())
    {
        throw Error("Converter: unpaired bracket");
    }
    operatorStack_.pop();

    if (!operatorStack_.empty() &&
        operatorStack_.top().type == TokenType::FUNCTION)
    {
        moveTokenFromStackToQueue(operatorStack_, outputQueue_);
    }
}

void s2e2::Converter::processOperators() const
{
    while (!operatorStack_.empty())
    {
        if (operatorStack_.top().type == TokenType::LEFT_BRACKET)
        {
            throw Error("Converter: unpaired bracket");
        }
        moveTokenFromStackToQueue(operatorStack_, outputQueue_);
    }
}
