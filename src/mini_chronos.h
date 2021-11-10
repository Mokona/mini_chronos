#include "database.h"
#include "error_handler.h"

#include <cassert>
#include <chrono>
#include <string>
#include <utility>

namespace MiniChronos
{
    template<typename TimeProvider>
    requires std::chrono::is_clock_v<TimeProvider>
    class Chronos
    {
    public:
        using TimePoint = typename TimeProvider::time_point;

        Database::TimerIterator begin() { return Database::TimerIterator{&db}; }
        Database::TimerIterator end() { return Database::TimerIterator{&db}.end(); }

        Chronos(Database& db, ErrorHandler error_handler)
            : db(db), error_handler(std::move(error_handler))
        {
            const auto no_path = Database::no_path;
            path_stack.push_back(no_path);
        }

        void start(std::string&& timer_id)
        {
            auto new_path = db.ensures_path(current_path(), timer_id);
            push_path(new_path);

            // Based on the fact that current_path is an index in another table... not really good.
            if (current_path() >= timer_start_points.size())
            {
                timer_start_points.resize(current_path() + 1);
            }
            timer_start_points[current_path()] = TimeProvider::now();
        }

        void stop()
        {
            if (current_path() == Database::no_path)
            {
                error_handler.fatal("Cannot stop a timer when none were started.");
                return;
            }
            auto timer_stop = TimeProvider::now();
            auto timer_start = timer_start_points[current_path()];

            update_current_path(timer_start, timer_stop);

            pop_path();
        }

        void update_current_path(TimePoint start, TimePoint stop)
        {
            const auto duration = duration_cast<std::chrono::nanoseconds>(stop - start);
            const auto current_duration = db.get_timer_data(current_path()).duration;
            const auto new_duration = current_duration + duration;
            db.set_duration(current_path(), new_duration);
            db.inc_call_count(current_path());
        }

        Database::TimerData get_timer_data(const std::string& path)
        {
            return db.get_timer_data(path);
        }

        [[nodiscard]] Database::PathId current_path() const
        {
            assert(!path_stack.empty());
            return path_stack.back();
        }

    private:
        Database& db;
        ErrorHandler error_handler;
        std::vector<TimePoint> timer_start_points;
        std::vector<Database::PathId> path_stack;

        void push_path(Database::PathId path) { path_stack.push_back(path); }
        void pop_path()
        {
            path_stack.pop_back();
            assert(!path_stack.empty());
        }
    };
}
