#ifndef MINICHRONOS_DATABASE_H
#define MINICHRONOS_DATABASE_H

#include <string>
#include <vector>

namespace MiniChronos
{
    class Database
    {
    public:
        bool has_path(const std::string& path);
        std::size_t ensures_key(std::string& key);

    private:
        std::vector<std::string> all_paths;
    };

}

#endif //MINICHRONOS_DATABASE_H
