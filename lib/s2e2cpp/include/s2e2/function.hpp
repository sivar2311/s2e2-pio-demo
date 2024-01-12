#pragma once

#include <any>
#include <cstdint>
#include <stack>
#include <string>
#include <vector>


namespace s2e2
{
    /**
     * @class Function
     * @brief Base class of all functions.
     */
    class Function
    {
    public:
        /**
         * @brief Just a virtual destructor.
         */
        virtual ~Function() = default;

        /**
         * @brief Invoke the function - pop all its arguments from the stack and put result in.
         * @param stack[in, out] - Stack with arguments.
         * @throws Error in case of wrong number or types of arguments.
         */
        void invoke(std::stack<std::any>& stack) const;

    protected:
        /**
         * @brief Constructor.
         * @param functionName[in] - Function's name.
         * @param numberOfArguments[in] - Number of arguments.
         */
        Function(std::string functionName, const uint_fast16_t numberOfArguments);

        /**
         * @brief Check if arguments are correct.
         * @returns true is arguments are correct, false otherwise.
         */
        virtual bool checkArguments() const = 0;

        /**
         * @brief Calculate result of the function.
         * @return Result.
         */
        virtual std::any result() const = 0;

    public:
        /// @brief Function's name.
        const std::string name;

    protected:
        /// @brief Vector of arguments.
        mutable std::vector<std::any> arguments_;
    };

} // namespace s2e2
