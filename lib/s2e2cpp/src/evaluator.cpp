#include "evaluator_impl.hpp"

#include <s2e2/evaluator.hpp>

/**
 * @class Evaluator::Impl
 * @brief Proxy between public Evaluator and real evaluator implementation.
 */
class s2e2::Evaluator::Impl
{
public:
    /// @brief Real evaluator.
    EvaluatorImpl evaluator;
};

s2e2::Evaluator::Evaluator()
    : pimpl_{std::make_unique<Impl>()}
{
}

s2e2::Evaluator::~Evaluator() = default;

void s2e2::Evaluator::addFunction(std::unique_ptr<Function>&& fn)
{
    pimpl_->evaluator.addFunction(std::move(fn));
}

void s2e2::Evaluator::addOperator(std::unique_ptr<Operator>&& op)
{
    pimpl_->evaluator.addOperator(std::move(op));
}

void s2e2::Evaluator::addStandardFunctions()
{
    pimpl_->evaluator.addStandardFunctions();
}

void s2e2::Evaluator::addStandardOperators()
{
    pimpl_->evaluator.addStandardOperators();
}

std::unordered_set<const s2e2::Function*> s2e2::Evaluator::getFunctions() const
{
    return pimpl_->evaluator.getFunctions();
}

std::unordered_set<const s2e2::Operator*> s2e2::Evaluator::getOperators() const
{
    return pimpl_->evaluator.getOperators();
}

std::optional<std::string> s2e2::Evaluator::evaluate(const std::string& expression) const
{
    return pimpl_->evaluator.evaluate(expression);
}
