#include "priorities.hpp"

#include <s2e2/operators/operator_or.hpp>

#include <typeinfo>


s2e2::OperatorOr::OperatorOr()
    : Operator("||", Priorities::OPERATOR_OR, 2)
{
}

bool s2e2::OperatorOr::checkArguments() const
{
    static const auto& boolType = typeid(bool);

    return arguments_[0].has_value() && arguments_[0].type() == boolType &&
           arguments_[1].has_value() && arguments_[1].type() == boolType;
}

std::any s2e2::OperatorOr::result() const
{
    return {std::any_cast<bool>(arguments_[0]) || std::any_cast<bool>(arguments_[1])};
}
