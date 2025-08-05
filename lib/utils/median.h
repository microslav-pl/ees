#pragma once

#include <algorithm>
#include <vector>

template<typename T>

/**
 * @brief Return median value from array of values.
 * @param[in] values    Array of values
 * @param[in] size      Size of array
 * @return              Median value (float)
 */
float medianFromArray(T values[], int size) {
    std::vector<T> sorted(values, values + size);
    std::sort(sorted.begin(), sorted.end());

    if (size % 2 == 0) {
        return (static_cast<float>(sorted[size/2 - 1]) + static_cast<float>(sorted[size/2])) / 2.0;
    } else {
        return static_cast<float>(sorted[size/2]);
    }
}
