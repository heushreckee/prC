//
// Created by sasha on 28.04.22.
//

#ifndef OOP_ASSIGNMENTS_RANGE_H
#define OOP_ASSIGNMENTS_RANGE_H

#include <iostream>

class Iterator {
 private:
  int64_t idx_ = 0;
  int64_t step_ = 1;
  int64_t end_;
  int64_t begin_ = 0;

 public:
  Iterator(int64_t idx, int64_t step) : idx_(idx), step_(step){};
  Iterator(int64_t step, int64_t end, int64_t begin) {
    step_ = step;
    end_ = end;
    begin_ = begin;
  }

  bool operator!=(const Iterator &other) {
    if (step_ > 0) {
      return idx_ < other.idx_;
    }
    return idx_ > other.idx_;
  }

  Iterator &operator++() {
    idx_ += step_;
    return *this;
  }

  int64_t operator*() {
    return idx_;
  }
  Iterator begin() const {  // NOLINT
    return {begin_, step_};
  }

  Iterator end() const {  // NOLINT
    return {end_, step_};
  }
};

Iterator inline Range(int64_t end) {
  int64_t begin = 0;
  int64_t step = 1;
  if (end < 0) {
    end = 0;
  }
  return Iterator(step, end, begin);
}

Iterator inline Range(int64_t begin, int64_t end, int64_t step = 1) {
  if (end < begin && step >= 0) {
    begin = 0;
    end = 0;
  }
  {
    if (end > begin && step < 0) {
      begin = 0;
      end = 0;
    }
  }
  return Iterator(step, end, begin);
}
#endif  // OOP_ASSIGNMENTS_RANGE_H
