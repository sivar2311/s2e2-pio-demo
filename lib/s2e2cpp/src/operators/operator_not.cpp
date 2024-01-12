#include "priorities.hpp"

#include <s2e2/operators/operator_not.hpp>

#include <typeinfo>


s2e2::OperatorNot::OperatorNot()
    : Operator("!", Priorities::OPERATOR_NOT, 1)
{
}

bool s2e2::OperatorNot::checkArguments() const
{
    static const auto& boolType = typeid(bool);

    return arguments_[0].has_value() && arguments_[0].type() == boolType;
}

std::any s2e2::OperatorNot::result() const
{
    return {!std::any_cast<bool>(arguments_[0])};
}
