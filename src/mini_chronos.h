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
            TimeProvider::now();
        }
        void stop()
        {
            TimeProvider::now();
            error_handler.fatal("Cannot stop a timer when none were started.");
        }

    private:
        Database& db;
        ErrorHandler error_handler;
    };
}
