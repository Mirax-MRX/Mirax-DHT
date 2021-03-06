
#include /utils/Enumerator.h"
#include /utils/tests.h"

TEST(Enumerator, simple) {
  td::Enumerator<std::string> e;
  auto b = e.add("b");
  auto a = e.add("a");
  auto d = e.add("d");
  auto c = e.add("c");
  ASSERT_STREQ(e.get(a), "a");
  ASSERT_STREQ(e.get(b), "b");
  ASSERT_STREQ(e.get(c), "c");
  ASSERT_STREQ(e.get(d), "d");
  ASSERT_EQ(a, e.add("a"));
  ASSERT_EQ(b, e.add("b"));
  ASSERT_EQ(c, e.add("c"));
  ASSERT_EQ(d, e.add("d"));
}
