

#pragma once

#include /utils/common.h"
#include /utils/SharedSlice.h"
#include /utils/Slice.h"
#include /utils/Status.h"

namespace td {

string base64_encode(Slice input);
Result<string> base64_decode(Slice base64);
Result<SecureString> base64_decode_secure(Slice base64);

string base64url_encode(Slice input);
Result<string> base64url_decode(Slice base64);

bool is_base64(Slice input);
bool is_base64url(Slice input);

bool is_base64_characters(Slice input);
bool is_base64url_characters(Slice input);

string base64_filter(Slice input);
string base32_encode(Slice input, bool upper_case = false);
Result<string> base32_decode(Slice base32);
}  // namespace td
