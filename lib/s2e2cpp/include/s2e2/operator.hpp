#pragma once

#include <any>
#include <cstdint>
#include <stack>
#include <string>
#include <vector>


namespace s2e2
{
    /**
     * @class Operator
     * @brief Base class of all operators.
     */
    class Operator
    {
    public:
        /**
         * @brief Just a virtual destructor.
         */
        virtual ~Operator() = default;

        /**
         * @brief Invoke the operator - pop all its arguments from the stack and put result in.
         * @param stack[in, out] - Stack with arguments.
         * @throws Error in case of wrong number or types of arguments.
         */
        void invoke(std::stack<std::any>& stack) const;

    protected:
        /**
         * @brief Constructor.
         * @param operatorName[in] - Operator's name.
         * @param priority[in] - Operator's priority.
         * @param numberOfArguments[in] - Number of arguments.
         */
        Operator(std::string operatorName,
                 const uint_fast16_t operatorPriority,
                 const uint_fast16_t numberOfArguments);

        /**
         * @brief Check if arguments are correct.
         * @returns true is arguments are correct, false otherwise.
         */
        virtual bool checkArguments() const = 0;

        /**
         * @brief Calculate result of the operator.
         * @return Result.
         */
        virtual std::any result() const = 0;

    public:
        /// @brief Operator's name.
        const std::string name;

        /// @brief Priority of the operator.
        const uint_fast16_t priority;

    protected:
        /// @brief Vector of arguments.
        mutable std::vector<std::any> arguments_;
    };

} // namespace s2e2
