
#include /utils/port/detail/ThreadIdGuard.h"

#include /utils/common.h"
#include /utils/port/thread_local.h"

#include <mutex>
#include <set>

namespace td {
namespace detail {
class ThreadIdManager {
 public:
  int32 register_thread() {
    std::lock_guard<std::mutex> guard(mutex_);
    if (unused_thread_ids_.empty()) {
      return ++max_thread_id_;
    }
    auto it = unused_thread_ids_.begin();
    auto result = *it;
    unused_thread_ids_.erase(it);
    return result;
  }
  void unregister_thread(int32 thread_id) {
    std::lock_guard<std::mutex> guard(mutex_);
    CHECK(0 < thread_id && thread_id <= max_thread_id_);
    bool is_inserted = unused_thread_ids_.insert(thread_id).second;
    CHECK(is_inserted);
  }

 private:
  std::mutex mutex_;
  std::set<int32> unused_thread_ids_;
  int32 max_thread_id_ = 0;
};
static ThreadIdManager thread_id_manager;

ThreadIdGuard::ThreadIdGuard() {
  thread_id_ = thread_id_manager.register_thread();
  set_thread_id(thread_id_);
}
ThreadIdGuard::~ThreadIdGuard() {
  thread_id_manager.unregister_thread(thread_id_);
  set_thread_id(0);
}
}  // namespace detail
}  // namespace td
