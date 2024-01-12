#include <s2e2/functions/function_now.hpp>

#include <ctime>


s2e2::FunctionNow::FunctionNow()
    : Function("NOW", 0)
{
}

bool s2e2::FunctionNow::checkArguments() const
{
    return true;
}

std::any s2e2::FunctionNow::result() const
{
    const auto now = std::time(nullptr);
    const auto* tm = std::gmtime(&now);

    return std::any{*tm};
}
