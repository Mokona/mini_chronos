#include "timer.h"

#include "database.h"

MiniChronos::Timer::Timer(Database& db) : db(db) {}
void MiniChronos::Timer::start(std::string&& timer_id) { db.ensures_key(timer_id); }
