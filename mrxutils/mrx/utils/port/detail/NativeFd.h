
#pragma once

#include /utils/port/config.h"

#include /utils/common.h"
#include /utils/Status.h"
#include /utils/StringBuilder.h"

namespace td {

class NativeFd {
 public:
#if TD_PORT_POSIX
  using Fd = int;
  using Socket = int;
#elif TD_PORT_WINDOWS
  using Fd = HANDLE;
  using Socket = SOCKET;
#endif
  NativeFd() = default;
  explicit NativeFd(Fd fd);
  NativeFd(Fd fd, bool nolog);
#if TD_PORT_WINDOWS
  explicit NativeFd(Socket socket);
#endif
  NativeFd(const NativeFd &) = delete;
  NativeFd &operator=(const NativeFd &) = delete;
  NativeFd(NativeFd &&other);
  NativeFd &operator=(NativeFd &&other);
  ~NativeFd();

  explicit operator bool() const;

  Fd fd() const;
  Socket socket() const;

  Status set_is_blocking(bool is_blocking) const;

  Status set_is_blocking_unsafe(bool is_blocking) const;  // may drop other Fd flags on non-Windows

  Status duplicate(const NativeFd &to) const;

  void close();
  Fd release();

  Status validate() const;

 private:
  static Fd empty_fd();

  Fd fd_ = empty_fd();
#if TD_PORT_WINDOWS
  bool is_socket_{false};
#endif
};

StringBuilder &operator<<(StringBuilder &sb, const NativeFd &fd);

}  // namespace td
