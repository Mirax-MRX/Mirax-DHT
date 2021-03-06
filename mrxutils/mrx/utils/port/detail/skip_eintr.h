
#pragma once

#include <cerrno>
#include <type_traits>

namespace td {

#if TD_PORT_POSIX
namespace detail {
template <class F>
auto skip_eintr(F &&f) {
  decltype(f()) res;
  static_assert(std::is_integral<decltype(res)>::value, "integral type expected");
  do {
    errno = 0;  // just in case
    res = f();
  } while (res < 0 && errno == EINTR);
  return res;
}

template <class F>
auto skip_eintr_cstr(F &&f) {
  char *res;
  do {
    errno = 0;  // just in case
    res = f();
  } while (res == nullptr && errno == EINTR);
  return res;
}
}  // namespace detail
#endif

}  // namespace td
