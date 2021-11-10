#include <mini_chronos.h>

#include <gmock/gmock.h>

#include <chrono>
#include <database.h>
#include <error_handler.h>

using namespace testing;

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

struct SimpleMiniChronos : public testing::Test
{
protected:
public:
    SimpleMiniChronos()
        : error_handler{{.fatal_error_cb = [](std::string&&) {}}}, chronos{db, error_handler}
    {}

protected:
    MiniChronos::ErrorHandler error_handler;
    MiniChronos::Database db;
    MiniChronos::Chronos<chrono_mock> chronos;
};

TEST_F(SimpleMiniChronos, start_a_timer_references_it_in_the_db)
{
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
    Chronos<chrono_mock> chronos(db, error_handler);

    ASSERT_FALSE(error_called);
    chronos.stop();
    ASSERT_TRUE(error_called);
}

TEST_F(SimpleMiniChronos, gets_a_timing_when_timer_started)
{
    chrono_mock::now_was_called = false;
    chronos.start("timer_1");
    ASSERT_TRUE(chrono_mock::now_was_called);
}

TEST_F(SimpleMiniChronos, gets_a_timing_when_timer_stopped)
{
    chronos.start("timer_1");
    chrono_mock::now_was_called = false;
    chronos.stop();
    ASSERT_TRUE(chrono_mock::now_was_called);
}

TEST_F(SimpleMiniChronos, gets_data_from_a_timer)
{
    chronos.start("timer_1");
    chronos.stop();

    auto data = chronos.get_timer_data("timer_1");

    ASSERT_THAT(data.name, Eq("timer_1"));
    ASSERT_THAT(data.duration, Eq(std::chrono::nanoseconds{1}));
}

TEST_F(SimpleMiniChronos, provides_an_iterator_on_one_timer)
{
    chronos.start("timer_1");
    chronos.stop();

    for (const auto& timer : chronos)
    {
        ASSERT_THAT(timer.name, Eq("timer_1"));
    }
}

TEST_F(SimpleMiniChronos, provides_an_iterator_on_two_timers)
{
    chronos.start("timer_1");
    chronos.stop();
    chronos.start("timer_2");
    chronos.stop();

    std::vector<MiniChronos::Database::TimerData> timers;
    std::copy(std::begin(chronos), std::end(chronos), std::back_inserter(timers));

    ASSERT_THAT(timers.size(), Eq(2));
    ASSERT_THAT(timers[0].name, Eq("timer_1"));
    ASSERT_THAT(timers[0].duration, Eq(std::chrono::nanoseconds{1}));
    ASSERT_THAT(timers[1].name, Eq("timer_2"));
    ASSERT_THAT(timers[1].duration, Eq(std::chrono::nanoseconds{1}));
}

TEST_F(SimpleMiniChronos, creates_a_hierarchy_of_timers)
{
    chronos.start("timer_1");
    chronos.start("timer_2");
    chronos.stop();
    chronos.stop();

    std::vector<MiniChronos::Database::TimerData> timers;
    std::copy(std::begin(chronos), std::end(chronos), std::back_inserter(timers));

    ASSERT_THAT(timers.size(), Eq(2));

    // now() on the mock timer will be called 3 times.
    ASSERT_THAT(timers[0].name, Eq("timer_1"));
    ASSERT_THAT(timers[0].duration, Eq(std::chrono::nanoseconds{3}));

    // now() on the mock timer will be called 1 time.
    ASSERT_THAT(timers[1].name, Eq("timer_1::timer_2"));
    ASSERT_THAT(timers[1].duration, Eq(std::chrono::nanoseconds{1}));
}

TEST_F(SimpleMiniChronos, accumulates_time_from_calls)
{
    chronos.start("timer_1");
    chronos.stop();
    chronos.start("timer_1");
    chronos.stop();

    std::vector<MiniChronos::Database::TimerData> timers;
    std::copy(std::begin(chronos), std::end(chronos), std::back_inserter(timers));

    ASSERT_THAT(timers.size(), Eq(1));
    ASSERT_THAT(timers[0].name, Eq("timer_1"));
    ASSERT_THAT(timers[0].duration, Eq(std::chrono::nanoseconds{2}));
}

TEST_F(SimpleMiniChronos, count_number_of_calls)
{
    chronos.start("timer_1");
    chronos.stop();

    std::vector<MiniChronos::Database::TimerData> timers;
    std::copy(std::begin(chronos), std::end(chronos), std::back_inserter(timers));

    ASSERT_THAT(timers.size(), Eq(1));
    ASSERT_THAT(timers[0].name, Eq("timer_1"));
    ASSERT_THAT(timers[0].calls, Eq(1));
}

TEST_F(SimpleMiniChronos, can_reset_calls_and_time_accumulation)
{
    chronos.start("timer_1");
    chronos.stop();

    chronos.reset();

    chronos.start("timer_1");
    chronos.stop();
    chronos.start("timer_1");
    chronos.stop();

    std::vector<MiniChronos::Database::TimerData> timers;
    std::copy(std::begin(chronos), std::end(chronos), std::back_inserter(timers));

    ASSERT_THAT(timers.size(), Eq(1));
    ASSERT_THAT(timers[0].name, Eq("timer_1"));
    ASSERT_THAT(timers[0].calls, Eq(2));
}
