#include <s2e2/error.hpp>
#include <s2e2/operator.hpp>


void s2e2::Operator::invoke(std::stack<std::any>& stack) const
{
    if (stack.size() < arguments_.size())
    {
        throw Error("Not enough arguments for operator " + name);
    }

    for (int i = static_cast<int>(arguments_.size() - 1); i >= 0; --i)
    {
        arguments_[i] = std::move(stack.top());
        stack.pop();
    }

    if (!checkArguments())
    {
        throw Error("Invalid arguments for operator " + name);
    }
    stack.push(result());
}

s2e2::Operator::Operator(std::string operatorName,
                         const uint_fast16_t operatorPriority,
                         const uint_fast16_t numberOfArguments)
    : name(std::move(operatorName))
    , priority(operatorPriority)
    , arguments_(numberOfArguments)
{
}
