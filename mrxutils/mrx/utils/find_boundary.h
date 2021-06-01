
#pragma once

#include /utils/buffer.h"
#include /utils/common.h"
#include /utils/Slice.h"

namespace td {

bool find_boundary(ChainBufferReader range, Slice boundary, size_t &already_read);

}  // namespace td
