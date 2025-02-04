#ifndef HISTOGRAM_H
#define HISTOGRAM_H
#include <functional>


#include "../../sorting/DefaultComparators.h"
#include "../../sorting/Person.h"
#include "../../sorting/quickSort.h"
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
        educations.Insert("Среднее профессиональное", 0);
        educations.Insert("Бакалавриат", 0);
        educations.Insert("Магистратура", 0);
        educations.Insert("Аспирантура", 0);
        maritalStatuses.Insert("В браке", 0);
        maritalStatuses.Insert("Не в браке", 0);
        maritalStatuses.Insert("В разводе", 0);
        maritalStatuses.Insert("Вдовец/Вдова", 0);
    };
};

struct Statistics {
    double median;
    double mean;
    double variance;

    Statistics() = default;

    Statistics(const double median, const double mean, const double variance) :
        median(median), mean(mean), variance(variance) {}
};

struct PartitionStatistics {
    Statistics ages{};
    Statistics weights{};
    Statistics heights{};
    Statistics salaries{};
    IDictionary<std::string, size_t> genders;
    IDictionary<std::string, size_t> educations;
    IDictionary<std::string, size_t> maritalStatuses;

    ArraySequence<int> agesData;
    ArraySequence<int> weightsData;
    ArraySequence<int> heightsData;
    ArraySequence<int> salariesData;
};

class Histogram final {
    using Range = std::pair<int, int>;
    IDictionary<Range, Partition> partitions;

    static Statistics CalculateStatisticsForField(const ArraySequence<int>& sequence) {
        double median, mean = 0.0, variance = 0.0;

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

    Range getRange(const int field, const ArraySequence<Range>& ranges) {
        for (const auto& range: ranges) {
            if (field >= range.first && field < range.second) {
                return range;
            }
        }
        return {-1, -1};
    }

public:
    Histogram() = default;

    void Build(const ArraySequence<Person>& persons, const ArraySequence<Range>& ranges,
              const std::function<int(const Person&)>& field) {
        for (const auto& range: ranges) {
            partitions.Insert(range, Partition());
        }

        for (const auto& person: persons) {
            const int partitionField = field(person);
            Range range = std::move(getRange(partitionField, ranges));
            if (range.first != -1 && range.second != -1) {
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

        QuickSorter<int> sorter;
        for (auto& [range, partition]: partitions) {
            sorter.Sort(partition.ages, ascendingComparator);
            sorter.Sort(partition.weights, ascendingComparator);
            sorter.Sort(partition.heights, ascendingComparator);
            sorter.Sort(partition.salaries, ascendingComparator);
        }
    }

    IDictionary<Range, PartitionStatistics> GetStatistics() {
        IDictionary<Range, PartitionStatistics> result;

        for (const auto& [range, partition]: partitions) {
            PartitionStatistics stats;
            stats.ages = CalculateStatisticsForField(partition.ages);
            stats.weights = CalculateStatisticsForField(partition.weights);
            stats.heights = CalculateStatisticsForField(partition.heights);
            stats.salaries = CalculateStatisticsForField(partition.salaries);
            stats.genders = partition.genders;
            stats.educations = partition.educations;
            stats.maritalStatuses = partition.maritalStatuses;
            stats.agesData = partition.ages;
            stats.weightsData = partition.weights;
            stats.heightsData = partition.heights;
            stats.salariesData = partition.salaries;
            result.Insert(range, stats);
        }

        return result;
    }

    ~Histogram() = default;
};

#endif // HISTOGRAM_H
