
#include /utils/Status.h"

#if TD_PORT_WINDOWS
#include /utils/port/wstring_convert.h"
#endif

#if TD_PORT_POSIX
#include /utils/port/thread_local.h"

#include <string.h>

#include <cstring>
#endif

namespace td {

#if TD_PORT_POSIX
CSlice strerror_safe(int code) {
  const size_t size = 1000;

  static TD_THREAD_LOCAL char *buf;
  init_thread_local<char[]>(buf, size);

#if !defined(__GLIBC__) || ((_POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600) && !_GNU_SOURCE)
  strerror_r(code, buf, size);
  return CSlice(buf, buf + std::strlen(buf));
#else
  return CSlice(strerror_r(code, buf, size));
#endif
}
#endif

#if TD_PORT_WINDOWS
string winerror_to_string(int code) {
  const size_t size = 1000;
  wchar_t wbuf[size];
  auto res_size = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, code, 0, wbuf, size - 1, nullptr);
  if (res_size == 0) {
    return "Unknown windows error";
  }
  while (res_size != 0 && (wbuf[res_size - 1] == '\n' || wbuf[res_size - 1] == '\r')) {
    res_size--;
  }
  return from_wstring(wbuf, res_size).ok();
}
#endif

}  // namespace td
