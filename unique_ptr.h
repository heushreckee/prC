
#ifndef OOP_ASSIGNMENTS_UNIQUE_PTR_H
#define OOP_ASSIGNMENTS_UNIQUE_PTR_H

#include <iostream>
template <class T>
class UniquePtr {
 private:
  T *ptr_;

 public:
  explicit UniquePtr(T *p = nullptr) noexcept : ptr_(p) {
  }
  UniquePtr(UniquePtr &&p) noexcept : ptr_(p.ptr_) {
    p.ptr_ = nullptr;
  }
  UniquePtr(const UniquePtr &) = delete;

  ~UniquePtr() {
    delete ptr_;
  }

  UniquePtr &operator=(const UniquePtr &p) = delete;

  UniquePtr &operator=(UniquePtr &&p) noexcept {
    T *tmp = ptr_;
    ptr_ = p.ptr_;
    p.ptr_ = tmp;
    return *this;
  }

  T *Release() {
    T *tmp = ptr_;
    ptr_ = nullptr;
    return tmp;
  }

  void Reset(T *p = nullptr) {
    delete ptr_;
    ptr_ = p;
  }

  void Swap(UniquePtr &p) {
    T *tmp = ptr_;
    ptr_ = p.ptr_;
    p.ptr_ = tmp;
  }

  T *Get() const {
    return ptr_;
  }

  T &operator*() const {
    return *ptr_;
  }

  T *operator->() const {
    return ptr_;
  }

  explicit operator bool() const {
    return ptr_ != nullptr;
  }
};

#endif  // OOP_ASSIGNMENTS_UNIQUE_PTR_H
