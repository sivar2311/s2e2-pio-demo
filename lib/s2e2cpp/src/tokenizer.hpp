#pragma once

#include "interface_tokenizer.hpp"
#include "token_type.hpp"
#include "token.hpp"

#include <functional>
#include <list>
#include <map>
#include <unordered_set>


namespace s2e2
{
    /**
     * @class Tokenizer
     * @brief Class splits plain string expressions into list of tokens.
     */
    class Tokenizer final : public ITokenizer
    {
    public:
        /**
         * @brief Add function expected within expression.
         * @param[in] functionName - Function's name.
         * @throws Error if functions's name is not unique.
         */
        void addFunction(const std::string& functionName) override;

        /**
         * @brief Add operator expected within expression.
         * @param[in] operatorName - Operator's name.
         * @throws Error if operator's name is not unique.
         */
        void addOperator(const std::string& operatorName) override;

        /**
         * @brief Split expression into tokens.
         * @param[in] expression - Input expression.
         * @returns List of tokens.
         * @throws Error if expression contains unknown symbol.
         */
        std::list<Token> tokenize(const std::string& expression) const override;

    private:
        /**
         * @brief Check is function's or operator's name is unique.
         * @param[in] entityName - Function's or operator's name.
         * @throws Error if the name is not unique.
         */
        void checkUniqueness(const std::string& entityName) const;

        /**
         * @brief Split all tokens by all expected operatos.
         * @details This is required since there can be no spaces between operator and its operands.
         * @param[in] tokens - List of tokens.
         * @returns List of splitted tokens.
         */
        std::list<Token> splitTokensByOperators(const std::list<Token>& tokens) const;

        /**
         * @brief Split all tokens by one operator.
         * @param[in, out] tokens - List of tokens.
         * @param[in] operatorName - Operator to split tokens by.
         */
        void splitTokensBySingleOperator(std::list<Token>& tokens, const std::string& operatorName) const;

        /**
         * @brief Split one token by one operator.
         * @param[in, out] tokens - List of all tokens.
         * @param[in, out] tokenIterator - Iterator pointing to the current token.
         * @param[in] operatorName  - Operator to split token by.
         */
        void splitSingleTokenBySingleOperator(std::list<Token>& tokens,
                                              std::list<Token>::iterator& tokenIterator,
                                              const std::string& operatorName) const;

        /**
         * @brief Get token type by its value.
         * @param[in] value - Token's value.
         * @returns Token's type.
         */
        TokenType tokenTypeByValue(const std::string& value) const;

    private:
        /// @brief Set of expected functions.
        std::unordered_set<std::string> functions_;

        /// @brief Set of expected operators.
        std::unordered_set<std::string> operators_;

        /// @brief Operators sorted by their lengthes (for instance: 1 -> !, +; 2 -> ||, &&)
        std::map<size_t, std::unordered_set<std::string>, std::greater<size_t>> operatorsByLength_;
    };

} // namespace s2e2
