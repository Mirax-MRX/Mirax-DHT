

#include /utils/SharedSlice.h"

#include /utils/buffer.h"

namespace td {

BufferSlice SharedSlice::clone_as_buffer_slice() const {
  return BufferSlice{as_slice()};
}

}  // namespace td
