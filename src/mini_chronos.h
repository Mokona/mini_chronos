#ifndef MINICHRONOS_MINI_CHRONOS_H
#define MINICHRONOS_MINI_CHRONOS_H

#include "database.h"

#include <cassert>
#include <chrono>
#include <string>

namespace MiniChronos
{
    template <typename TimeProvider>
        requires std::chrono::is_clock_v<TimeProvider>
    class Chronos
    {
    public:
        using TimePoint = TimeProvider::time_point;

        [[nodiscard]] Database::TimerIterator begin() const { return Database::TimerIterator{&db}; }
        [[nodiscard]] Database::TimerIterator end() const { return Database::TimerIterator{&db}.end(); }

        explicit Chronos(Database& db) : db(db) {}

        Database::PathId create_path(const std::string& timer_id)
        {
            const auto new_path = db.ensures_path(timer_id);

            // timer_start_points is a dense vector as path ids are consecutive
            // and paths are never removed.
            if (new_path >= timer_start_points.size())
            {
                timer_start_points.resize(new_path + 1);
            }

            return new_path;
        }

        void start(Database::PathId path_id)
        {
            assert(db.is_path_valid(path_id));
            timer_start_points[path_id] = TimeProvider::now();
        }

        void stop(Database::PathId path_id)
        {
            assert(db.is_path_valid(path_id));
            auto timer_stop = TimeProvider::now();
            auto timer_start = timer_start_points[path_id];

            update_path(path_id, timer_start, timer_stop);
        }

        void reset()
        {
            db.reset();
        }

        [[nodiscard]] Database::TimerData get_timer_data(const std::string& path) const
        {
            return db.get_timer_data(path);
        }

    private:
        Database& db;
        std::vector<TimePoint> timer_start_points;

        void update_path(Database::PathId path_id, TimePoint start, TimePoint stop)
        {
            const auto duration = duration_cast<std::chrono::nanoseconds>(stop - start);
            const auto current_duration = db.get_timer_data(path_id).duration;
            db.set_duration(path_id, current_duration + duration);
            db.inc_call_count(path_id);
        }
    };
}

#endif // MINICHRONOS_MINI_CHRONOS_H
