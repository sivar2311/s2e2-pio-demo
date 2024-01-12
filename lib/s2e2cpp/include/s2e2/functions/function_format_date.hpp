#pragma once

#include <s2e2/function.hpp>

#include <any>


namespace s2e2
{
    /**
     * @class FunctionFormatDate
     * @brief Function FORMAT_DATE(<datetime>, <format>)
     * @details Converts datetime to string according to format.
     */
    class FunctionFormatDate final : public Function
    {
    public:
        /**
         * Default constructor.
         */
        FunctionFormatDate();

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
