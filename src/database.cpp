#include "database.h"

namespace MiniChronos
{
    bool Database::has_path(const std::string& path)
    {
        return std::find(begin(all_paths), end(all_paths), path) != end(all_paths);
    }
    std::size_t Database::ensures_key(std::string& key)
    {
        auto exists = std::find(begin(all_paths), end(all_paths), key);

        if (exists == end(all_paths))
        {
            all_paths.push_back(key);
            return all_paths.size() - 1;
        }

        return std::distance(begin(all_paths), exists);
    }
}