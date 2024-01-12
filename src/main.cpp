#include <s2e2/evaluator.hpp>

s2e2::Evaluator evaluator;

int main() {
    evaluator.addStandardFunctions();
    evaluator.addStandardOperators();

    const std::string expression = "A + B";
    const auto        result     = evaluator.evaluate(expression);

    return 0;
}