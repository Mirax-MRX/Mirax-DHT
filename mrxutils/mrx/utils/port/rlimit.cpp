

#include /utils/port/rlimit.h"

#include /utils/port/config.h"

#include /utils/misc.h"

#if TD_PORT_POSIX
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/types.h>
#endif

namespace td {

#if TD_PORT_POSIX

namespace {

int get_rlimit_type(RlimitType rlim_type) {
  switch (rlim_type) {
    case RlimitType::nofile:
      return RLIMIT_NOFILE;
    case RlimitType::rss:
      return RLIMIT_RSS;
    default:
      UNREACHABLE();
  }
}

}  // namespace

td::Status change_rlimit(RlimitType rlim_type, td::uint64 value, td::uint64 cap) {
  if (cap && value > cap) {
    return td::Status::Error("setrlimit(): bad argument");
  }
  int resource = get_rlimit_type(rlim_type);

  struct rlimit r;
  if (getrlimit(resource, &r) < 0) {
    return td::Status::PosixError(errno, "failed getrlimit()");
  }

  if (cap) {
    r.rlim_max = cap;
  } else if (r.rlim_max < value) {
    r.rlim_max = value;
  }
  r.rlim_cur = value;
  if (setrlimit(resource, &r) < 0) {
    return td::Status::PosixError(errno, "failed setrlimit()");
  }
  return td::Status::OK();
}

td::Status change_maximize_rlimit(RlimitType rlim_type, td::uint64 value) {
  int resource = get_rlimit_type(rlim_type);

  struct rlimit r;
  if (getrlimit(resource, &r) < 0) {
    return td::Status::PosixError(errno, "failed getrlimit()");
  }

  if (r.rlim_max < value) {
    auto t = r;
    t.rlim_cur = value;
    t.rlim_max = value;
    if (setrlimit(resource, &t) >= 0) {
      return td::Status::OK();
    }
  }

  r.rlim_cur = value < r.rlim_max ? value : r.rlim_max;
  if (setrlimit(resource, &r) < 0) {
    return td::Status::PosixError(errno, "failed setrlimit()");
  }
  return td::Status::OK();
}
#else
td::Status change_rlimit(RlimitType rlim, td::uint64 value) {
  return td::Status::Error("setrlimit not implemented on WINDOWS");
}
td::Status change_maximize_rlimit(RlimitType rlim, td::uint64 value) {
  return td::Status::OK();
}
#endif

}  // namespace td

