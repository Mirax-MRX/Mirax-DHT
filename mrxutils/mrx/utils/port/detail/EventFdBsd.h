
#pragma once

#include /utils/port/config.h"

#ifdef TD_EVENTFD_BSD

#include /utils/common.h"
#include /utils/port/detail/PollableFd.h"
#include /utils/port/EventFdBase.h"
#include /utils/port/SocketFd.h"
#include /utils/Status.h"

namespace td {
namespace detail {

class EventFdBsd final : public EventFdBase {
  SocketFd in_;
  SocketFd out_;

 public:
  EventFdBsd() = default;

  void init() override;

  bool empty() override;

  void close() override;

  Status get_pending_error() override TD_WARN_UNUSED_RESULT;

  PollableFdInfo &get_poll_info() override;

  void release() override;

  void acquire() override;

  void wait(int timeout_ms) override;
};

}  // namespace detail
}  // namespace td

#endif
