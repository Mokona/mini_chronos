#ifndef MINICHRONOS_ERRORHANDLER_H
#define MINICHRONOS_ERRORHANDLER_H

#include <functional>
#include <string>

namespace MiniChronos
{
    class ErrorHandler
    {
    public:
        struct Init
        {
            std::function<void(std::string&&)> fatal_error_cb;
        };

        explicit ErrorHandler(Init init);

        void fatal(std::string && reason);

    private:
        Init handlers;
    };
}

#endif //MINICHRONOS_ERRORHANDLER_H
