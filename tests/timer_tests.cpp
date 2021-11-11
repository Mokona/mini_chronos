#include <gmock/gmock.h>

#include "mini_chronos_mock.h"
#include <timer.h>

using namespace testing;

TEST_F(SimpleMiniChronos, timer_can_be_declared)
{
    MiniChronos::Timer t1(chronos, "timer_1");

    ASSERT_TRUE(db.has_path("timer_1"));
    ASSERT_FALSE(db.has_path("timer_2"));
}

TEST_F(SimpleMiniChronos, timer_calls_the_chrono_when_started)
{
    chrono_mock::now_was_called = false;

    MiniChronos::Timer t1(chronos, "timer_1");
    t1.start();
    ASSERT_TRUE(chrono_mock::now_was_called);
}

TEST_F(SimpleMiniChronos, timer_calls_the_chrono_when_stopped)
{
    MiniChronos::Timer t1(chronos, "timer_1");

    t1.start();
    chrono_mock::now_was_called = false;
    t1.stop();
    ASSERT_TRUE(chrono_mock::now_was_called);
}
