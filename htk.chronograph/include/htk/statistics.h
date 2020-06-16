#ifndef __statistics_h__
#define __statistics_h__

#include <htk/session.h>

#include <algorithm>

namespace chronograph
{
    using ll = long long;
    struct statistics
    {
        size_t iterations;
        float mean;
        float stddev;
        float ci;
        ll median;
        ll min;
        ll max;
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
    std::tuple<ll, ll, ll> median(std::vector<lap> laps)
    {
        std::sort(laps.begin(), laps.end(), [](const lap& l, const lap& r) { return l.duration() < r.duration(); });
        size_t mid = laps.size() / 2;
        auto midval = std::chrono::duration_cast<DurationT>(laps[mid].duration()).count();
        auto min = std::chrono::duration_cast<DurationT>(laps[0].duration()).count();
        auto max = std::chrono::duration_cast<DurationT>(laps[laps.size() - 1].duration()).count();
        return std::make_tuple( midval, min, max );
    }


    template <typename DurationT>
    float stddev(const std::vector<lap> &laps, float mean)
    {
        float sum = 0;
        for (const auto &lap : laps)
        {
            auto v = std::chrono::duration_cast<DurationT>(lap.duration()).count() - mean;
            sum += v*v;
        }
        return std::sqrt(sum / laps.size()-1);
    }

    template <typename DurationT>
    float ci(float dev, int size, float confidence = 1.96)
    {
        auto v = dev / std::sqrt(size);
        return v * confidence;
    }

    template<typename DurationT>
    statistics calculate_statistics(const run& run)
    {
        auto [median,min,max] = median<DurationT>(run.laps);
        statistics s;
        s.iterations = run.laps.size();
        s.median = median;
        s.min = min;
        s.max = max;
        s.mean = mean<DurationT>(run.laps);
        s.stddev = stddev<DurationT>(run.laps, s.mean);
        s.ci = ci<DurationT>(s.stddev, run.laps.size());
        return s;
    }

};

#endif // __statistics_h__
