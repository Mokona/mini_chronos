#include "error_handler.h"
#include <string>
#
namespace MiniChronos
{
    class Database;

    void init(Database& db, MiniChronos::ErrorHandler error_handler);
    void uninit();
    void start(std::string&& timer_id);
}
