#include <string>
#
namespace MiniChronos
{
    class Database;

    void init(Database& db);
    void start(std::string&& timer_id);
}
