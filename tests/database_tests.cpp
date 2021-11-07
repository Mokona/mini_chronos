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
    auto id = db.ensures_path(pathname);

    ASSERT_THAT(id, Ne(MiniChronos::Database::no_path));
    ASSERT_THAT(db.has_path(pathname), IsTrue());
}
