#ifndef HISTOGRAM_H
#define HISTOGRAM_H
#include <sstream>
#include <numeric>
#include <cmath>
#include <algorithm>
#include "../../sorting/Person.h"
#include "../headers/SortedSequence.h"

struct Range {
    int start;
    int end;

    bool operator==(const Range& other) const { return start == other.start && end == other.end; }

    std::string ToString() const {
        std::ostringstream oss;
        oss << "[" << start << ", " << end << ")";
        return oss.str();
    }
};

template<>
struct std::hash<Range> {
    size_t operator()(const Range& range) const noexcept {
        const size_t h1 = std::hash<double>{}(range.start);
        const size_t h2 = std::hash<double>{}(range.end);
        return h1 ^ (h2 << 1);
    }
};

struct NumericStatistics {
    size_t count = 0;
    double sum = 0.0;
    double sumOfSquares = 0.0;
    SortedSequence<double> values;

    void AddValue(const double value) {
        ++count;
        sum += value;
        sumOfSquares += value * value;
        values.Add(value);
    }

    double GetMean() const {
        return count > 0 ? sum / count : 0.0;
    }

    double GetVariance() const {
        if (count > 1) {
            const double meanVal = GetMean();
            return (sumOfSquares / count) - (meanVal * meanVal);
        }
    }

    double GetMedian() const {
        if (!values.IsEmpty()) {
            const size_t mid = values.GetSize() / 2;
            return values.GetSize() % 2 == 0 ? (values[mid - 1] + values[mid]) / 2.0 : values[mid];
        }
        return 0.0;
    }

    std::string ToString() const {
        std::ostringstream oss;
        oss << "Count: " << count << ", Mean: " << mean() << ", Variance: " << variance() << ", Median: " << median();
        return oss.str();
    }
};


#endif // HISTOGRAM_H
