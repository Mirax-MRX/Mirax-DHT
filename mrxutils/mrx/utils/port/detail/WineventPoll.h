
#pragma once

#include /utils/port/config.h"

#ifdef TD_POLL_WINEVENT

#include /utils/common.h"
#include /utils/port/detail/PollableFd.h"
#include /utils/port/PollBase.h"
#include /utils/port/PollFlags.h"

namespace td {
namespace detail {

class WineventPoll final : public PollBase {
 public:
  WineventPoll() = default;
  WineventPoll(const WineventPoll &) = delete;
  WineventPoll &operator=(const WineventPoll &) = delete;
  WineventPoll(WineventPoll &&) = delete;
  WineventPoll &operator=(WineventPoll &&) = delete;
  ~WineventPoll() override = default;

  void init() override;

  void clear() override;

  void subscribe(PollableFd fd, PollFlags flags) override;

  void unsubscribe(PollableFdRef fd) override;

  void unsubscribe_before_close(PollableFdRef fd) override;

  void run(int timeout_ms) override;

  static bool is_edge_triggered() {
    return true;
  }
};

}  // namespace detail
}  // namespace td

#endif
