#pragma once

#include "token.hpp"

#include <cstdint>
#include <list>
#include <string>


namespace s2e2
{
    /**
     * @class IConverter
     * @brief Interface class  for converting infix token sequence into postfix one.
     */
    class IConverter
    {
    public:
        /**
         * @brief Just a virtual destructor.
         */
        virtual ~IConverter() = default;

        /**
         * @brief Add operator expected within expression.
         * @param[in] name - Operator's name.
         * @param[in] priority - Operator's priority.
         * @throws Error if operator's name is not unique.
         */
        virtual void addOperator(const std::string& name, const uint_fast16_t priority) = 0;

        /**
         * @brief Convert infix token sequence into postfix one.
         * @param[in] infixExpression - Input sequence of tokens.
         * @returns Postfix sequence of tokens.
         * @throws Error in case of an error.
         */
        virtual std::list<Token> convert(const std::list<Token>& infixExpression) const = 0;
    };

} // namespace s2e2
