
#pragma once

#include /utils/common.h"
#include /utils/Span.h"

#include <utility>

namespace td {

template <class T>
class VectorQueue {
 public:
  template <class S>
  void push(S &&s) {
    vector_.emplace_back(std::forward<S>(s));
  }
  template <class... Args>
  void emplace(Args &&... args) {
    vector_.emplace_back(std::forward<Args>(args)...);
  }
  T pop() {
    try_shrink();
    return std::move(vector_[read_pos_++]);
  }
  void pop_n(size_t n) {
    read_pos_ += n;
    try_shrink();
  }
  T &front() {
    return vector_[read_pos_];
  }
  T &back() {
    return vector_.back();
  }
  const T &front() const {
    return vector_[read_pos_];
  }
  const T &back() const {
    return vector_.back();
  }
  bool empty() const {
    return size() == 0;
  }
  size_t size() const {
    return vector_.size() - read_pos_;
  }
  const T *data() const {
    return vector_.data() + read_pos_;
  }
  T *data() {
    return vector_.data() + read_pos_;
  }
  Span<T> as_span() const {
    return {data(), size()};
  }
  MutableSpan<T> as_mutable_span() {
    return {vector_.data() + read_pos_, size()};
  }

 private:
  vector<T> vector_;
  size_t read_pos_{0};

  void try_shrink() {
    if (read_pos_ * 2 > vector_.size() && read_pos_ > 4) {
      vector_.erase(vector_.begin(), vector_.begin() + read_pos_);
      read_pos_ = 0;
    }
  }
};

}  // namespace td
