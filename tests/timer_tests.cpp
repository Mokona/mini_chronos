#include <mini_chronos.h>

#include <gmock/gmock.h>

#include "mini_chronos_mock.h"
#include <database.h>

using namespace testing;

namespace MiniChronos
{
    class Timer
    {
    public:
        Timer(Chronos<chrono_mock> chronos, std::string&& pathname)
        {
            chronos.create_path(std::move(pathname));
        }
    };
}

TEST_F(SimpleMiniChronos, declare_a_time_point)
{
    MiniChronos::Timer t1(chronos, "timer_1");

    ASSERT_TRUE(db.has_path("timer_1"));
    ASSERT_FALSE(db.has_path("timer_2"));
}

/*
 static auto t1 = chronos.create_time_point("time_1");

 t1.start()
 t1.stop()

 */