
#pragma once

#include /utils/port/config.h"

#ifdef TD_THREAD_PTHREAD

#include /utils/common.h"
#include /utils/Destructor.h"
#include /utils/invoke.h"
#include /utils/MovableValue.h"
#include /utils/port/detail/ThreadIdGuard.h"
#include /utils/port/thread_local.h"
#include /utils/Slice.h"

#include <tuple>
#include <type_traits>
#include <utility>

#include <sys/types.h>

namespace td {
namespace detail {
class ThreadPthread {
 public:
  ThreadPthread() = default;
  ThreadPthread(const ThreadPthread &other) = delete;
  ThreadPthread &operator=(const ThreadPthread &other) = delete;
  ThreadPthread(ThreadPthread &&other) noexcept : is_inited_(std::move(other.is_inited_)), thread_(other.thread_) {
  }
  ThreadPthread &operator=(ThreadPthread &&other) {
    join();
    is_inited_ = std::move(other.is_inited_);
    thread_ = other.thread_;
    return *this;
  }
  template <class Function, class... Args>
  explicit ThreadPthread(Function &&f, Args &&... args) {
    auto func = create_destructor([args = std::make_tuple(decay_copy(std::forward<Function>(f)),
                                                          decay_copy(std::forward<Args>(args))...)]() mutable {
      invoke_tuple(std::move(args));
      clear_thread_locals();
    });
    do_pthread_create(&thread_, nullptr, run_thread, func.release());
    is_inited_ = true;
  }
  ~ThreadPthread() {
    join();
  }

  void set_name(CSlice name);

  void join();

  void detach();

  static unsigned hardware_concurrency();

  using id = pthread_t;

 private:
  MovableValue<bool> is_inited_;
  pthread_t thread_;

  template <class T>
  std::decay_t<T> decay_copy(T &&v) {
    return std::forward<T>(v);
  }

  int do_pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg);

  static void *run_thread(void *ptr) {
    ThreadIdGuard thread_id_guard;
    auto func = unique_ptr<Destructor>(static_cast<Destructor *>(ptr));
    return nullptr;
  }
};

namespace this_thread_pthread {
void yield();
ThreadPthread::id get_id();
}  // namespace this_thread_pthread
}  // namespace detail
}  // namespace td

#endif
