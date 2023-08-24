#ifndef SRC_QUEUE_HPP_
#define SRC_QUEUE_HPP_

#include "array.hpp"

template<typename T>
void swap(T &lhs, T &rhs) {
  const T t = lhs;
  lhs       = rhs;
  rhs       = t;
}

template <class T, uint8_t N>
class queue {
public:
  void push(T t) {
    if (s < N)
      c[s++] = t;
  }
  T &get (uint8_t pos) {
    return c[pos];
  }
  void clear() { s = 0; }
  uint8_t size()  { return s; }
  void shuffle() {
    // Queue mischen
    for (uint8_t i = 0; i < s; ++i) {
      const uint8_t j = random(0, s);
      swap(c[i], c[j]);
    }
  }
  // Iterators
  T *begin() { return c.begin(); }
  const T *begin() const { return c.begin(); }
  T *end() { return &c[s]; }
  const T *end() const { return &c[s]; }

private:
  array<T, N> c{};
  uint8_t     s{};
};

#endif /* SRC_QUEUE_HPP_ */
