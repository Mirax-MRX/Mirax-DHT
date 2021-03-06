
#pragma once

#include /utils/buffer.h"
#include /utils/common.h"
#include /utils/port/detail/PollableFd.h"
#include /utils/port/FileFd.h"
#include /utils/Status.h"

#include <limits>
#include <memory>

namespace td {

FileFd &Stdin();
FileFd &Stdout();
FileFd &Stderr();

namespace detail {
class BufferedStdinImpl;
class BufferedStdinImplDeleter {
 public:
  void operator()(BufferedStdinImpl *impl);
};
}  // namespace detail

class BufferedStdin {
 public:
  BufferedStdin();
  BufferedStdin(const BufferedStdin &) = delete;
  BufferedStdin &operator=(const BufferedStdin &) = delete;
  BufferedStdin(BufferedStdin &&);
  BufferedStdin &operator=(BufferedStdin &&);
  ~BufferedStdin();
  ChainBufferReader &input_buffer();
  PollableFdInfo &get_poll_info();
  const PollableFdInfo &get_poll_info() const;
  Result<size_t> flush_read(size_t max_read = std::numeric_limits<size_t>::max()) TD_WARN_UNUSED_RESULT;

 private:
  std::unique_ptr<detail::BufferedStdinImpl, detail::BufferedStdinImplDeleter> impl_;
};

}  // namespace td
