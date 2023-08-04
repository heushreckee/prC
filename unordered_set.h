//
// Created by sasha on 27.04.22.
//

#ifndef OOP_ASSIGNMENTS_UNORDERED_SET_H
#define OOP_ASSIGNMENTS_UNORDERED_SET_H

#define ITERATOR_IMPLEMENTED
#define FORWARD_LIST_IMPLEMENTED

#include <forward_list>
#include <vector>
#include <iostream>
#include <iterator>
#include <stdexcept>

template <class KeyT>
class UnorderedSet {
 private:
  template <class Iter>
  using EnableIfForwardIter = std::enable_if_t<
      std::is_base_of_v<std::forward_iterator_tag, typename std::iterator_traits<Iter>::iterator_category>>;

 public:
  using Iterator = typename std::forward_list<KeyT>::const_iterator;
  using ConstIterator = typename std::forward_list<KeyT>::const_iterator;
  using DifferenceType = int;

  UnorderedSet() : count_(0), size_(0), tab_(std::forward_list<KeyT>()), iters_(std::vector<Iterator>()) {
  }

  explicit UnorderedSet(size_t count) : count_(0), size_(count) {
    if (count != 0) {
      iters_ = std::vector<Iterator>(count, tab_.cend());
    } else {
      iters_ = std::vector<Iterator>();
    }
  }

  template <typename InputIt, typename = EnableIfForwardIter<InputIt>>
  UnorderedSet(const InputIt& begin, const InputIt& end) {
    auto it = begin;
    size_ = 0;
    while (it != end) {
      ++size_;
      ++it;
    }
    count_ = size_;
    if (size_ != 0) {
      iters_ = std::vector<Iterator>(size_, tab_.cend());
      it = begin;
      while (it != end) {
        Add(*it);
        ++it;
      }
    } else {
      iters_ = std::vector<Iterator>();
    }
  }

  UnorderedSet(const UnorderedSet<KeyT>& other) {
    count_ = other.count_;
    size_ = other.size_;
    if (size_ == 0) {
      iters_ = std::vector<Iterator>();
    } else {
      iters_ = std::vector<Iterator>(size_);
      tab_ = other.tab_;
      iters_ = std::vector<Iterator>(size_, tab_.cend());
      for (auto i = tab_.cbegin(); i != tab_.cend(); ++i) {
        if (iters_[std::hash<KeyT>{}(*i) % size_] == tab_.cend()) {
          iters_[std::hash<KeyT>{}(*i) % size_] = i;
        }
      }
    }
  }

  UnorderedSet<KeyT>& operator=(const UnorderedSet<KeyT>& other) {
    if (this == &other) {
      return *this;
    }
    count_ = other.count_;
    size_ = other.size_;
    if (count_ != 0) {
      tab_ = other.tab_;
      iters_ = std::vector<Iterator>(size_, tab_.cend());
      for (auto i = tab_.begin(); i != tab_.end(); ++i) {
        if (iters_[std::hash<KeyT>{}(*i) % size_] == tab_.end()) {
          iters_[std::hash<KeyT>{}(*i) % size_] = i;
        }
      }
    }
    return *this;
  }

  UnorderedSet(UnorderedSet<KeyT>&& other) {
    count_ = other.count_;
    size_ = other.size_;
    if (count_ != 0) {
      tab_ = std::move(other.tab_);
      iters_ = std::move(other.iters_);
      other.size_ = 0;
      other.count_ = 0;
      return;
    }
    tab_ = std::forward_list<KeyT>();
    if (size_ != 0) {
      iters_ = std::vector<Iterator>(size_);
    } else {
      iters_ = std::vector<Iterator>();
    }
    other.size_ = 0;
    other.count_ = 0;
  }

  UnorderedSet<KeyT> operator=(UnorderedSet<KeyT>&& other) {
    if (this == &other) {
      other.size_ = 0;
      other.count_ = 0;
      return *this;
    }
    count_ = other.count_;
    size_ = other.size_;
    if (count_ != 0) {
      tab_ = std::move(other.tab_);
      iters_ = std::move(other.iters_);
      other.size_ = 0;
      other.count_ = 0;
      return *this;
    }
    tab_ = std::forward_list<KeyT>();
    if (size_ != 0) {
      iters_ = std::vector<Iterator>(size_);
    } else {
      iters_ = std::vector<Iterator>();
    }
    other.size_ = 0;
    other.count_ = 0;
    return *this;
  }

  ~UnorderedSet() = default;

  [[nodiscard]] size_t Size() const noexcept {
    return count_;
  }

  [[nodiscard]] bool Empty() const noexcept {
    return count_ == 0;
  }

  void Clear() {
    count_ = 0;
  }

  [[nodiscard]] size_t BucketCount() const noexcept {
    return size_;
  }

  [[nodiscard]] size_t Bucket(const KeyT& key) const noexcept {
    return std::hash<KeyT>{}(key) % size_;
  }

  [[nodiscard]] double LoadFactor() const noexcept {
    if (size_ == 0) {
      return 0;
    }
    return static_cast<double>(count_) / size_;
  }

  [[nodiscard]] ConstIterator begin() const noexcept {  // NOLINT
    return tab_.cbegin();
  }

  [[nodiscard]] ConstIterator end() const noexcept {  // NOLINT
    return tab_.cend();
  }

  [[nodiscard]] ConstIterator cbegin() const noexcept {  // NOLINT
    return tab_.cbegin();
  }

  [[nodiscard]] ConstIterator cend() const noexcept {  // NOLINT
    return tab_.cend();
  }

  void Insert(const KeyT& other) {
    if (size_ == 0) {
      ++size_;
      tab_.push_front(other);
      iters_ = std::vector<Iterator>(size_, tab_.begin());
      ++count_;
      if (size_ < count_) {
        Rehash(size_ * 2);
      }
      return;
    }
    Add(other);
    ++count_;
    if (size_ < count_) {
      Rehash(size_ * 2);
    }
  }

  Iterator inline EndBucket(size_t ind) {
    auto i = static_cast<int>(ind) - 1;
    while (i >= 0 && iters_[i] == tab_.cend()) {
      --i;
    }
    if (i < 0) {
      return tab_.before_begin();
    }
    if (ind == 0) {
      return tab_.cbefore_begin();
    }
    auto size_b = BucketSize(i);
    if (size_b == 1) {
      return iters_[i];
    }
    auto beg = iters_[i];
    for (int j = 0; j < size_b - 1; ++j) {
      ++beg;
    }
    return beg;
  }

  void Erase(const KeyT& key) {
    if (*(iters_[std::hash<KeyT>{}(key) % size_]) == key) {
      if (BucketSize(std::hash<KeyT>{}(key) % size_) == 1) {
        auto i = EndBucket(std::hash<KeyT>{}(key) % size_);
        tab_.erase_after(i);
        iters_[std::hash<KeyT>{}(key) % size_] = tab_.cend();
      } else {
        ++iters_[std::hash<KeyT>{}(key) % size_];
        auto i = EndBucket(std::hash<KeyT>{}(key) % size_);
        tab_.erase_after(i);
      }
    } else {
      auto bac = BucketSize(std::hash<KeyT>{}(key) % size_);
      auto start = iters_[std::hash<KeyT>{}(key) % size_];
      auto result = start;
      for (int i = 0; i < bac; ++i) {
        auto s = start;
        ++s;
        if (*s == key) {
          result = start;
          break;
        }
        ++start;
      }
      tab_.erase_after(result);
    }
    --count_;
  }

  [[nodiscard]] bool Find(const KeyT& key) const noexcept {
    if (size_ == 0) {
      return false;
    }
    if (count_ == 0) {
      return false;
    }
    auto it = iters_[std::hash<KeyT>{}(key) % size_];
    auto j = std::hash<KeyT>{}(key) % size_ + 1;
    while (j < size_ && iters_[j] == tab_.end()) {
      ++j;
    }
    Iterator k;
    if (j >= size_) {
      k = tab_.end();
    } else {
      k = iters_[j];
    }
    while (it != tab_.end() && it != k) {
      if (*it == key) {
        return true;
      }
      ++it;
    }
    return false;
  }

  [[nodiscard]] int BucketSize(size_t id) const noexcept {
    if (id >= size_) {
      return 0;
    }
    if (count_ <= 0) {
      return 0;
    }
    auto it = iters_[id];
    ++it;
    if (it == tab_.cend()) {
      return 1;
    }
    it = iters_[id];
    auto temp = std::hash<KeyT>{}(*it) % size_;
    auto temp2 = std::hash<KeyT>{}(*it) % size_;
    int count = 0;
    while (it != tab_.end() && temp == temp2) {
      ++count;
      ++it;
      if (it != tab_.end()) {
        temp2 = std::hash<KeyT>{}(*it) % size_;
      }
    }
    return count;
  }

  void Rehash(size_t newcount) {
    if (newcount != size_ && newcount >= count_) {
      UnorderedSet temp(newcount);
      for (auto& it : tab_) {
        temp.Insert(it);
      }
      *this = std::move(temp);
    }
  }

  void Reserve(size_t newcount) {
    if (newcount >= size_) {
      UnorderedSet temp(newcount);
      for (auto& it : tab_) {
        temp.Insert(it);
      }
      *this = std::move(temp);
    }
  }

  void Add(const KeyT& key) {
    auto index = static_cast<int64_t>(std::hash<KeyT>{}(key) % size_);
    if (iters_[index] != tab_.cend()) {
      tab_.insert_after(iters_[index], key);
      return;
    }
    auto temp_idx = index - 1;
    while (temp_idx >= 0 && iters_[temp_idx] == tab_.cend()) {
      --temp_idx;
    }
    if (temp_idx < 0) {
      tab_.push_front(key);
      iters_[index] = tab_.cbegin();
      return;
    }
    auto start = iters_[temp_idx];
    if (++start == tab_.cend()) {
      tab_.insert_after(iters_[temp_idx], key);
      start = iters_[temp_idx];
      iters_[index] = ++start;
      return;
    }
    start = iters_[temp_idx];
    auto result = start;
    size_t ind_start = std::hash<KeyT>{}(*start) % size_;
    size_t begin = std::hash<KeyT>{}(*(++start)) % size_;
    while (start != tab_.cend() && ind_start == begin) {
      result = start;
      ++start;
      if (start != tab_.cend()) {
        begin = std::hash<KeyT>{}(*start) % size_;
      }
    }
    tab_.insert_after(result, key);
    iters_[index] = ++result;
  }

 private:
  size_t count_;
  size_t size_;
  std::forward_list<KeyT> tab_ = std::forward_list<KeyT>();
  std::vector<Iterator> iters_;
};

#endif
