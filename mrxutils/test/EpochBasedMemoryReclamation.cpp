
#include /utils/common.h"
#include /utils/EpochBasedMemoryReclamation.h"
#include /utils/logging.h"
#include /utils/port/thread.h"
#include /utils/Random.h"
#include /utils/tests.h"

#include <atomic>

#if !TD_THREAD_UNSUPPORTED
TEST(EpochBaseMemoryReclamation, stress) {
  struct Node {
    std::atomic<std::string *> name_{nullptr};
    char pad[64];
  };

  int threads_n = 10;
  std::vector<Node> nodes(threads_n);
  td::EpochBasedMemoryReclamation<std::string> ebmr(threads_n + 1);
  auto locker = ebmr.get_locker(threads_n);
  locker.lock();
  locker.unlock();
  std::vector<td::thread> threads(threads_n);
  int thread_id = 0;
  for (auto &thread : threads) {
    thread = td::thread([&, thread_id] {
      auto locker = ebmr.get_locker(thread_id);
      locker.lock();
      for (int i = 0; i < 1000000; i++) {
        auto &node = nodes[td::Random::fast(0, threads_n - 1)];
        auto *str = node.name_.load(std::memory_order_acquire);
        if (str) {
          CHECK(*str == "one" || *str == "twotwo");
        }
        if ((i + 1) % 100 == 0) {
          locker.retire();
        }
        if (td::Random::fast(0, 5) == 0) {
          std::string *new_str = new std::string(td::Random::fast(0, 1) == 0 ? "one" : "twotwo");
          if (node.name_.compare_exchange_strong(str, new_str, std::memory_order_acq_rel)) {
            locker.retire(str);
          } else {
            delete new_str;
          }
        }
      }
      locker.retire_sync();
      locker.unlock();
    });
    thread_id++;
  }
  for (auto &thread : threads) {
    thread.join();
  }
  LOG(ERROR) << "Undeleted pointers: " << ebmr.to_delete_size_unsafe();
  //CHECK(static_cast<int>(ebmr.to_delete_size_unsafe()) <= threads_n * threads_n);
  for (int i = 0; i < threads_n; i++) {
    ebmr.get_locker(i).retire_sync();
  }
  CHECK(ebmr.to_delete_size_unsafe() == 0);
}
#endif  //!TD_THREAD_UNSUPPORTED
