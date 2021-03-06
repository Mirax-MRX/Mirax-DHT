
#pragma once

#include /utils/common.h"
#include /utils/port/FileFd.h"
#include /utils/Slice.h"
#include /utils/Status.h"

namespace td {

class MemoryMapping {
 public:
  struct Options {
    int64 offset{0};
    int64 size{-1};

    Options() {
    }
    Options &with_offset(int64 new_offset) {
      offset = new_offset;
      return *this;
    }
    Options &with_size(int64 new_size) {
      size = new_size;
      return *this;
    }
  };

  static Result<MemoryMapping> create_anonymous(const Options &options = {});
  static Result<MemoryMapping> create_from_file(const FileFd &file, const Options &options = {});

  Slice as_slice() const;
  MutableSlice as_mutable_slice();  // returns empty slice if memory is read-only

  MemoryMapping(const MemoryMapping &other) = delete;
  const MemoryMapping &operator=(const MemoryMapping &other) = delete;
  MemoryMapping(MemoryMapping &&other);
  MemoryMapping &operator=(MemoryMapping &&other);
  ~MemoryMapping();

 private:
  class Impl;
  unique_ptr<Impl> impl_;
  explicit MemoryMapping(unique_ptr<Impl> impl);
};

}  // namespace td
