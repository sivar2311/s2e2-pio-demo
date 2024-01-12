#include <s2e2/error.hpp>


s2e2::Error::Error(const std::string& message)
    : std::runtime_error(message)
{
}

s2e2::Error::Error(const char* message)
    : std::runtime_error(message)
{
}
