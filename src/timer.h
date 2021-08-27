#ifndef MINICHRONOS_TIMER_H
#define MINICHRONOS_TIMER_H

#include "error_handler.h"

#include <string>

namespace MiniChronos
{
    class Database;

    class Timer
    {
    public:
        Timer(Database& db, ErrorHandler error_handler);

        void start(std::string&& timer_id);

    private:
        Database& db;
        ErrorHandler error_handler;
    };
}

#endif //MINICHRONOS_TIMER_H
