#include <mini_chronos.h>

#include <gmock/gmock.h>

#include <chrono>
#include <database.h>
#include <error_handler.h>

using namespace testing;

struct chrono_stub
{
    typedef std::chrono::nanoseconds duration;
    typedef duration::rep rep;
    typedef duration::period period;
    typedef std::chrono::time_point<chrono_stub, duration> time_point;

    static constexpr time_point now() { return time_point{}; }
    static constexpr bool is_steady = false;
};

TEST(MiniChronos, start_a_timer_references_it_in_the_db)
{
    using namespace MiniChronos;

    ErrorHandler error_handler({.fatal_error_cb = [](std::string&&) {}});
    Database db;
    init<chrono_stub>(db, error_handler);
    start("timer_1");

    ASSERT_TRUE(db.has_path("timer_1"));
    ASSERT_FALSE(db.has_path("timer_2"));

    uninit();
}

TEST(MiniChronos, cannot_be_initialized_twice)
{
    using namespace MiniChronos;
    bool error_called = false;

    ErrorHandler error_handler(
            {.fatal_error_cb = [&error_called](std::string&&) { error_called = true; }});
    Database db;
    init<chrono_stub>(db, error_handler);

    ASSERT_FALSE(error_called);
    init<chrono_stub>(db, error_handler);
    ASSERT_TRUE(error_called);

    uninit();
}

TEST(MiniChronos, cannot_stop_when_not_started)
{
    using namespace MiniChronos;

    bool error_called = false;

    ErrorHandler error_handler(
            {.fatal_error_cb = [&error_called](std::string&&) { error_called = true; }});
    Database db;
    init<chrono_stub>(db, error_handler);

    ASSERT_FALSE(error_called);
    stop();
    ASSERT_TRUE(error_called);

    uninit();
}

struct chrono_mock
{
    typedef std::chrono::nanoseconds duration;
    typedef duration::rep rep;
    typedef duration::period period;
    typedef std::chrono::time_point<chrono_mock, duration> time_point;

    static constexpr time_point now() { return time_point{}; }
    static constexpr bool is_steady = false;
};

TEST(MiniChronos, gets_a_timing_when_timer_started)
{
    using namespace MiniChronos;

    ErrorHandler error_handler({.fatal_error_cb = [](std::string&&) {}});
    Database db;
    init<chrono_mock>(db, error_handler);

    start("timer_1");

    uninit();
}