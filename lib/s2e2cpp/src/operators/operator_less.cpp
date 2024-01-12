#include "priorities.hpp"

#include <s2e2/operators/operator_less.hpp>

#include <typeinfo>


s2e2::OperatorLess::OperatorLess()
    : Operator("<", Priorities::OPERATOR_LESS, 2)
{
}

bool s2e2::OperatorLess::checkArguments() const
{
    static const auto& stringType = typeid(std::string);

    return (arguments_[0].has_value() && arguments_[0].type() == stringType) &&
           (arguments_[1].has_value() && arguments_[1].type() == stringType);
}

std::any s2e2::OperatorLess::result() const
{
    return {*std::any_cast<std::string>(&arguments_[0]) < *std::any_cast<std::string>(&arguments_[1])};
}
