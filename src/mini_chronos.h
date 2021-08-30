#include "error_handler.h"
#include <chrono>
#include <string>
#include <utility>

namespace MiniChronos
{
    class Database;

    void initialize_the_one_timer(Database& db, ErrorHandler error_handler);

    template<typename TimeProvider>
#if __cpp_lib_concepts
    requires std::chrono::is_clock_v<TimeProvider>
#endif
    void init(Database& db, ErrorHandler error_handler)
    {
        initialize_the_one_timer(db, std::move(error_handler));
    };

    void uninit();
    void start(std::string&& timer_id);
    void stop();
}
