
#include /utils/tests.h"

#include /utils/buffer.h"
#include /utils/Random.h"

using namespace mrx;

TEST(Buffer, buffer_builder) {
  {
    BufferBuilder builder;
    builder.append("b");
    builder.prepend("a");
    builder.append("c");
    ASSERT_EQ(builder.extract().as_slice(), "abc");
  }
  {
    BufferBuilder builder{"hello", 0, 0};
    ASSERT_EQ(builder.extract().as_slice(), "hello");
  }
  {
    BufferBuilder builder{"hello", 1, 1};
    builder.prepend("A ");
    builder.append(" B");
    ASSERT_EQ(builder.extract().as_slice(), "A hello B");
  }
  {
    std::string str = rand_string('a', 'z', 10000);
    auto splitted_str = rand_split(str);

    int l = Random::fast(0, static_cast<int32>(splitted_str.size() - 1));
    int r = l;
    BufferBuilder builder(splitted_str[l], 123, 1000);
    while (l != 0 || r != static_cast<int32>(splitted_str.size()) - 1) {
      if (l == 0 || (Random::fast(0, 1) == 1 && r != static_cast<int32>(splitted_str.size() - 1))) {
        r++;
        if (Random::fast(0, 1) == 1) {
          builder.append(splitted_str[r]);
        } else {
          builder.append(BufferSlice(splitted_str[r]));
        }
      } else {
        l--;
        if (Random::fast(0, 1) == 1) {
          builder.prepend(splitted_str[l]);
        } else {
          builder.prepend(BufferSlice(splitted_str[l]));
        }
      }
    }
    ASSERT_EQ(builder.extract().as_slice(), str);
  }
}
