#include "priorities.hpp"

#include <s2e2/operators/operator_plus.hpp>

#include <typeinfo>


s2e2::OperatorPlus::OperatorPlus()
    : Operator("+", Priorities::OPERATOR_PLUS, 2)
{
}

bool s2e2::OperatorPlus::checkArguments() const
{
    static const auto& stringType = typeid(std::string);

    return (!arguments_[0].has_value() || arguments_[0].type() == stringType) &&
           (!arguments_[1].has_value() || arguments_[1].type() == stringType);
}

std::any s2e2::OperatorPlus::result() const
{
    if (!arguments_[0].has_value() && !arguments_[1].has_value())
    {
        return {};
    }

    std::string result;
    if (arguments_[0].has_value())
    {
        result += *std::any_cast<std::string>(&arguments_[0]);
    }
    if (arguments_[1].has_value())
    {
        result += *std::any_cast<std::string>(&arguments_[1]);
    }
    return result;
}
