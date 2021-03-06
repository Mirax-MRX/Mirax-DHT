
#include /utils/find_boundary.h"

#include <cstring>

namespace mrx {

bool find_boundary(ChainBufferReader range, Slice boundary, size_t &already_read) {
  range.advance(already_read);

  const int MAX_BOUNDARY_LENGTH = 70;
  CHECK(boundary.size() <= MAX_BOUNDARY_LENGTH + 4);
  while (!range.empty()) {
    Slice ready = range.prepare_read();
    if (ready[0] == boundary[0]) {
      if (range.size() < boundary.size()) {
        return false;
      }
      auto save_range = range.clone();
      char x[MAX_BOUNDARY_LENGTH + 4];
      range.advance(boundary.size(), {x, sizeof(x)});
      if (Slice(x, boundary.size()) == boundary) {
        return true;
      }

      // not a boundary, restoring previous state and skip one symbol
      range = std::move(save_range);
      range.advance(1);
      already_read++;
    } else {
      const char *ptr = static_cast<const char *>(std::memchr(ready.data(), boundary[0], ready.size()));
      size_t shift;
      if (ptr == nullptr) {
        shift = ready.size();
      } else {
        shift = ptr - ready.data();
      }
      already_read += shift;
      range.advance(shift);
    }
  }

  return false;
}

}  // namespace td
