#include <mini_chronos.h>

#include <database.h>

#include <gmock/gmock.h>

#include <error_handler.h>

using namespace testing;

TEST(MiniChronos, start_a_timer_references_it_in_the_db)
{
    using namespace MiniChronos;

    ErrorHandler error_handler({.fatal_error_cb = [](std::string&&) {}});
    Database db;
    init(db, error_handler);
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
    init(db, error_handler);

    ASSERT_FALSE(error_called);
    init(db, error_handler);
    ASSERT_TRUE(error_called);

    uninit();
}
