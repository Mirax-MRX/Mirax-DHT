
#pragma once

#include /utils/port/config.h"

#if TD_PORT_WINDOWS

#include /utils/common.h"
#include /utils/Slice.h"
#include /utils/Status.h"

#include <string>

namespace td {

Result<std::wstring> to_wstring(CSlice slice);

Result<string> from_wstring(const std::wstring &str);

Result<string> from_wstring(const wchar_t *begin, size_t size);

Result<string> from_wstring(const wchar_t *begin);

}  // namespace td

#endif
