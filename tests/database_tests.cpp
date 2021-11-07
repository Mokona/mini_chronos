#include <database.h>

#include <gmock/gmock.h>

using namespace testing;

TEST(Database, has_no_initial_path)
{
    MiniChronos::Database db{};

    std::string pathname{"path_1"};

    ASSERT_THAT(db.has_path(pathname), IsFalse());
}

TEST(Database, registers_path)
{
    MiniChronos::Database db{};

    std::string pathname{"path_1"};
    auto id = db.ensures_path(MiniChronos::Database::no_path, pathname);

    ASSERT_THAT(id, Ne(MiniChronos::Database::no_path));
    ASSERT_THAT(db.has_path(pathname), IsTrue());
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
