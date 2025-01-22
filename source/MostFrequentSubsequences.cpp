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
                ++table[substr];
            } else {
                table.Insert(substr, 1);
            }
        }
    }

    return table;
}

void processFileAndSaveResults(const std::string& inputFile, const std::string& outputFile, size_t lmin, size_t lmax) {
    std::ifstream inFile(inputFile);
    if (!inFile) {
        throw std::runtime_error("Failed to open input file.");
    }

    std::string content((std::istreambuf_iterator(inFile)), std::istreambuf_iterator<char>());
    inFile.close();

    IDictionary<std::string, size_t, FNV1a<std::string>> prefixTable = std::move(createPrefixTable(content, lmin, lmax));

    std::ofstream outFile(outputFile);
    if (!outFile) {
        throw std::runtime_error("Failed to open output file.");
    }

    for (const auto& [key, value] : prefixTable) {
        outFile << key << " - " << value << std::endl;
    }

    outFile.close();
}
