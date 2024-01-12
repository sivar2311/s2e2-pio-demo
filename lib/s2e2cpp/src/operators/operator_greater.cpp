#include "priorities.hpp"

#include <s2e2/operators/operator_greater.hpp>

#include <typeinfo>


s2e2::OperatorGreater::OperatorGreater()
    : Operator(">", Priorities::OPERATOR_GREATER, 2)
{
}

bool s2e2::OperatorGreater::checkArguments() const
{
    static const auto& stringType = typeid(std::string);

    return (arguments_[0].has_value() && arguments_[0].type() == stringType) &&
           (arguments_[1].has_value() && arguments_[1].type() == stringType);
}

std::any s2e2::OperatorGreater::result() const
{
    return {*std::any_cast<std::string>(&arguments_[0]) > *std::any_cast<std::string>(&arguments_[1])};
}
