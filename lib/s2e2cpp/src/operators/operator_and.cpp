#include "priorities.hpp"

#include <s2e2/operators/operator_and.hpp>

#include <typeinfo>


s2e2::OperatorAnd::OperatorAnd()
    : Operator("&&", Priorities::OPERATOR_AND, 2)
{
}

bool s2e2::OperatorAnd::checkArguments() const
{
    static const auto& boolType = typeid(bool);

    return arguments_[0].has_value() && arguments_[0].type() == boolType &&
           arguments_[1].has_value() && arguments_[1].type() == boolType;
}

std::any s2e2::OperatorAnd::result() const
{
    return {std::any_cast<bool>(arguments_[0]) && std::any_cast<bool>(arguments_[1])};
}
