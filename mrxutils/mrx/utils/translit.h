
#pragma once

#include /utils/common.h"
#include /utils/Slice.h"

namespace td {

vector<string> get_word_transliterations(Slice word, bool allow_partial);

}  // namespace td
