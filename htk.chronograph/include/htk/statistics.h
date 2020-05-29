#ifndef __statistics_h__
#define __statistics_h__

#include <htk/session.h>

#include <algorithm>

namespace chronograph
{
    struct statistics
    {
        float mean;
        long long median;
    };

    template<typename DurationT>
    float mean(const std::vector<lap>& laps)
    {
        float sum = 0;
        for (const auto& lap : laps)
        {
            sum += std::chrono::duration_cast<DurationT>(lap.duration()).count();
        }
        return sum / laps.size();
    }

    template<typename DurationT>
    long long median(std::vector<lap> laps)
    {
        std::sort(laps.begin(), laps.end(), [](const lap& l, const lap& r) { return l.duration() < r.duration(); });
        size_t mid = laps.size() / 2;
        return std::chrono::duration_cast<DurationT>(laps[mid].duration()).count();
    }

    template<typename DurationT>
    statistics calculate_statistics(const run& run)
    {
        statistics s;
        s.mean = mean<DurationT>(run.laps);
        s.median = median<DurationT>(run.laps);
        return s;
    }

};

#endif // __statistics_h__
