#pragma once

#include <s2e2/function.hpp>
#include <s2e2/operator.hpp>

#include <memory>
#include <optional>
#include <string>
#include <unordered_set>


namespace s2e2
{
    /**
     * @class Evaluator
     * @brief Class evaluates string values of expressions.
     */
    class Evaluator final
    {
    public:
        /**
         * @brief Default constructor.
         */
        Evaluator();

        /**
         * @brief Destructor.
         */
        ~Evaluator();

        /**
         * @brief Add function to set of supported functions.
         * @param[in] fn - Pointer to new supported function.
         * @throws std::invalid_argument if pointer is empty.
         * @throws Error if function or operator with the same name is already added.
         */
        void addFunction(std::unique_ptr<Function>&& fn);

        /**
         * @brief Add operator to set of supported operators.
         * @param[in] op - Pointer to new supported operator.
         * @throws std::invalid_argument if pointer is empty.
         * @throws Error if function or operator with the same name is already added.
         */
        void addOperator(std::unique_ptr<Operator>&& op);

        /**
         * @brief Add standard functions to set of supported functions.
         * @throws Error if there is a collision between functions names.
         */
        void addStandardFunctions();

        /**
         * @brief Add standard operators to set of supported operators.
         * @throws Error if there is a collision between operators names.
         */
        void addStandardOperators();

        /**
         * @brief Get collection of pointers to all supported functions.
         * @returns Collection of pointers to functions.
         */
        std::unordered_set<const Function*> getFunctions() const;

        /**
         * @brief Get collection of pointers to all supported operators.
         * @returns Collection of pointers to operators.
         */
        std::unordered_set<const Operator*> getOperators() const;

        /**
         * @brief Evaluate the expression.
         * @param[in] expression - Input expression.
         * @returns Value of expression as a string or empty value if the result is NULL.
         * @throws Error in case of an invalid expression.
         */
        std::optional<std::string> evaluate(const std::string& expression) const;

    private:
        /// @brief Nested proxy of real evaluator implementation.
        class Impl;

        /// @brief Pointer to evaluator proxy;
        std::unique_ptr<Impl> pimpl_;
    };

} // namespace s2e2
