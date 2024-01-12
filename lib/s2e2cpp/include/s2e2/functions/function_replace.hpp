#pragma once

#include <s2e2/function.hpp>

#include <any>


namespace s2e2
{
    /**
     * @class FunctionReplace
     * @brief Function REPLACE(<source>, <regex>, <replacement>)
     * @details Returns copy of source with all matches of regex replaced by replacement.
     */
    class FunctionReplace final : public Function
    {
    public:
        /**
         * Default constructor.
         */
        FunctionReplace();

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
