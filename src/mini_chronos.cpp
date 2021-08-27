#include "mini_chronos.h"

#include <algorithm>
#include <memory>

namespace MiniChronos
{
    class Timer
    {
    public:
        Timer(Database& db) : db(db) {}

        void start(std::string&& timer_id) { db.ensures_key(timer_id); }

    private:
        Database& db;
    };

    std::unique_ptr<Timer> the_one_timer;

    void init(Database& db) { the_one_timer = std::make_unique<Timer>(db); };

    void start(std::string&& timer_id) { the_one_timer->start(std::move(timer_id)); }

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
