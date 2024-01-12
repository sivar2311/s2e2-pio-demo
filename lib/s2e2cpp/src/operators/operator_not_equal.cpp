#include "priorities.hpp"

#include <s2e2/operators/operator_not_equal.hpp>

#include <typeinfo>


s2e2::OperatorNotEqual::OperatorNotEqual()
    : Operator("!=", Priorities::OPERATOR_NOT_EQUAL, 2)
{
}

bool s2e2::OperatorNotEqual::checkArguments() const
{
    static const auto& stringType = typeid(std::string);

    return (!arguments_[0].has_value() || arguments_[0].type() == stringType) &&
           (!arguments_[1].has_value() || arguments_[1].type() == stringType);
}

std::any s2e2::OperatorNotEqual::result() const
{
    if (!arguments_[0].has_value())
    {
        return {arguments_[1].has_value()};
    }
    if (!arguments_[1].has_value())
    {
        return {arguments_[0].has_value()};
    }

    return {*std::any_cast<std::string>(&arguments_[0]) != *std::any_cast<std::string>(&arguments_[1])};
}
