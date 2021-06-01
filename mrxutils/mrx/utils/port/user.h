

#pragma once

#include /utils/Slice.h"
#include /utils/Status.h"

namespace td {
Status change_user(CSlice username, CSlice groupname = CSlice());
}
