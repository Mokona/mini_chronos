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

    Chronos<chrono_stub> chronos(db, error_handler);
    chronos.start("timer_1");

    ASSERT_TRUE(db.has_path("timer_1"));
    ASSERT_FALSE(db.has_path("timer_2"));
}

TEST(MiniChronos, cannot_stop_when_not_started)
{
    using namespace MiniChronos;

    bool error_called = false;

    ErrorHandler error_handler(
            {.fatal_error_cb = [&error_called](std::string&&) { error_called = true; }});
    Database db;
    Chronos<chrono_stub> chronos(db, error_handler);

    ASSERT_FALSE(error_called);
    chronos.stop();
    ASSERT_TRUE(error_called);
}

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

bool chrono_mock::now_was_called = false;
chrono_mock::duration chrono_mock::fake_duration{0};

TEST(MiniChronos, gets_a_timing_when_timer_started)
{
    using namespace MiniChronos;

    ErrorHandler error_handler({.fatal_error_cb = [](std::string&&) {}});
    Database db;
    Chronos<chrono_mock> chronos(db, error_handler);

    chrono_mock::now_was_called = false;
    chronos.start("timer_1");
    ASSERT_TRUE(chrono_mock::now_was_called);
}

TEST(MiniChronos, gets_a_timing_when_timer_stopped)
{
    using namespace MiniChronos;

    ErrorHandler error_handler({.fatal_error_cb = [](std::string&&) {}});
    Database db;
    Chronos<chrono_mock> chronos(db, error_handler);

    chronos.start("timer_1");
    chrono_mock::now_was_called = false;
    chronos.stop();
    ASSERT_TRUE(chrono_mock::now_was_called);
}

TEST(MiniChronos, gets_data_from_a_timer)
{
    using namespace MiniChronos;

    ErrorHandler error_handler({.fatal_error_cb = [](std::string&&) {}});
    Database db;
    Chronos<chrono_mock> chronos(db, error_handler);

    chronos.start("timer_1");
    chronos.stop();

    auto data = chronos.get_timer_data("timer_1");

    ASSERT_THAT(data.name, Eq("timer_1"));
    ASSERT_THAT(data.duration, Eq(std::chrono::nanoseconds{1}));
}

TEST(MiniChronos, provides_an_iterator_on_one_timer)
{
    using namespace MiniChronos;

    ErrorHandler error_handler({.fatal_error_cb = [](std::string&&) {}});
    Database db;
    Chronos<chrono_mock> chronos(db, error_handler);

    chronos.start("timer_1");
    chronos.stop();

    for (const auto& timer : chronos)
    {
        ASSERT_THAT(timer.name, Eq("timer_1"));
    }
}

TEST(MiniChronos, provides_an_iterator_on_two_timers)
{
    using namespace MiniChronos;

    ErrorHandler error_handler({.fatal_error_cb = [](std::string&&) {}});
    Database db;
    Chronos<chrono_mock> chronos(db, error_handler);

    chronos.start("timer_1");
    chronos.stop();
    chronos.start("timer_2");
    chronos.stop();

    std::vector<Database::TimerData> timers;
    std::copy(std::begin(chronos), std::end(chronos), std::back_inserter(timers));

    ASSERT_THAT(timers.size(), Eq(2));
    ASSERT_THAT(timers[0].name, Eq("timer_1"));
    ASSERT_THAT(timers[0].duration, Eq(std::chrono::nanoseconds{1}));
    ASSERT_THAT(timers[1].name, Eq("timer_2"));
    ASSERT_THAT(timers[1].duration, Eq(std::chrono::nanoseconds{1}));
}
