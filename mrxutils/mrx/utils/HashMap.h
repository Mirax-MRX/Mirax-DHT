
#pragma once

#include /utils/Hash.h"

#if TD_HAVE_ABSL
#include <absl/container/flat_hash_map.h>
#else
#include <unordered_map>
#endif

namespace td {

#if TD_HAVE_ABSL
template <class Key, class Value, class H = Hash<Key>>
using HashMap = absl::flat_hash_map<Key, Value, H>;
#else
template <class Key, class Value, class H = Hash<Key>>
using HashMap = std::unordered_map<Key, Value, H>;
#endif

}  // namespace td
