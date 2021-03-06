
#pragma once

#include /utils/common.h"
#include /utils/Slice.h"
#include /utils/Status.h"
#include /utils/StringBuilder.h"

#include <functional>

namespace td {

class OptionsParser {
  class Option {
   public:
    enum Type { NoArg, Arg, OptionalArg };
    Type type;
    char short_key;
    std::string long_key;
    std::string description;
    std::function<Status(Slice)> arg_callback;
  };

 public:
  void set_description(std::string description);

  void add_option(Option::Type type, char short_key, Slice long_key, Slice description,
                  std::function<Status(Slice)> callback);

  void add_option(char short_key, Slice long_key, Slice description, std::function<Status(Slice)> callback);

  void add_option(char short_key, Slice long_key, Slice description, std::function<Status(void)> callback);

  Result<int> run(int argc, char *argv[]) TD_WARN_UNUSED_RESULT;

  friend StringBuilder &operator<<(StringBuilder &sb, const OptionsParser &o);

 private:
  std::vector<Option> options_;
  std::string description_;
};

}  // namespace td
