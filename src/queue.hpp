#ifndef SRC_QUEUE_HPP_
#define SRC_QUEUE_HPP_

#include "array.hpp"

inline void setBit  (uint8_t &v, uint8_t b) { v |=  (1<<b); }
inline void clearBit(uint8_t &v, uint8_t b) { v &= ~(1<<b); }
inline bool getBit  (uint8_t &v, uint8_t b) { return v & (1<<b); }

template<uint8_t BITS>
class bitfield {
public:
  void setBit  (uint8_t n) { uint8_t &v = bits[n/8]; ::setBit  (v, n%8); }
  void clearBit(uint8_t n) { uint8_t &v = bits[n/8]; ::clearBit(v, n%8); }
  bool getBit  (uint8_t n) { uint8_t &v = bits[n/8]; return ::getBit(v, n%8); }
  void setAll  (uint8_t v) { for (uint8_t i = 0; i < BITS/8+1; ++i) bits[i] = v; }

private:
  uint8_t bits[BITS/8+1]{};
};

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
  T pop() {
    T r{};
    if (s > 0) {
      r = c[0];
      for (uint8_t i = 1; i < s; ++i)
        c[i-1] = c[i];
      --s;
    }
    return r;
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
