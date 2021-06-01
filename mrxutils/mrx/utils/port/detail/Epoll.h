
#pragma once

#include /utils/port/config.h"

#ifdef TD_POLL_EPOLL

#include /utils/common.h"
#include /utils/List.h"
#include /utils/port/detail/NativeFd.h"
#include /utils/port/detail/PollableFd.h"
#include /utils/port/PollBase.h"
#include /utils/port/PollFlags.h"

#include <sys/epoll.h>

namespace td {
namespace detail {

class Epoll final : public PollBase {
 public:
  Epoll() = default;
  Epoll(const Epoll &) = delete;
  Epoll &operator=(const Epoll &) = delete;
  Epoll(Epoll &&) = delete;
  Epoll &operator=(Epoll &&) = delete;
  ~Epoll() override = default;

  void init() override;

  void clear() override;

  void subscribe(PollableFd fd, PollFlags flags) override;

  void unsubscribe(PollableFdRef fd) override;

  void unsubscribe_before_close(PollableFdRef fd) override;

  void run(int timeout_ms) override;

  static bool is_edge_triggered() {
    return true;
  }

 private:
  NativeFd epoll_fd_;
  vector<struct epoll_event> events_;
  ListNode list_root_;
};

}  // namespace detail
}  // namespace td

#endif
