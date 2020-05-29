#ifndef __session_h__
#define __session_h__

#include <htk/lap.h>
#include <string>
#include <vector>

namespace chronograph
{
    class session_config
    {
    };

    struct session_context
    {
        const std::string name;
        const clock::time_point when;
    };

    struct run
    {
        run(const std::string &name, int group)
            : name(name), group(group)
        {
        }

        const int group;
        const std::string name;
        lap total;
        std::vector<lap> laps;
    };

    class session
    {

    public:
        session()
            : group_(0)
        {
        }

        std::pair<size_t, run &> begin_run(const std::string &name, int group)
        {
            runs_.emplace_back(name, group);
            start(runs_.back().total);
            return std::pair<size_t, run &>(runs_.size() - 1, runs_.back());
        }
        void end_run(size_t index)
        {
            auto &r = runs_[index];
            stop(r.total);
        }

        const run &get_run(size_t index) const
        {
            return runs_[index];
        }

        const session_context &context() const
        {
            return context_;
        }

        const std::vector<run> &runs() const
        {
            return runs_;
        }

        int next_group()
        {
            return group_++;
        }
        

    private:
        session_context context_;
        std::vector<run> runs_;
        int group_;
    };

    class session_run
    {
    public:
        session_run(session &s, const std::string &name, int group)
            : session_(s), index_(-1), run_(nullptr)
        {
            const auto &[index, run] = s.begin_run(name, group);
            index_ = index;
            run_ = &run;
        }
        ~session_run()
        {
            if (index_ == -1)
                throw std::exception("bad run");
            session_.end_run(index_);
        }
        void record(const lap &l)
        {
            run_->laps.emplace_back(l);
        }


    private:
        int index_;
        session &session_;
        run *run_;
    };
}

#endif // __session_h__