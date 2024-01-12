#include "../utils.hpp"

#include <s2e2/functions/function_add_days.hpp>

#include <ctime>
#include <string>
#include <typeinfo>


s2e2::FunctionAddDays::FunctionAddDays()
    : Function("ADD_DAYS", 2)
{
}

bool s2e2::FunctionAddDays::checkArguments() const
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

    try
    {
        std::stoi(*std::any_cast<std::string>(&arguments_[1]));
    }
    catch (const std::exception&)
    {
        return false;
    }

    return true;
}

std::any s2e2::FunctionAddDays::result() const
{
    auto datetime = std::any_cast<std::tm>(arguments_[0]);
    const auto days = std::stoi(*std::any_cast<std::string>(&arguments_[1]));

    datetime.tm_mday += days;
    const auto newTs = utcTs(&datetime);
    datetime = *std::gmtime(&newTs);

    return std::any{std::move(datetime)};
}
