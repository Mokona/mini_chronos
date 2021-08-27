#include "mini_chronos.h"

#include "database.h"
#include "timer.h"

#include <memory>

namespace MiniChronos
{
    std::unique_ptr<Timer> the_one_timer;

    void init(Database& db) { the_one_timer = std::make_unique<Timer>(db); };
    void start(std::string&& timer_id) { the_one_timer->start(std::move(timer_id)); }
}
