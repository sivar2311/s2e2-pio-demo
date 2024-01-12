#pragma once

#include <s2e2/operator.hpp>

#include <any>


namespace s2e2
{
    /**
     * @class OperatorPlus
     * @brief Operator +
     * @details Concatenates two strings.
     */
    class OperatorPlus final : public Operator
    {
    public:
        /**
         * Default constructor.
         */
        OperatorPlus();

    private:
        /**
         * @brief Check if arguments are correct.
         * @returns true is arguments are correct, false otherwise.
         */
        bool checkArguments() const override;

        /**
         * @brief Calculate result of the operator.
         * @return Result.
         */
        std::any result() const override;
    };

} // namespace s2e2
