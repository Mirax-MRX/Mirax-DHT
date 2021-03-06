
#pragma once

#include /utils/common.h"
#include /utils/Slice.h"

namespace td {

class MimeType {
 public:
  static string to_extension(Slice mime_type, Slice default_value = Slice());
  static string from_extension(Slice extension, Slice default_value = Slice());
};

}  // namespace td
