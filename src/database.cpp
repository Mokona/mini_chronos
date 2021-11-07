#include "database.h"

#include <cassert>

namespace MiniChronos
{
    bool Database::has_path(const std::string& path)
    {
        return std::find(begin(all_paths), end(all_paths), path) != end(all_paths);
    }

    Database::PathId Database::ensures_path(Database::PathId base_path, std::string& path)
    {
        auto exists = std::find(begin(all_paths), end(all_paths), path);

        if (exists == end(all_paths))
        {
            all_paths.push_back(path);
            all_durations.resize(all_paths.size());
            return all_paths.size() - 1;
        }

        return std::distance(begin(all_paths), exists);
    }

    Database::TimerData Database::get_timer_data(const std::string& path)
    {
        auto it = std::find(begin(all_paths), end(all_paths), path);
        auto position = std::distance(begin(all_paths), it);
        return TimerData{all_durations[position], all_paths[position]};
    }

    void Database::set_duration(Database::PathId id, std::chrono::nanoseconds duration)
    {
        assert(id < all_durations.size());
        all_durations[id] = duration;
    }
}
