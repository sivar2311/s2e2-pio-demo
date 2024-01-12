#include "utils.hpp"


std::time_t s2e2::utcTs(std::tm* tm)
{
    const auto localTs = mktime(tm);
    const auto localToUTcDiff = std::mktime(std::localtime(&localTs)) - std::mktime(std::gmtime(&localTs));
    return localTs + localToUTcDiff;
}
