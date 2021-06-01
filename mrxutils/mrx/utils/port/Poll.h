
#pragma once

#include /utils/port/config.h"

#include /utils/port/detail/Epoll.h"
#include /utils/port/detail/KQueue.h"
#include /utils/port/detail/Poll.h"
#include /utils/port/detail/Select.h"
#include /utils/port/detail/WineventPoll.h"

namespace td {

// clang-format off

#if TD_POLL_EPOLL
  using Poll = detail::Epoll;
#elif TD_POLL_KQUEUE
  using Poll = detail::KQueue;
#elif TD_POLL_WINEVENT
  using Poll = detail::WineventPoll;
#elif TD_POLL_POLL
  using Poll = detail::Poll;
#elif TD_POLL_SELECT
  using Poll = detail::Select;
#endif

// clang-format on

}  // namespace td
