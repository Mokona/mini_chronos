#include <database.h>

#include "mini_chronos_mock.h"

using namespace testing;


TEST_F(SimpleMiniChronos, create_path_accepts_lvalue_string)
{
    const std::string name{"timer_1"};
    chronos.create_path(name);

    ASSERT_TRUE(db.has_path("timer_1"));
}

TEST_F(SimpleMiniChronos, create_path_references_it_in_the_db)
{
    chronos.create_path("timer_1");

    ASSERT_TRUE(db.has_path("timer_1"));
    ASSERT_FALSE(db.has_path("timer_2"));
}

TEST_F(SimpleMiniChronos, gets_a_timing_when_timer_started)
{
    const std::size_t t1 = chronos.create_path("timer_1");
    chronos.start(t1);
    ASSERT_TRUE(chrono_mock::now_was_called);
}

TEST_F(SimpleMiniChronos, mock_clock_resets_between_tests)
{
    ASSERT_THAT(chrono_mock::fake_duration.count(), Eq(0));
}

TEST_F(SimpleMiniChronos, gets_a_timing_when_timer_stopped)
{
    const auto t1 = chronos.create_path("timer_1");
    chronos.start(t1);
    chrono_mock::now_was_called = false;
    chronos.stop(t1);
    ASSERT_TRUE(chrono_mock::now_was_called);
}

TEST_F(SimpleMiniChronos, gets_data_from_a_timer)
{
    const auto t1 = chronos.create_path("timer_1");
    chronos.start(t1);
    chronos.stop(t1);

    const auto data = chronos.get_timer_data("timer_1");

    ASSERT_THAT(data.name, Eq("timer_1"));
    ASSERT_THAT(data.duration, Eq(std::chrono::nanoseconds{1}));
}

TEST_F(SimpleMiniChronos, gets_call_count_from_a_timer)
{
    const auto t1 = chronos.create_path("timer_1");
    chronos.start(t1);
    chronos.stop(t1);

    const auto data = chronos.get_timer_data("timer_1");

    ASSERT_THAT(data.calls, Eq(1));
}

TEST_F(SimpleMiniChronos, end_method_does_not_mutate_iterator)
{
    const auto t1 = chronos.create_path("timer_1");
    chronos.start(t1);
    chronos.stop(t1);

    auto it = chronos.begin();
    static_cast<void>(it.end());

    ASSERT_TRUE(it == chronos.begin());
}

TEST_F(SimpleMiniChronos, provides_an_iterator_on_one_timer)
{
    const auto t1 = chronos.create_path("timer_1");
    chronos.start(t1);
    chronos.stop(t1);

    for (const auto& timer : chronos)
    {
        ASSERT_THAT(timer.name, Eq("timer_1"));
    }
}

TEST_F(SimpleMiniChronos, post_increment_advances_iterator)
{
    const auto t1 = chronos.create_path("timer_1");
    chronos.start(t1);
    chronos.stop(t1);
    const auto t2 = chronos.create_path("timer_2");
    chronos.start(t2);
    chronos.stop(t2);

    auto it = chronos.begin();
    static_cast<void>(it++);

    ASSERT_THAT((*it).name, Eq("timer_2"));
}

TEST_F(SimpleMiniChronos, provides_an_iterator_on_two_timers)
{
    const auto t1 = chronos.create_path("timer_1");
    chronos.start(t1);
    chronos.stop(t1);
    const auto t2 = chronos.create_path("timer_2");
    chronos.start(t2);
    chronos.stop(t2);

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
    const auto t1 = chronos.create_path("timer_1");
    const auto t2 = chronos.create_path("timer_1::timer_2");

    chronos.start(t1);
    chronos.start(t2);
    chronos.stop(t2);
    chronos.stop(t1);

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
    const auto t1 = chronos.create_path("timer_1");
    chronos.start(t1);
    chronos.stop(t1);
    chronos.start(t1);
    chronos.stop(t1);

    std::vector<MiniChronos::Database::TimerData> timers;
    std::copy(std::begin(chronos), std::end(chronos), std::back_inserter(timers));

    ASSERT_THAT(timers.size(), Eq(1));
    ASSERT_THAT(timers[0].name, Eq("timer_1"));
    ASSERT_THAT(timers[0].duration, Eq(std::chrono::nanoseconds{2}));
}

TEST_F(SimpleMiniChronos, count_number_of_calls)
{
    const auto t1 = chronos.create_path("timer_1");
    chronos.start(t1);
    chronos.stop(t1);

    std::vector<MiniChronos::Database::TimerData> timers;
    std::copy(std::begin(chronos), std::end(chronos), std::back_inserter(timers));

    ASSERT_THAT(timers.size(), Eq(1));
    ASSERT_THAT(timers[0].name, Eq("timer_1"));
    ASSERT_THAT(timers[0].calls, Eq(1));
}

TEST_F(SimpleMiniChronos, can_reset_calls_and_time_accumulation)
{
    const auto t1 = chronos.create_path("timer_1");
    chronos.start(t1);
    chronos.stop(t1);

    chronos.reset();

    chronos.start(t1);
    chronos.stop(t1);
    chronos.start(t1);
    chronos.stop(t1);

    std::vector<MiniChronos::Database::TimerData> timers;
    std::copy(std::begin(chronos), std::end(chronos), std::back_inserter(timers));

    ASSERT_THAT(timers.size(), Eq(1));
    ASSERT_THAT(timers[0].name, Eq("timer_1"));
    ASSERT_THAT(timers[0].calls, Eq(2));
}
