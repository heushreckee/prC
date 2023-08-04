#ifndef OOP_ASSIGNMENTS_UNIQUE_PTR_H
#define OOP_ASSIGNMENTS_UNIQUE_PTR_H
#define WEAK_PTR_IMPLEMENTED

#include <iostream>
#include <stdexcept>

class BadWeakPtr : public std::runtime_error {
 public:
  BadWeakPtr() : std::runtime_error("BadWeakPtr") {
  }
};

template <typename T, class Deleter = std::default_delete<T>>
struct ControlBlock {
  Deleter deleter;
  T *stored_ptr_ = nullptr;
  uint64_t ref_count_ = 0;
  uint64_t weak_count = 0;
};

template <typename T, class Deleter = std::default_delete<T>>
class WeakPtr;

template <typename T, class Deleter = std::default_delete<T>>
class SharedPtr {
 public:
  ControlBlock<T, Deleter> *cb_ = nullptr;
  SharedPtr() : cb_(nullptr) {
  }
  explicit SharedPtr(const T &&data) = delete;

  SharedPtr(const WeakPtr<T> &other) : cb_(other.cb_) {  //  NOLINT
    if (other.Expired()) {
      throw BadWeakPtr();
    }
    if (cb_ != nullptr) {
      ++(cb_->ref_count_);
    }
  }

  SharedPtr(T *raw_ptr) noexcept {  // NOLINT
    if (raw_ptr == nullptr) {
      return;
    }
    cb_ = new ControlBlock<T, Deleter>;
    cb_->stored_ptr_ = raw_ptr;
    ++(cb_->ref_count_);
  }

  SharedPtr(const SharedPtr &other) noexcept : cb_(other.cb_) {
    if (cb_ != nullptr) {
      ++(cb_->ref_count_);
    }
  }

  SharedPtr(SharedPtr &&p) noexcept : cb_(p.cb_) {
    p.cb_ = nullptr;
  }

  SharedPtr &operator=(const SharedPtr &other) {
    if (this == &other) {
      return *this;
    }
    if (cb_ == nullptr) {
      cb_ = other.cb_;
      if (cb_->stored_ptr_ != nullptr) {
        ++(cb_->ref_count_);
      }
    } else {
      --(cb_->ref_count_);
      if (cb_->ref_count_ == 0) {
        cb_->deleter(cb_->stored_ptr_);
        delete cb_;
      }
      cb_ = other.cb_;
      if (cb_ != nullptr) {
        ++(cb_->ref_count_);
      }
    }
    return *this;
  }

  SharedPtr &operator=(SharedPtr &&other) noexcept {
    if (this == &other) {
      return *this;
    }
    if (cb_ != nullptr) {
      --(cb_->ref_count_);
      if (cb_->ref_count_ == 0) {
        cb_->deleter(cb_->stored_ptr_);
        delete cb_;
      }
    }
    cb_ = other.cb_;
    other.cb_ = nullptr;
    return *this;
  }

  ~SharedPtr() {
    if (cb_ == nullptr) {
      return;
    }
    --(cb_->ref_count_);
    if (cb_->ref_count_ == 0) {
      cb_->deleter(cb_->stored_ptr_);
      cb_->stored_ptr_ = nullptr;
    }
    if (cb_->weak_count == 0 && cb_->ref_count_ == 0) {
      delete cb_;
      cb_ = nullptr;
    }
  }

  void Reset(T *ptr = nullptr) {
    if (cb_ != nullptr) {
      --cb_->ref_count_;
      if (cb_->ref_count_ == 0) {
        cb_->deleter(cb_->stored_ptr_);
        delete cb_;
        cb_ = nullptr;
      }
    }
    if (ptr != nullptr) {
      cb_ = new ControlBlock<T, Deleter>;
      cb_->stored_ptr_ = ptr;
      ++(cb_->ref_count_);
    } else {
      cb_ = nullptr;
    }
  }

  void Swap(SharedPtr<T> &p) {
    if (cb_ == p.cb_) {
      return;
    }
    ControlBlock<T, Deleter> *tmp = cb_;
    cb_ = p.cb_;
    p.cb_ = tmp;
  }

  T *Get() const {
    if (cb_ == nullptr) {
      return nullptr;
    }
    return cb_->stored_ptr_;
  }

  explicit operator T *() {
    if (cb_ == nullptr) {
      return nullptr;
    }
    return cb_->stored_ptr_;
  }

  T &operator*() const {
    return *(cb_->stored_ptr_);
  }

  T *operator->() const {
    if (cb_ == nullptr) {
      return nullptr;
    }
    return cb_->stored_ptr_;
  }

  explicit operator bool() const {
    if (cb_ == nullptr) {
      return false;
    }
    return (cb_->stored_ptr_) != nullptr;
  }

  uint16_t UseCount() const {
    if (cb_ == nullptr) {
      return 0;
    }
    return cb_->ref_count_;
  }
  friend WeakPtr<T>;
};

template <typename T, class Deleter>
class WeakPtr {
 public:
  ControlBlock<T> *cb_ = nullptr;
  WeakPtr() : cb_(nullptr) {
  }

  WeakPtr(T *weak_ptr) noexcept {  // NOLINT
    if (weak_ptr == nullptr) {
      return;
    }
    cb_ = new ControlBlock<T, Deleter>;
    cb_->stored_ptr_ = weak_ptr;
  }

  explicit WeakPtr(const T &&ptr) = delete;

  WeakPtr(const WeakPtr &other) noexcept : cb_(other.cb_) {
    if (cb_ != nullptr) {
      ++(cb_->weak_count);
    }
  }

  WeakPtr(WeakPtr &&other) noexcept : WeakPtr() {
    Swap(other);
    other.cb_ = nullptr;
  }

  WeakPtr(const SharedPtr<T> &other) : cb_(other.cb_) {  //  NOLINT
    if (cb_ != nullptr) {
      ++(cb_->weak_count);
    }
  }

  WeakPtr &operator=(const WeakPtr &other) {
    {
      if (this == &other) {
        return *this;
      }
      if (cb_ == nullptr) {
        cb_ = other.cb_;
        if (cb_->stored_ptr_ != nullptr) {
          ++(cb_->weak_count);
        }
      } else {
        --(cb_->weak_count);
        cb_ = other.cb_;
      }
      return *this;
    }
  }

  WeakPtr &operator=(WeakPtr &&other) noexcept {
    if (this == &other) {
      return *this;
    }
    if (cb_ != nullptr) {
      --(cb_->weak_count);
    }
    cb_ = other.cb_;
    other.cb_ = nullptr;
    return *this;
  }

  void Reset(T *ptr = nullptr) {
    if (cb_) {
      --(cb_->weak_count);
    }
    if (ptr != nullptr) {
      cb_ = new ControlBlock<T, Deleter>;
      cb_->stored_ptr_ = ptr;
    } else {
      cb_ = nullptr;
    }
  }

  void Swap(WeakPtr<T> &other) {
    if (cb_ == other.cb_) {
      return;
    }
    ControlBlock<T, Deleter> *tmp = cb_;
    cb_ = other.cb_;
    other.cb_ = tmp;
  }

  bool Expired() const {
    if (cb_ != nullptr) {
      return cb_->ref_count_ == 0;
    }
    return true;
  }

  size_t UseCount() const {
    if (cb_) {
      return cb_->ref_count_;
    }
    return 0;
  }

  SharedPtr<T> Lock() const {
    return (Expired() ? SharedPtr<T>(nullptr) : SharedPtr<T>(*this));
  }

  ~WeakPtr() {
    if (cb_ == nullptr) {
      return;
    }
    --(cb_->weak_count);
    if (cb_->weak_count == 0 && cb_->ref_count_ == 0) {
      delete cb_;
      cb_ = nullptr;
    }
  }
  friend SharedPtr<T>;
};

template <class T, class... Args>
SharedPtr<T> MakeShared(Args &&... args) {
  return SharedPtr<T>(new T(std::forward<Args>(args)...));
}

#endif  // OOP_ASSIGNMENTS_UNIQUE_PTR_H
