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
        auto full_path = construct_path(base_path, path);
        auto exists = std::find(begin(all_paths), end(all_paths), full_path);

        if (exists == end(all_paths))
        {
            all_paths.push_back(full_path);
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

    Database::TimerData Database::get_timer_data(Database::PathId path_id)
    {
        assert(path_id < all_durations.size());
        return TimerData{all_durations[path_id], all_paths[path_id]};
    }


    void Database::set_duration(Database::PathId id, std::chrono::nanoseconds duration)
    {
        assert(id < all_durations.size());
        all_durations[id] = duration;
    }

    std::string Database::construct_path(PathId root_id, std::string& path)
    {
        if (root_id == no_path)
        {
            return path;
        }
        assert(root_id < all_paths.size());
        return all_paths[root_id] + "::" + path;
    }
}
