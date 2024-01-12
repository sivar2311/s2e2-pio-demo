#pragma once

#include <ctime>


namespace s2e2
{
    /**
     * @brief Convert UTC datetime into number of seconds since epoch.
     * @param[in] tm - Pointer to UTC datetime.
     * @returns Seconds since epoch.
     */
    std::time_t utcTs(std::tm* tm);

} // namespace s2e2
