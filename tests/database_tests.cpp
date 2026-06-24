#include <database.h>

#include <gmock/gmock.h>

using namespace testing;

TEST(Database, has_no_initial_path)
{
    MiniChronos::Database db{};

    std::string pathname{"path_1"};

    ASSERT_THAT(db.has_path(pathname), IsFalse());
}

TEST(Database, ensures_path_accepts_const_string)
{
    MiniChronos::Database db{};

    const std::string pathname{"path_1"};
    auto id = db.ensures_path(MiniChronos::Database::no_path, pathname);

    ASSERT_THAT(id, Ne(MiniChronos::Database::no_path));
    ASSERT_THAT(db.has_path(pathname), IsTrue());
}

TEST(Database, registers_path)
{
    MiniChronos::Database db{};

    std::string pathname{"path_1"};
    auto id = db.ensures_path(MiniChronos::Database::no_path, pathname);

    ASSERT_THAT(id, Ne(MiniChronos::Database::no_path));
    ASSERT_THAT(db.has_path(pathname), IsTrue());
}

TEST(Database, get_timer_data_throws_on_unknown_path)
{
    MiniChronos::Database db{};

    ASSERT_THROW(db.get_timer_data(std::string{"unknown_path"}), std::out_of_range);
}


TEST(Database, query_methods_are_callable_on_const_database)
{
    // This is mainly to ensure some methods are const

    MiniChronos::Database db{};
    std::string pathname{"path_1"};
    db.ensures_path(MiniChronos::Database::no_path, pathname);

    const MiniChronos::Database& const_db = db;
    ASSERT_TRUE(const_db.has_path(pathname));
    ASSERT_NO_THROW(const_db.get_timer_data(pathname));
}

TEST(Database, registers_path_as_a_hierarchy)
{
    MiniChronos::Database db{};

    std::string root{"path_1"};
    auto id_root = db.ensures_path(MiniChronos::Database::no_path, root);

    std::string pathname{"path_2"};
    auto id = db.ensures_path(id_root, pathname);

    std::string full_path{"path_1::path_2"};

    ASSERT_THAT(id, Ne(MiniChronos::Database::no_path));
    ASSERT_THAT(db.has_path(full_path), IsTrue());
}
