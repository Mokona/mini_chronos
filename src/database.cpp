#include "database.h"

#include <cassert>
#include <stdexcept>

namespace MiniChronos
{
    bool Database::has_path(const std::string& path) const
    {
        return std::ranges::find(all_paths, path) != end(all_paths);
    }

    Database::PathId Database::ensures_path(Database::PathId base_path, const std::string& path)
    {
        const auto full_path = construct_path(base_path, path);
        const auto exists = std::ranges::find(all_paths, full_path);

        if (exists == end(all_paths))
        {
            all_paths.push_back(full_path);
            all_durations.resize(all_paths.size());
            all_calls.resize(all_paths.size());
            return all_paths.size() - 1;
        }

        return std::distance(begin(all_paths), exists);
    }

    Database::TimerData Database::get_timer_data(const std::string& path) const
    {
        const auto it = std::ranges::find(all_paths, path);
        if (it == end(all_paths))
        {
            throw std::out_of_range("get_timer_data: unknown path '" + path + "'");
        }
        auto position = std::distance(begin(all_paths), it);
        return TimerData{all_durations[position], all_calls[position], all_paths[position]};
    }

    Database::TimerData Database::get_timer_data(Database::PathId path_id) const
    {
        // Hot path: called on every stop(). Assert only — PathId validity is
        // guaranteed by Chronos which never passes an unregistered id.
        assert(path_id < all_durations.size());
        return TimerData{all_durations[path_id], all_calls[path_id], all_paths[path_id]};
    }

    void Database::set_duration(Database::PathId id, std::chrono::nanoseconds duration)
    {
        // Hot path: called on every stop(). Assert only — see get_timer_data(PathId).
        assert(id < all_durations.size());
        all_durations[id] = duration;
    }

    void Database::inc_call_count(Database::PathId id)
    {
        // Hot path: called on every stop(). Assert only — see get_timer_data(PathId).
        assert(id < all_calls.size());
        all_calls[id] += 1;
    }

    std::string Database::construct_path(PathId root_id, const std::string& path) const
    {
        if (root_id == no_path)
        {
            return path;
        }
        assert(root_id < all_paths.size());
        return all_paths[root_id] + "::" + path;
    }

    void Database::reset()
    {
        std::ranges::fill(all_durations, std::chrono::nanoseconds{});
        std::ranges::fill(all_calls, 0);
    }
}
