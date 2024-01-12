#include <s2e2/error.hpp>
#include <s2e2/function.hpp>


void s2e2::Function::invoke(std::stack<std::any>& stack) const
{
    if (stack.size() < arguments_.size())
    {
        throw Error("Not enough arguments for function " + name);
    }

    for (int i = static_cast<int>(arguments_.size() - 1); i >= 0; --i)
    {
        arguments_[i] = std::move(stack.top());
        stack.pop();
    }

    if (!checkArguments())
    {
        throw Error("Invalid arguments for function " + name);
    }
    stack.push(result());
}

s2e2::Function::Function(std::string functionName, const uint_fast16_t numberOfArguments)
    : name(std::move(functionName))
    , arguments_(numberOfArguments)
{
}
