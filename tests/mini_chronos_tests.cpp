#include <mini_chronos.h>

#include <database.h>

#include <gmock/gmock.h>

using namespace testing;

TEST(MiniChronos, start_a_timer_references_it_in_the_db)
{
    MiniChronos::Database db;
    MiniChronos::init(db);
    MiniChronos::start("timer_1");

    ASSERT_TRUE(db.has_path("timer_1"));
    ASSERT_FALSE(db.has_path("timer_2"));
}

// Cannot be initialized twice