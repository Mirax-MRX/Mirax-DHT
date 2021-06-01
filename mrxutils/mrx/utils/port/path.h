
#pragma once

#include /utils/common.h"
#include /utils/port/FileFd.h"
#include /utils/Slice.h"
#include /utils/Status.h"

#include <functional>
#include <type_traits>
#include <utility>

namespace td {

Status mkdir(CSlice dir, int32 mode = 0700) TD_WARN_UNUSED_RESULT;

Status mkpath(CSlice path, int32 mode = 0700) TD_WARN_UNUSED_RESULT;

Status rename(CSlice from, CSlice to) TD_WARN_UNUSED_RESULT;

Result<string> realpath(CSlice slice, bool ignore_access_denied = false) TD_WARN_UNUSED_RESULT;

Status chdir(CSlice dir) TD_WARN_UNUSED_RESULT;

Status rmdir(CSlice dir) TD_WARN_UNUSED_RESULT;

Status unlink(CSlice path) TD_WARN_UNUSED_RESULT;

Status rmrf(CSlice path) TD_WARN_UNUSED_RESULT;

Status set_temporary_dir(CSlice dir) TD_WARN_UNUSED_RESULT;

CSlice get_temporary_dir();

Result<std::pair<FileFd, string>> mkstemp(CSlice dir) TD_WARN_UNUSED_RESULT;

Result<string> mkdtemp(CSlice dir, Slice prefix) TD_WARN_UNUSED_RESULT;

class WalkPath {
 public:
  enum class Action { Continue, Abort, SkipDir };
  enum class Type { EnterDir, ExitDir, NotDir };

  template <class F, class R = decltype(std::declval<F>()("", Type::ExitDir))>
  static TD_WARN_UNUSED_RESULT std::enable_if_t<std::is_same<R, Action>::value, Status> run(CSlice path, F &&func) {
    return do_run(path, func);
  }
  template <class F, class R = decltype(std::declval<F>()("", Type::ExitDir))>
  static TD_WARN_UNUSED_RESULT std::enable_if_t<!std::is_same<R, Action>::value, Status> run(CSlice path, F &&func) {
    return do_run(path, [&](CSlice name, Type type) {
      func(name, type);
      return Action::Continue;
    });
  }

 private:
  static TD_WARN_UNUSED_RESULT Status do_run(CSlice path,
                                             const std::function<WalkPath::Action(CSlice name, Type type)> &func);
};

// deprecated interface
template <class F>
TD_WARN_UNUSED_RESULT Status walk_path(CSlice path, F &&func) {
  return WalkPath::run(path, func);
}

}  // namespace td