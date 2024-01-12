#pragma once

#include <s2e2/function.hpp>

#include <any>


namespace s2e2
{
    /**
     * @class FunctionAddDays
     * @brief Function ADD_DAYS(<datetime>, <days>)
     * @details Adds number of days to datetime.
     */
    class FunctionAddDays final : public Function
    {
    public:
        /**
         * Default constructor.
         */
        FunctionAddDays();

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
