
#pragma once

#include /utils/port/config.h"

// include all and let config.h decide
#include /utils/port/detail/EventFdBsd.h"
#include /utils/port/detail/EventFdLinux.h"
#include /utils/port/detail/EventFdWindows.h"

namespace td {

// clang-format off

#if TD_EVENTFD_LINUX
  using EventFd = detail::EventFdLinux;
#elif TD_EVENTFD_BSD
  using EventFd = detail::EventFdBsd;
#elif TD_EVENTFD_WINDOWS
  using EventFd = detail::EventFdWindows;
#elif TD_EVENTFD_UNSUPPORTED
#else
  #error "EventFd's implementation is not defined"
#endif

// clang-format on

}  // namespace td
