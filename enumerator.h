#ifndef OOP_ASSIGNMENTS_ENUMERATE_H
#define OOP_ASSIGNMENTS_ENUMERATE_H

template <class Container>
class EnumeratorLValueHolder {
 private:
  Container &container_;

 public:
  explicit EnumeratorLValueHolder(Container &container) : container_(container) {
  }

  using Iter = decltype(std::declval<Container>().begin());
  using T = decltype(*std::declval<Iter>());

  class Iterator {
   private:
    size_t index_;
    Iter it_;

   public:
    Iterator(size_t index, Iter it) : index_(index), it_(it) {
    }

    auto operator*() {
      return std::pair<size_t, T>(index_, *it_);
    }

    Iterator &operator++() {
      ++it_;
      ++index_;
      return *this;
    }

    bool operator!=(const Iterator &other) {
      return (it_ != other.it_);
    }
  };

  Iterator begin() const {  //  NOLINT
    return {0, container_.begin()};
  }

  Iterator end() const {  //  NOLINT
    auto it = container_.begin();
    size_t count = 0;
    while (it != container_.end()) {
      ++count;
      ++it;
    }
    return {count, container_.end()};
  }
};

template <class Container>
auto Enumerate(Container &&container) {
  return EnumeratorLValueHolder<decltype(container)>(container);
}

#endif  // OOP_ASSIGNMENTS_ENUMERATE_H
