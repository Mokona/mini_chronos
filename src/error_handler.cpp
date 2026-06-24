#include "error_handler.h"

#include <utility>

namespace MiniChronos
{
    ErrorHandler::ErrorHandler(Init init) : handlers(std::move(init)) {}

    void ErrorHandler::fatal(std::string reason) const
    {
        handlers.fatal_error_cb(std::move(reason));
    }

}
