#include "../headers/MostFrequentSubsequences.h"

IDictionary<std::string, size_t, FNV1a<std::string>> createPrefixTable(const std::string& str, const size_t lmin,
                                                                       const size_t lmax) {
    IDictionary<std::string, size_t, FNV1a<std::string>> table;
    const size_t n = str.size();

    if (lmin > lmax || str.empty()) {
        throw std::out_of_range("Invalid indices");
    }

    for (size_t start = 0; start < n; ++start) {
        for (size_t len = lmin; len <= lmax && start + len <= n; ++len) {
            if (std::string substr = str.substr(start, len); table.Contains(substr)) {
                ++table.Get(substr);
            } else {
                table.Insert(substr, 1);
            }
        }
    }

    return table;
}
