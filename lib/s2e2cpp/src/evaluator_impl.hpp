#pragma once

#include "interface_converter.hpp"
#include "interface_tokenizer.hpp"
#include "token.hpp"

#include <s2e2/function.hpp>
#include <s2e2/operator.hpp>

#include <any>
#include <list>
#include <memory>
#include <optional>
#include <stack>
#include <unordered_map>
#include <unordered_set>


namespace s2e2
{
    /**
     * @class EvaluatorImpl
     * @brief Real implementation of Evaluator class.
     */
    class EvaluatorImpl final
    {
    public:
        /**
         * @brief Default constructor.
         */
        EvaluatorImpl();

        /**
         * @brief Constructor for unit testing.
         * @param[in] converter - External IConverter object.
         * @param[in] tokenizer - External ITokenizer object.
         * @throws throw std::invalid_argument if any of input params in empty.
         */
        EvaluatorImpl(std::unique_ptr<IConverter>&& converter, std::unique_ptr<ITokenizer>&& tokenizer);

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
        /**
         * @brief Check is function's or operator's name is unique.
         * @param[in] entityName - Function's or operator's name.
         * @throws Error if the name is not unique.
         */
        void checkUniqueness(const std::string& entityName) const;

        /**
         * @brief Get value of the postfix sequence of tokens.
         * @param[in] postfixExpression - Sequence of tokens.
         * @return String value or empty value.
         * @throws Error in case of an invalid expression.
         */
        std::optional<std::string> evaluateExpression(const std::list<Token>& postfixExpression) const;

        /**
         * @brief Process ATOM token.
         * @param[in] token - ATOM token.
         */
        void processAtom(const Token& token) const;

        /**
         * @brief Process OPERATOR token.
         * @param[in] token - OPERATOR token.
         * @throws Error in case of unsupported operator.
         */
        void processOperator(const Token& token) const;

        /**
         * @brief Process FUNCTION token.
         * @param[in] token - FUNCTION token.
         * @throws Error in case of unsupported function.
         */
        void processFunction(const Token& token) const;

        /**
         * @brief Get result value from the stack of intermediate values.
         * @return String value or empty value.
         * @throws Error in case of an invalid expression.
         */
        std::optional<std::string> getResultValueFromStack() const;

    private:
        /// @brief Converter of infix token sequence into postfix one.
        const std::unique_ptr<IConverter> converter_;

        /// @brief Tokenizer of expression onto list of tokens.
        const std::unique_ptr<ITokenizer> tokenizer_;

        /// @brief Set of all supported functions.
        std::unordered_map<std::string, std::unique_ptr<Function>> functions_;

        /// @brief Set of all supported operators.
        std::unordered_map<std::string, std::unique_ptr<Operator>> operators_;

        /// @brief Stack of intermediate values.
        mutable std::stack<std::any> stack_;
    };

} // namespace s2e2
