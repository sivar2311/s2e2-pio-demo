#pragma once

#include <stdexcept>
#include <string>


namespace s2e2
{
    /**
     * @class Error
     * @brief Error thrown from inside the s2e2 library.
     * @details Is a subsclass of std::runtime_error.
     */
    class Error final : public std::runtime_error
    {
    public:
        /**
         * @brief Construct the error object.
         * @param[in] message - Exception description as a C++ string.
         */
        explicit Error(const std::string& message);

        /**
         * @brief Construct the error object.
         * @param[in] message - Exception description as a C string.
         */
        explicit Error(const char* message);
    };

} // namespace s2e2
