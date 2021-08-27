#ifndef MINICHRONOS_TIMER_H
#define MINICHRONOS_TIMER_H

#include <string>

namespace MiniChronos
{
    class Database;

    class Timer
    {
    public:
        explicit Timer(Database& db);

        void start(std::string&& timer_id);

    private:
        Database& db;
    };
}

#endif //MINICHRONOS_TIMER_H
