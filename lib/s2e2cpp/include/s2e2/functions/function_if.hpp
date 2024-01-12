#pragma once

#include <s2e2/function.hpp>

#include <any>


namespace s2e2
{
    /**
     * @class FunctionIf
     * @brief Function IF(<conition>, <value1>, <value2>)
     * @details Returns value1 if boolean condition is true, and value2 otherwise.
     */
    class FunctionIf final : public Function
    {
    public:
        /**
         * Default constructor.
         */
        FunctionIf();

    private:
        /**
         * @brief Check if arguments are correct.
         * @returns true is arguments are correct, false otherwise.
         */
        bool checkArguments() const override;

        /**
         * @brief Calculate result of the function.
         * @return Result.
         */
        std::any result() const override;
    };

} // namespace s2e2
