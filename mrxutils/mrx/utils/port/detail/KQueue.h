
#pragma once

#include /utils/port/config.h"

#ifdef TD_POLL_KQUEUE

#include /utils/common.h"
#include /utils/List.h"
#include /utils/port/detail/NativeFd.h"
#include /utils/port/detail/PollableFd.h"
#include /utils/port/PollBase.h"
#include /utils/port/PollFlags.h"

#include <cstdint>

#include <sys/types.h>  // must be included before sys/event.h, which depends on sys/types.h on FreeBSD

#include <sys/event.h>

namespace td {
namespace detail {

class KQueue final : public PollBase {
 public:
  KQueue() = default;
  KQueue(const KQueue &) = delete;
  KQueue &operator=(const KQueue &) = delete;
  KQueue(KQueue &&) = delete;
  KQueue &operator=(KQueue &&) = delete;
  ~KQueue() override;

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
  vector<struct kevent> events_;
  int changes_n_;
  NativeFd kq_;
  ListNode list_root_;

  int update(int nevents, const timespec *timeout, bool may_fail = false);

  void invalidate(int native_fd);

  void flush_changes(bool may_fail = false);

  void add_change(std::uintptr_t ident, int16 filter, uint16 flags, uint32 fflags, std::intptr_t data, void *udata);
};

}  // namespace detail
}  // namespace td

#endif
