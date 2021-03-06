
#pragma once

namespace td {

template <class FunctionT>
struct member_function_class;

template <class ReturnType, class Type, class... Args>
struct member_function_class<ReturnType (Type::*)(Args...)> {
  using type = Type;
  static constexpr size_t argument_count() {
    return sizeof...(Args);
  }
};

template <class FunctionT>
using member_function_class_t = typename member_function_class<FunctionT>::type;

template <class FunctionT>
constexpr size_t member_function_argument_count() {
  return member_function_class<FunctionT>::argument_count();
}

}  // namespace td
