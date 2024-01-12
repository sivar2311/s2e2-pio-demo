#include <s2e2/functions/function_if.hpp>

#include <typeinfo>


s2e2::FunctionIf::FunctionIf()
    : Function("IF", 3)
{
}

bool s2e2::FunctionIf::checkArguments() const
{
    static const auto& boolType = typeid(bool);

    return arguments_[0].has_value() &&
           arguments_[0].type() == boolType;
}

std::any s2e2::FunctionIf::result() const
{
    return std::move(std::any_cast<bool>(arguments_[0]) ? arguments_[1] : arguments_[2]);
}
