
#pragma once

#include /utils/port/config.h"

#include /utils/common.h"
#include /utils/Slice.h"
#include /utils/Status.h"

namespace td {

struct Stat {
  bool is_dir_;
  bool is_reg_;
  int64 size_;
  int64 real_size_;
  uint64 atime_nsec_;
  uint64 mtime_nsec_;
};

Result<Stat> stat(CSlice path) TD_WARN_UNUSED_RESULT;

struct CpuStat {
  uint64 total_ticks_{0};
  uint64 process_user_ticks_{0};
  uint64 process_system_ticks_{0};
};

Result<CpuStat> cpu_stat() TD_WARN_UNUSED_RESULT;

struct MemStat {
  uint64 resident_size_ = 0;
  uint64 resident_size_peak_ = 0;
  uint64 virtual_size_ = 0;
  uint64 virtual_size_peak_ = 0;
};

Result<MemStat> mem_stat() TD_WARN_UNUSED_RESULT;

#if TD_PORT_POSIX

namespace detail {
Result<Stat> fstat(int native_fd);
}  // namespace detail

Status update_atime(CSlice path) TD_WARN_UNUSED_RESULT;

#endif

}  // namespace td
