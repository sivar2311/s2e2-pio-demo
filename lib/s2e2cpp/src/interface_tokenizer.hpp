#pragma once

#include "token.hpp"

#include <list>
#include <string>


namespace s2e2
{
    /**
     * @class ITokenizer
     * @brief Interface class for splitting expression string into list of tokens.
     */
    class ITokenizer
    {
    public:
        /**
         * @brief Just a virtual destructor.
         */
        virtual ~ITokenizer() = default;

        /**
         * @brief Add function expected within expression.
         * @param[in] functionName - Function's name.
         * @throws Error if functons's name is not unique.
         */
        virtual void addFunction(const std::string& functionName) = 0;

        /**
         * @brief Add operator expected within expression.
         * @param[in] operatorName - Operator's name.
         * @throws Error if operator's name is not unique.
         */
        virtual void addOperator(const std::string& operatorName) = 0;

        /**
         * @brief Split expression into tokens.
         * @param[in] expression - Input expression.
         * @returns List of tokens.
         * @throws Error if expression contains unknown symbol.
         */
        virtual std::list<Token> tokenize(const std::string& expression) const = 0;
    };

} // namespace s2e2
