
#pragma once

#include /utils/common.h"
#include /utils/Slice.h"
#include /utils/Status.h"
#include /utils/StringBuilder.h"

#include <functional>

namespace td {

class OptionParser {
  class Option {
   public:
    enum class Type { NoArg, Arg };
    Type type;
    char short_key;
    string long_key;
    string description;
    std::function<Status(Slice)> arg_callback;
  };

  void add_option(Option::Type type, char short_key, Slice long_key, Slice description,
                  std::function<Status(Slice)> callback);

 public:
  void set_description(string description);

  void add_checked_option(char short_key, Slice long_key, Slice description, std::function<Status(Slice)> callback);

  void add_checked_option(char short_key, Slice long_key, Slice description, std::function<Status(void)> callback);

  void add_option(char short_key, Slice long_key, Slice description, std::function<Status(Slice)> callback) = delete;

  void add_option(char short_key, Slice long_key, Slice description, std::function<Status(void)> callback) = delete;

  void add_option(char short_key, Slice long_key, Slice description, std::function<void(Slice)> callback);

  void add_option(char short_key, Slice long_key, Slice description, std::function<void(void)> callback);

  void add_check(std::function<Status()> check);

  // returns found non-option parameters
  Result<vector<char *>> run(int argc, char *argv[], int expected_non_option_count = -1) TD_WARN_UNUSED_RESULT;

  friend StringBuilder &operator<<(StringBuilder &sb, const OptionParser &o);

 private:
  vector<Option> options_;
  vector<std::function<Status()>> checks_;
  string description_;
};

}  // namespace td
