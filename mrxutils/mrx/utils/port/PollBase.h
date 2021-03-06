
#pragma once

#include /utils/port/detail/PollableFd.h"
#include /utils/port/PollFlags.h"

namespace td {
class PollBase {
 public:
  PollBase() = default;
  PollBase(const PollBase &) = delete;
  PollBase &operator=(const PollBase &) = delete;
  PollBase(PollBase &&) = default;
  PollBase &operator=(PollBase &&) = default;
  virtual ~PollBase() = default;
  virtual void init() = 0;
  virtual void clear() = 0;
  virtual void subscribe(PollableFd fd, PollFlags flags) = 0;
  virtual void unsubscribe(PollableFdRef fd) = 0;
  virtual void unsubscribe_before_close(PollableFdRef fd) = 0;
  virtual void run(int timeout_ms) = 0;
};
}  // namespace td
