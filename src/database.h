#ifndef MINICHRONOS_DATABASE_H
#define MINICHRONOS_DATABASE_H

#include <chrono>
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
            std::string name{"timer_1"};
        };

        struct TimerIterator : public std::iterator<std::input_iterator_tag, TimerData, size_t,
                                                    const TimerData*, TimerData&>
        {
            TimerIterator() = default;
            explicit TimerIterator(bool is_end) : is_end(is_end){};

            TimerIterator& operator++()
            {
                is_end = true;
                return *this;
            };
            TimerIterator operator++(int) { return TimerIterator{true}; };
            bool operator==(TimerIterator other) const { return is_end == other.is_end; };
            bool operator!=(TimerIterator other) const { return is_end != other.is_end; };

            TimerData operator*() const { return {}; }

        private:
            bool is_end{};
        };

        bool has_path(const std::string& path);
        std::size_t ensures_key(std::string& key);

    private:
        std::vector<std::string> all_paths;
    };

}

#endif //MINICHRONOS_DATABASE_H
