
#include /utils/port/detail/WineventPoll.h"

char disable_linker_warning_about_empty_file_wineventpoll_cpp TD_UNUSED;

#ifdef TD_POLL_WINEVENT

#include /utils/common.h"

namespace td {
namespace detail {

void WineventPoll::init() {
}

void WineventPoll::clear() {
}

void WineventPoll::subscribe(PollableFd fd, PollFlags flags) {
  fd.release_as_list_node();
}

void WineventPoll::unsubscribe(PollableFdRef fd) {
  auto pollable_fd = fd.lock();  // unlocked in destructor
}

void WineventPoll::unsubscribe_before_close(PollableFdRef fd) {
  unsubscribe(std::move(fd));
}

void WineventPoll::run(int timeout_ms) {
  UNREACHABLE();
}

}  // namespace detail
}  // namespace td

#endif
