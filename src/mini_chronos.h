#include "database.h"
#include "error_handler.h"

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
        Database::TimerIterator begin() { return Database::TimerIterator{&db}; }
        Database::TimerIterator end() { return Database::TimerIterator{&db}.end(); }

        Chronos(Database& db, ErrorHandler error_handler)
            : db(db), error_handler(std::move(error_handler))
        {}

        void start(std::string&& timer_id)
        {
            current_path = db.ensures_path(timer_id);
            timer_start = TimeProvider::now();
        }
        void stop()
        {
            if (current_path == Database::no_path)
            {
                error_handler.fatal("Cannot stop a timer when none were started.");
                return;
            }
            auto timer_stop = TimeProvider::now();
            const auto duration = duration_cast<std::chrono::nanoseconds>(timer_stop - timer_start);
            db.set_duration(current_path, duration);
            current_path = Database::no_path;
        }

        Database::TimerData get_timer_data(const std::string& string)
        {
            return db.get_timer_data();
        }

    private:
        Database& db;
        Database::PathId current_path{Database::no_path};
        ErrorHandler error_handler;

        typename TimeProvider::time_point timer_start;
    };
}
