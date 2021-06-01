
#pragma once

#include /utils/common.h"
#include /utils/logging.h"
#include /utils/port/FileFd.h"
#include /utils/Slice.h"
#include /utils/Status.h"

namespace td {

class FileLog : public LogInterface {
  static constexpr int64 DEFAULT_ROTATE_THRESHOLD = 10 * (1 << 20);

 public:
  static Result<td::unique_ptr<LogInterface>> create(string path, int64 rotate_threshold = DEFAULT_ROTATE_THRESHOLD,
                                                     bool redirect_stderr = true);
  Status init(string path, int64 rotate_threshold = DEFAULT_ROTATE_THRESHOLD, bool redirect_stderr = true);

  Slice get_path() const;

  vector<string> get_file_paths() override;

  void set_rotate_threshold(int64 rotate_threshold);

  int64 get_rotate_threshold() const;

  void append(CSlice cslice, int log_level) override;

  void rotate() override;

  void lazy_rotate();

 private:
  FileFd fd_;
  string path_;
  int64 size_ = 0;
  int64 rotate_threshold_ = 0;
  bool redirect_stderr_ = false;
  std::atomic<bool> want_rotate_{};

  void do_rotate();
};

}  // namespace td
