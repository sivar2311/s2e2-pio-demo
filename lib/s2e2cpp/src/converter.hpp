#pragma once

#include "interface_converter.hpp"
#include "token_type.hpp"
#include "token.hpp"

#include <cstdint>
#include <list>
#include <stack>
#include <string>
#include <unordered_map>


namespace s2e2
{
    /**
     * @class Converter
     * @brief Class converts infix token sequence into postfix one.
     * @details Convertion is done by Shunting Yard algorithm.
     */
    class Converter final : public IConverter
    {
    public:
        /**
         * @brief Add operator expected within expression.
         * @param[in] name - Operator's name.
         * @param[in] priority - Operator's priority.
         * @throws Error if operator's name is not unique.
         */
        void addOperator(const std::string& name, const uint_fast16_t priority) override;

        /**
         * @brief Convert infix token sequence into postfix one.
         * @param[in] infixExpression - Input sequence of tokens.
         * @returns Postfix sequence of tokens.
         * @throws Error in case of an error.
         */
        std::list<Token> convert(const std::list<Token>& infixExpression) const override;

    private:
        /**
         * @brief Process all tokens in the input sequence.
         * @param[in] expression - Tokens sequence.
         * @throws Error in case of an error.
         */
        void processTokens(const std::list<Token>& expression) const;

        /**
         * @brief Process ATOM token.
         * @param[in] token - Input token.
         */
        void processAtom(const Token& token) const;

        /**
         * @brief Process COMMA token.
         */
        void processComma() const;

        /**
         * @brief Process FUNCTION token.
         * @param[in] token - Input token.
         */
        void processFunction(const Token& token) const;

        /**
         * @brief Process OPERATOR token.
         * @param[in] token - Input token.
         * @throws Error in case of an unknown operator.
         */
        void processOperator(const Token& token) const;

        /**
         * @brief Process LEFT BRACKET token.
         * @param[in] token - Input token.
         */
        void processLeftBracket(const Token& token) const;

        /**
         * @brief Process RIGHT BRACKET token.
         * @throws Error in case of an error.
         */
        void processRightBracket() const;

        /**
         * @brief Process all operators left in the operator stack.
         * @throws Error in case of an error.
         */
        void processOperators() const;

    private:
        /// @brief Output queue of all tokens.
        mutable std::list<Token> outputQueue_;

        /// @brief Stack of operators and functions.
        mutable std::stack<Token> operatorStack_;

        /// @brief All expected operators and their priorities (precedences).
        std::unordered_map<std::string, uint_fast16_t> operators_;
    };

} // namespace s2e2
