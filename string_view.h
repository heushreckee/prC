#ifndef OOP_ASSIGNMENTS_D_H
#define OOP_ASSIGNMENTS_D_H

#include <stdexcept>
#include <iostream>
#include <cstring>

class StringViewOutOfRange : public std::out_of_range {
 public:
  StringViewOutOfRange() : std::out_of_range("StringViewOutOfRange") {
  }
};

class StringView {
 private:
  size_t size_;
  char* str_;

 public:
  StringView() {
    size_ = 0;
    str_ = nullptr;
  }
  StringView(const char* s) {  // NOLINT
    str_ = const_cast<char*>(s);
    size_ = strlen(s);
  }
  StringView(const char* s, size_t n) {
    size_ = n;
    str_ = const_cast<char*>(s);
  }
  StringView(const StringView& s) = default;
  ~StringView() = default;
  StringView& operator=(const StringView& s) = default;
  const char& operator[](size_t index) const {
    return str_[index];
  }
  const char& At(size_t index) const {
    if (index >= size_) {
      throw StringViewOutOfRange{};
    }
    return str_[index];
  }
  const char& Front() const {
    return str_[0];
  }
  const char& Back() const {
    return str_[size_ - 1];
  }
  const char* Data() const {
    return str_;
  }
  bool Empty() const {
    return size_ == 0;
  }
  size_t Size() const {
    return size_;
  }
  size_t Length() const {
    return size_;
  }
  void Swap(StringView& other) {
    StringView tmp = other;
    other = *(this);
    *(this) = tmp;
  }
  void RemovePrefix(size_t prefix_size) {
    size_ -= prefix_size;
    str_ += prefix_size;
  }
  void RemoveSuffix(size_t suffix_size) {
    size_ -= suffix_size;
  }
  StringView Substr(size_t pos, size_t count = -1) {
    if (pos > size_) {
      throw StringViewOutOfRange{};
    }
    size_t n = std::min(count, size_ - pos);
    return StringView{str_ + pos, n};
  }
};
#endif  // OOP_ASSIGNMENTS_D_H
