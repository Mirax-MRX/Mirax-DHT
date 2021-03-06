
#pragma once

#include /utils/port/detail/NativeFd.h"
#include /utils/port/detail/PollableFd.h"
#include /utils/port/SocketFd.h"

#include /utils/Slice.h"
#include /utils/Status.h"

#include <memory>

namespace td {
namespace detail {
class ServerSocketFdImpl;
class ServerSocketFdImplDeleter {
 public:
  void operator()(ServerSocketFdImpl *impl);
};
}  // namespace detail

class ServerSocketFd {
 public:
  ServerSocketFd();
  ServerSocketFd(const ServerSocketFd &) = delete;
  ServerSocketFd &operator=(const ServerSocketFd &) = delete;
  ServerSocketFd(ServerSocketFd &&);
  ServerSocketFd &operator=(ServerSocketFd &&);
  ~ServerSocketFd();

  static Result<ServerSocketFd> open(int32 port, CSlice addr = CSlice("0.0.0.0")) TD_WARN_UNUSED_RESULT;

  PollableFdInfo &get_poll_info();
  const PollableFdInfo &get_poll_info() const;

  Status get_pending_error() TD_WARN_UNUSED_RESULT;

  Result<SocketFd> accept() TD_WARN_UNUSED_RESULT;

  void close();
  bool empty() const;

  const NativeFd &get_native_fd() const;

 private:
  std::unique_ptr<detail::ServerSocketFdImpl, detail::ServerSocketFdImplDeleter> impl_;
  explicit ServerSocketFd(unique_ptr<detail::ServerSocketFdImpl> impl);
};
}  // namespace td
