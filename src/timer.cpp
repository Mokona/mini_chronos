#include "timer.h"

#include <utility>

#include "database.h"

MiniChronos::Timer::Timer(Database& db, ErrorHandler error_handler)
    : db(db), error_handler(std::move(error_handler))
{}

void MiniChronos::Timer::start(std::string&& timer_id) { db.ensures_key(timer_id); }
