#include "../utils.hpp"

#include <s2e2/functions/function_format_date.hpp>

#include <ctime>
#include <string>
#include <typeinfo>


s2e2::FunctionFormatDate::FunctionFormatDate()
    : Function("FORMAT_DATE", 2)
{
}

bool s2e2::FunctionFormatDate::checkArguments() const
{
    static const auto& stringType = typeid(std::string);
    static const auto& tmType = typeid(std::tm);

    // check 1st argument
    if (!arguments_[0].has_value() ||
        arguments_[0].type() != tmType)
    {
        return false;
    }

    // check 2nd argument
    if (!arguments_[1].has_value() ||
        arguments_[1].type() != stringType)
    {
        return false;
    }

    return true;
}

std::any s2e2::FunctionFormatDate::result() const
{
    const auto* datetime = std::any_cast<std::tm>(&arguments_[0]);
    const auto* format = std::any_cast<std::string>(&arguments_[1]);

    thread_local char buffer[256];

    if (std::strftime(buffer, sizeof(buffer), format->c_str(), datetime) == 0)
    {
        return std::any{};
    }
    return std::any{std::string(buffer)};
}
