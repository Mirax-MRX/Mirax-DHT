
#pragma once

#include /utils/port/config.h"

#ifdef TD_POLL_POLL

#include /utils/common.h"
#include /utils/port/detail/PollableFd.h"
#include /utils/port/PollBase.h"
#include /utils/port/PollFlags.h"

#include <poll.h>

namespace td {
namespace detail {

class Poll final : public PollBase {
 public:
  Poll() = default;
  Poll(const Poll &) = delete;
  Poll &operator=(const Poll &) = delete;
  Poll(Poll &&) = delete;
  Poll &operator=(Poll &&) = delete;
  ~Poll() override = default;

  void init() override;

  void clear() override;

  void subscribe(PollableFd fd, PollFlags flags) override;

  void unsubscribe(PollableFdRef fd) override;

  void unsubscribe_before_close(PollableFdRef fd) override;

  void run(int timeout_ms) override;

  static bool is_edge_triggered() {
    return false;
  }

 private:
  vector<pollfd> pollfds_;
  vector<PollableFd> fds_;
};

}  // namespace detail
}  // namespace td

#endif
