#ifndef SRC_RINGBUFFER_HPP_
#define SRC_RINGBUFFER_HPP_

#include <Arduino.h>

template<size_t LOG_BUFFER_SIZE>
class RingBuffer
{
public:
  RingBuffer() {}
  ~RingBuffer() {}
  size_t write(uint8_t character);
  size_t write(const String& string);
  String get();
  void * operator new(size_t size);

private:
  char    buffer[LOG_BUFFER_SIZE];
  size_t  bufferIndex{0};
  boolean isFull     {false};
};

template<size_t LOG_BUFFER_SIZE>
size_t RingBuffer<LOG_BUFFER_SIZE>::write(uint8_t c) {
  buffer[bufferIndex++] = c;

  // Check for overflow
  if (bufferIndex >= LOG_BUFFER_SIZE)
  {
    bufferIndex = 0u;
    isFull = true;
  }

  return 1u;
}

template<size_t LOG_BUFFER_SIZE>
size_t RingBuffer<LOG_BUFFER_SIZE>::write(const String& string) {
  for (uint32_t i = 0; i < string.length(); ++i)
    write(string[i]);
  return string.length();
}

template<size_t LOG_BUFFER_SIZE>
String RingBuffer<LOG_BUFFER_SIZE>::get() {
  String logString;

  if (not isFull) {
    for (size_t i = 0; i < bufferIndex; i++) {
      logString += buffer[i];
    }
  }
  else {
    for (size_t i = 0; i < LOG_BUFFER_SIZE; i++) {
      size_t index = bufferIndex + i;

      if (index >= LOG_BUFFER_SIZE) {
        index -= LOG_BUFFER_SIZE;
      }

      logString += buffer[index];
    }
  }
  return logString;
}

template<size_t LOG_BUFFER_SIZE>
void *RingBuffer<LOG_BUFFER_SIZE>::operator new(size_t size)
{
  return psramFound() ? ps_malloc(size) : malloc(size);
}


#endif // SRC_RINGBUFFER_HPP_
