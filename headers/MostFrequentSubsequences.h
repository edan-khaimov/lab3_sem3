#ifndef MOSTFREQUENTSUBSEQUENCES_H
#define MOSTFREQUENTSUBSEQUENCES_H
#include "FNV1aHash.h"
#include "IDictionary.h"

IDictionary<std::string, size_t, FNV1a<std::string>> createPrefixTable(const std::string& str, size_t lmin,
                                                                       size_t lmax);

#endif // MOSTFREQUENTSUBSEQUENCES_H
