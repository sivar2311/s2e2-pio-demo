#include "priorities.hpp"

#include <s2e2/operators/operator_greater_or_equal.hpp>

#include <typeinfo>


s2e2::OperatorGreaterOrEqual::OperatorGreaterOrEqual()
    : Operator(">=", Priorities::OPERATOR_GREATER_OR_EQUAL, 2)
{
}

bool s2e2::OperatorGreaterOrEqual::checkArguments() const
{
    static const auto& stringType = typeid(std::string);

    return (!arguments_[0].has_value() && !arguments_[1].has_value()) ||
           ((arguments_[0].has_value() && arguments_[0].type() == stringType) &&
            (arguments_[1].has_value() && arguments_[1].type() == stringType));
}

std::any s2e2::OperatorGreaterOrEqual::result() const
{
    if (!arguments_[0].has_value())
    {
        return {!arguments_[1].has_value()};
    }
    if (!arguments_[1].has_value())
    {
        return {!arguments_[0].has_value()};
    }
    
    return {*std::any_cast<std::string>(&arguments_[0]) >= *std::any_cast<std::string>(&arguments_[1])};
}
