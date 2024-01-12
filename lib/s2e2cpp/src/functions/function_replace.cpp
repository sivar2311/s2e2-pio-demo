#include <s2e2/functions/function_replace.hpp>

#include <regex>
#include <string>
#include <typeinfo>


s2e2::FunctionReplace::FunctionReplace()
    : Function("REPLACE", 3)
{
}

bool s2e2::FunctionReplace::checkArguments() const
{
    static const auto& stringType = typeid(std::string);

    // check 1st argument
    if (arguments_[0].has_value() &&
        arguments_[0].type() != stringType)
    {
        return false;
    }

    // check 2nd argument
    if (!arguments_[1].has_value() ||
        arguments_[1].type() != stringType ||
        std::any_cast<std::string>(&arguments_[1])->empty())
    {
        return false;
    }

    // check 3rd argument
    if (!arguments_[2].has_value() ||
        arguments_[2].type() != stringType)
    {
        return false;
    }

    return true;
}

std::any s2e2::FunctionReplace::result() const
{
    if (!arguments_[0].has_value())
    {
        return std::any{};
    }

    const auto* source = std::any_cast<std::string>(&arguments_[0]);
    const auto regex = std::regex(*std::any_cast<std::string>(&arguments_[1]));
    const auto* replacement = std::any_cast<std::string>(&arguments_[2]);

    auto result = std::regex_replace(*source, regex, *replacement);
    return std::any{std::move(result)};
}
