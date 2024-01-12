#pragma once

#include <cstdint>

/**
 * @bried Priorities of standard operators.
 */
namespace s2e2::Priorities
{
    inline constexpr uint_fast16_t OPERATOR_AND = 200;
    inline constexpr uint_fast16_t OPERATOR_EQUAL = 300;
    inline constexpr uint_fast16_t OPERATOR_GREATER = 400;
    inline constexpr uint_fast16_t OPERATOR_GREATER_OR_EQUAL = 400;
    inline constexpr uint_fast16_t OPERATOR_LESS = 400;
    inline constexpr uint_fast16_t OPERATOR_LESS_OR_EQUAL = 400;
    inline constexpr uint_fast16_t OPERATOR_NOT = 600;
    inline constexpr uint_fast16_t OPERATOR_NOT_EQUAL = 300;
    inline constexpr uint_fast16_t OPERATOR_OR = 100;
    inline constexpr uint_fast16_t OPERATOR_PLUS = 500;
}
