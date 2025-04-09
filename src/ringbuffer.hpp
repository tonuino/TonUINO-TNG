#ifndef SRC_RINGBUFFER_HPP_
#define SRC_RINGBUFFER_HPP_

#include <Arduino.h>

inline constexpr size_t LOG_BUFFER_SIZE = 2048u;

class RingBuffer : public Print
{
public:
  RingBuffer();
  ~RingBuffer();
  size_t write(uint8_t character);
  size_t write(const String& string);
  String get();
  void * operator new(size_t size);

private:
  char    buffer[LOG_BUFFER_SIZE];
  size_t  bufferIndex{0};
  boolean isFull     {false};
};

#endif // SRC_RINGBUFFER_HPP_
