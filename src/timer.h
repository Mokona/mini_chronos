#ifndef MINICHRONOS_TIMER_H
#define MINICHRONOS_TIMER_H

#include "database.h"

namespace MiniChronos
{
    template<typename ChronosType>
    concept IsChronosVariant = requires(ChronosType t)
    {
        t.create_path("");
        t.start(Database::PathId{});
        t.stop(Database::PathId{});
    };

    template<typename ChronosType>
    requires IsChronosVariant<ChronosType>
    class Timer
    {
    public:
        Timer(ChronosType& chronos, std::string&& pathname) : chronos(chronos)
        {
            path_id = chronos.create_path(std::move(pathname));
        }

        void start() { chronos.start(path_id); }
        void stop() { chronos.stop(path_id); }

    private:
        ChronosType& chronos;
        Database::PathId path_id;
    };
}

#endif //MINICHRONOS_TIMER_H
