#include "mini_chronos.h"

#include "database.h"
#include "timer.h"

#include <memory>

namespace MiniChronos
{
    std::unique_ptr<Timer> the_one_timer{};

    void init(Database& db, ErrorHandler error_handler)
    {
        if (the_one_timer != nullptr)
        {
            error_handler.fatal("MiniChronos cannot be initialized twice");
        }
        the_one_timer = std::make_unique<Timer>(db, error_handler);
    };

    void uninit()
    {
        the_one_timer.reset();
    }

    void start(std::string&& timer_id) { the_one_timer->start(std::move(timer_id)); }
}
