#ifndef HISTOGRAM_H
#define HISTOGRAM_H
#include <functional>


#include "../../sorting/Person.h"
#include "../../sorting/quickSort.h"
#include "../../sorting/DefaultComparators.h"
#include "../headers/IDictionary.h"


template<typename T1, typename T2>
struct std::hash<std::pair<T1, T2>> {
    std::size_t operator()(const std::pair<T1, T2>& p) const {
        std::size_t h1 = std::hash<T1>{}(p.first);
        std::size_t h2 = std::hash<T2>{}(p.second);

        return h1 ^ (h2 << 1);
    }
};

struct Partition {
    ArraySequence<int> ages;
    ArraySequence<int> weights;
    ArraySequence<int> heights;
    ArraySequence<int> salaries;
    IDictionary<std::string, size_t> genders;
    IDictionary<std::string, size_t> educations;
    IDictionary<std::string, size_t> maritalStatuses;

    Partition() {
        genders.Insert("Мужчина", 0);
        genders.Insert("Женщина", 0);
        educations.Insert("Основное общее", 0);
        educations.Insert("Среднее общее", 0);
        educations.Insert("Бакалавриат", 0);
        educations.Insert("Магистратура", 0);
        educations.Insert("Аспирантура", 0);
        maritalStatuses.Insert("В браке", 0);
        maritalStatuses.Insert("Не в браке", 0);
        maritalStatuses.Insert("В разводе", 0);
    };
};

struct Statistics {
    double median;
    double mean;
    double variance;

    Statistics() = default;

    Statistics(double median, double mean, double variance) : median(median), mean(mean), variance(variance) {}
};

struct PartitionStatistics {
    Statistics ages;
    Statistics weights;
    Statistics heights;
    Statistics salaries;
    IDictionary<std::string, size_t> genders;
    IDictionary<std::string, size_t> educations;
    IDictionary<std::string, size_t> maritalStatuses;


};

class Histogram final {
    using Range = std::pair<int, int>;
    IDictionary<Range, Partition> partitions;

    Statistics CalculateStatisticsForField(ArraySequence<int>& sequence) {
        double median, mean = 0.0, variance = 0.0;
        QuickSorter<int> sorter;
        sorter.Sort(sequence, ascendingComparator);

        size_t n = sequence.GetLength();
        if (n % 2 == 0) {
            median = (sequence[n / 2 - 1] + sequence[n / 2]) / 2.0;
        } else {
            median = sequence[n / 2];
        }

        n = static_cast<double>(n);
        for (int data: sequence) {
            mean += data;
        }
        mean /= n;

        for (int data: sequence) {
            variance += std::pow(data - mean, 2);
        }
        variance /= n;

        return {median, mean, variance};
    }
/*
    PartitionStatistics calculateStatisticsForPartitions() {
        for (auto& [key, value]: partitions) {
            CalculateStatisticsForField(partition..ages);
            CalculateStatisticsForField(partition.element.weights);
            CalculateStatisticsForField(partition.element.heights);
            CalculateStatisticsForField(partition.element.salaries);
        }
    }
*/
    Range getRange(const int field, const ArraySequence<Range>& ranges) {
        for (const auto& range: ranges) {
            if (field >= range.first && field <= range.second) {
                return range;
            }
        }
        return {};
    }

public:
    Histogram(const ArraySequence<Person>& persons, const ArraySequence<Range>& ranges,
              const std::function<int(const Person&)>& field) {
        for (const auto& range: ranges) {
            partitions.Insert(range, Partition());
        }

        for (const auto& person: persons) {
            int partitionField = field(person);
            Range range = std::move(getRange(partitionField, ranges));
            Partition& partition = partitions[range];
            partition.ages.Append(person.getAge());
            partition.weights.Append(person.getWeight());
            partition.heights.Append(person.getHeight());
            partition.salaries.Append(person.getSalary());
            ++partition.genders[person.getGender()];
            ++partition.educations[person.getEducation()];
            ++partition.maritalStatuses[person.getMaritalStatus()];
        }
    }

    IDictionary<Range, Statistics> getStatistics() {
        IDictionary<Range, PartitionStatistics> result;

    }

    ~Histogram() = default;
};

#endif // HISTOGRAM_H
