#include <string>
#include <vector>

namespace MiniChronos
{
    // Move this in a private header to keep the interface simple
    class Database
    {
    public:
        bool has_path(const std::string& path);
        std::size_t ensures_key(std::string& key);

    private:
        std::vector<std::string> all_paths;
    };

    void init(Database& db);

    void start(std::string&& timer_id);
}
