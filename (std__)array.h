
#ifndef OOP_ASSIGNMENTS_ARRAY_H
#define OOP_ASSIGNMENTS_ARRAY_H
#include <stdexcept>

class ArrayOutOfRange : public std::out_of_range {
 public:
  ArrayOutOfRange() : std::out_of_range("ArrayOutOfRange") {
  }
};

#include <iostream>

template <typename T, size_t N>
class Array {
 public:
  T arr[N]{};
  Array() = default;

  inline const T &operator[](size_t idx) const {
    return arr[idx];
  }

  inline T &operator[](size_t idx) {
    return arr[idx];
  }

  T &Front() {
    return arr[0];
  }

  const T &Front() const {
    return arr[0];
  }

  T &Back() {
    return arr[N - 1];
  }

  const T &Back() const {
    return arr[N - 1];
  }

  const T *Data() const {
    return arr;
  }

  size_t Size() const {
    return N;
  }

  bool Empty() const {
    return false;
  }

  void Fill(const T &value) {
    for (auto &item : arr) {
      item = value;
    }
  }

  inline const T &At(size_t idx) const {
    if (idx >= N || idx < 0) {
      throw ArrayOutOfRange();
    }
    return arr[idx];
  }

  inline T &At(size_t idx) {
    if (idx >= N || idx < 0) {
      throw ArrayOutOfRange();
    }
    return arr[idx];
  }

  void Swap(Array<T, N> &other) {
    for (size_t i = 0; i < N; ++i) {
      std::swap(arr[i], other.arr[i]);
    }
  }
};

#endif  // OOP_ASSIGNMENTS_ARRAY_H
