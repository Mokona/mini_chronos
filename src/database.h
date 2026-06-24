#ifndef MINICHRONOS_DATABASE_H
#define MINICHRONOS_DATABASE_H

#include <chrono>
#include <cstdint>
#include <string>
#include <vector>

namespace MiniChronos
{
    class Database
    {
    public:
        struct TimerData
        {
            std::chrono::nanoseconds duration{};
            uint32_t calls{};
            std::string name{};
        };

        struct TimerIterator
        {
            using iterator_category = std::input_iterator_tag;
            using value_type        = TimerData;
            using difference_type   = std::ptrdiff_t;
            using pointer           = const TimerData*;
            using reference         = TimerData&;
            explicit TimerIterator(Database* db) : associated_db{db}, count{0} {};
            TimerIterator(Database* db, std::size_t count) : associated_db{db}, count(count){};

            TimerIterator end() const
            {
                return TimerIterator{associated_db, associated_db->all_paths.size()};
            }

            TimerIterator& operator++()
            {
                if (count < associated_db->all_paths.size())
                {
                    count += 1;
                }
                return *this;
            };
            TimerIterator operator++(int) { TimerIterator old = *this; ++(*this); return old; };

            bool operator==(TimerIterator other) const
            {
                return count == other.count && associated_db == other.associated_db;
            };

            bool operator!=(TimerIterator other) const
            {
                return count != other.count || associated_db != other.associated_db;
            };

            TimerData operator*() const
            {
                return TimerData{.duration = associated_db->all_durations[count],
                                 .calls = associated_db->all_calls[count],
                                 .name = associated_db->all_paths[count]};
            }

        private:
            Database* associated_db;
            std::size_t count;
        };

        static_assert(std::input_iterator<TimerIterator>);

        using PathId = std::size_t;
        static const PathId no_path = std::numeric_limits<std::size_t>::max();

        bool has_path(const std::string& path) const;
        PathId ensures_path(Database::PathId base_path, const std::string& path);

        void set_duration(Database::PathId, std::chrono::nanoseconds duration);
        void inc_call_count(Database::PathId id);
        Database::TimerData get_timer_data(const std::string& path) const;
        Database::TimerData get_timer_data(Database::PathId path_id) const;

        void reset();

    private:
        std::vector<std::string> all_paths;
        std::vector<uint32_t> all_calls;
        std::vector<std::chrono::nanoseconds> all_durations;

        std::string construct_path(PathId root_id, const std::string& path);
    };

}

#endif //MINICHRONOS_DATABASE_H
