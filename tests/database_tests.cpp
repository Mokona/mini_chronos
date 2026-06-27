#include <database.h>

#include <gmock/gmock.h>

using namespace testing;

TEST(Database, has_no_initial_path)
{
    const MiniChronos::Database db{};

    const std::string pathname{"path_1"};

    ASSERT_THAT(db.has_path(pathname), IsFalse());
}

TEST(Database, ensures_path_accepts_const_string)
{
    MiniChronos::Database db{};

    const std::string pathname{"path_1"};
    const auto id = db.ensures_path(pathname);

    ASSERT_THAT(id, Ne(MiniChronos::Database::no_path));
    ASSERT_THAT(db.has_path(pathname), IsTrue());
}

TEST(Database, registers_path)
{
    MiniChronos::Database db{};

    const std::string pathname{"path_1"};
    const auto id = db.ensures_path(pathname);

    ASSERT_THAT(id, Ne(MiniChronos::Database::no_path));
    ASSERT_THAT(db.has_path(pathname), IsTrue());
    ASSERT_THAT(db.is_path_valid(id), IsTrue());
}

TEST(Database, get_timer_data_throws_on_unknown_path)
{
    const MiniChronos::Database db{};

    ASSERT_THROW(static_cast<void>(db.get_timer_data("unknown_path")), std::out_of_range);
}

TEST(Database, query_methods_are_callable_on_const_database)
{
    MiniChronos::Database db{};
    const std::string pathname{"path_1"};
    db.ensures_path(pathname);

    const MiniChronos::Database& const_db = db;
    ASSERT_TRUE(const_db.has_path(pathname));
    ASSERT_NO_THROW(static_cast<void>(const_db.get_timer_data(pathname)));
}
