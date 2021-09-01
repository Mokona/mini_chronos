#include "database.h"

namespace MiniChronos
{
    bool Database::has_path(const std::string& path)
    {
        return std::find(begin(all_paths), end(all_paths), path) != end(all_paths);
    }
    Database::PathId Database::ensures_path(std::string& path)
    {
        auto exists = std::find(begin(all_paths), end(all_paths), path);

        if (exists == end(all_paths))
        {
            all_paths.push_back(path);
            return all_paths.size() - 1;
        }

        return std::distance(begin(all_paths), exists);
    }

    Database::TimerData Database::get_timer_data()
    {
        return TimerData{std::chrono::nanoseconds{latest_duration}};
    }

    void Database::set_duration(Database::PathId, std::chrono::nanoseconds duration)
    {
        latest_duration = duration;
    }
}