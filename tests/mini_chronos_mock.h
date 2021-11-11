#ifndef MINICHRONOS_MINI_CHRONOS_MOCK_H
#define MINICHRONOS_MINI_CHRONOS_MOCK_H

#include <chrono>
#include <gmock/gmock.h>

#include <mini_chronos.h>

struct chrono_mock
{
    typedef std::chrono::nanoseconds duration;
    typedef duration::rep rep;
    typedef duration::period period;
    typedef std::chrono::time_point<chrono_mock, duration> time_point;

    static time_point now()
    {
        now_was_called = true;
        fake_duration += duration{1};
        return time_point{fake_duration};
    }
    static constexpr bool is_steady = false;

    static bool now_was_called;
    static duration fake_duration;
};

struct SimpleMiniChronos : public testing::Test
{
protected:
public:
    SimpleMiniChronos()
        : error_handler{{.fatal_error_cb = [](std::string&&) {}}}, chronos{db, error_handler}
    {
        chrono_mock::now_was_called = false;
        chrono_mock::fake_duration = chrono_mock::duration{0};
    }

protected:
    MiniChronos::ErrorHandler error_handler;
    MiniChronos::Database db;
    MiniChronos::Chronos<chrono_mock> chronos;
};

#endif //MINICHRONOS_MINI_CHRONOS_MOCK_H
