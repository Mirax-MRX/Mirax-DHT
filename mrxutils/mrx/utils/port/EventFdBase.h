
#pragma once

#include /utils/common.h"
#include /utils/port/detail/PollableFd.h"
#include /utils/Status.h"

namespace td {
class EventFdBase {
 public:
  EventFdBase() = default;
  EventFdBase(const EventFdBase &) = delete;
  EventFdBase &operator=(const EventFdBase &) = delete;
  EventFdBase(EventFdBase &&) = default;
  EventFdBase &operator=(EventFdBase &&) = default;
  virtual ~EventFdBase() = default;

  virtual void init() = 0;
  virtual bool empty() = 0;
  virtual void close() = 0;
  virtual PollableFdInfo &get_poll_info() = 0;
  virtual Status get_pending_error() TD_WARN_UNUSED_RESULT = 0;
  virtual void release() = 0;
  virtual void acquire() = 0;
  virtual void wait(int timeout_ms) = 0;
};
}  // namespace td
