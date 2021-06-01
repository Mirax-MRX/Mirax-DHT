

#include <atomic>

#include /utils/common.h"
#include /utils/port/thread.h"
namespace td {
template <class T>

class AtomicRead {
 public:
  void read(T &dest) const {
    while (true) {
      static_assert(std::is_trivially_copyable<T>::value, "T must be trivially copyable");
      auto version_before = version.load();
      memcpy(&dest, &value, sizeof(dest));
      auto version_after = version.load();
      if (version_before == version_after && version_before % 2 == 0) {
        break;
      }
      td::this_thread::yield();
    }
  }

  friend struct Write;
  struct Write {
    explicit Write(AtomicRead *read) {
      read->do_lock();
      ptr.reset(read);
    }
    struct Destructor {
      void operator()(AtomicRead *read) const {
        read->do_unlock();
      }
    };
    T &operator*() {
      return value();
    }
    T *operator->() {
      return &value();
    }
    T &value() {
      CHECK(ptr);
      return ptr->value;
    }

   private:
    std::unique_ptr<AtomicRead, Destructor> ptr;
  };
  Write lock() {
    return Write(this);
  }

 private:
  std::atomic<td::uint64> version{0};
  T value;

  void do_lock() {
    CHECK(++version % 2 == 1);
  }
  void do_unlock() {
    CHECK(++version % 2 == 0);
  }
};
};  // namespace td
