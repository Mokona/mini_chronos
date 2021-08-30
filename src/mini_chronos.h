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
        Chronos(Database& db, ErrorHandler error_handler)
            : db(db), error_handler(std::move(error_handler))
        {}

        void start(std::string&& timer_id)
        {
            db.ensures_key(timer_id);
            timer_start = TimeProvider::now();
        }
        void stop()
        {
            auto timer_stop = TimeProvider::now();
            const auto duration = duration_cast<std::chrono::nanoseconds>(timer_stop - timer_start);
            latest_duration = duration;
            error_handler.fatal("Cannot stop a timer when none were started.");
        }

        struct TimerData
        {
            std::chrono::nanoseconds duration;
        };

        TimerData get_timer_data(const std::string& string)
        {
            return TimerData{std::chrono::nanoseconds{latest_duration.count()}};
        }

    private:
        Database& db;
        ErrorHandler error_handler;

        typename TimeProvider::time_point timer_start;
        typename TimeProvider::duration latest_duration;
    };
}
