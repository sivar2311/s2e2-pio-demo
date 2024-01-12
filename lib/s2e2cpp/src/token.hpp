#pragma once

#include "token_type.hpp"

#include <string>


namespace s2e2
{
    /**
     * @class Token
     * @brief Represents a single token i.e. a unit of some expression.
     */
    class Token final
    {
    public:
        /**
         * @brief Construct the token.
         * @param[in] tokenType - Type of the token.
         * @param[in] tokenValue - String value of the token.
         */
        Token(const TokenType tokenType, std::string tokenValue);

        /**
         * @brief Compare token with another token.
         * @param[in] another - Another token.
         * @returns true if tokens are equal, false otherwise.
         */
        bool operator==(const Token& another) const;

    public:
        /// @brief Token's type.
        const TokenType type;

        /// @brief Token's string value.
        const std::string value;
    };

} // namespace s2e2
