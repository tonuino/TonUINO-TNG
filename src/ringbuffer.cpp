#include "constants.hpp"
#ifdef TonUINO_Esp32

#include "ringbuffer.hpp"

RingBuffer::RingBuffer() {
}

RingBuffer::~RingBuffer() {
}

size_t RingBuffer::write(uint8_t c) {
  buffer[bufferIndex++] = c;

  // Check for overflow
  if (bufferIndex >= LOG_BUFFER_SIZE)
  {
    bufferIndex = 0u;
    isFull = true;
  }

  return 1u;
}

size_t RingBuffer::write(const String& string) {
  for (uint32_t i = 0; i < string.length(); ++i)
    write(string[i]);
  return string.length();
}

String RingBuffer::get() {
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

void *RingBuffer::operator new(size_t size)
{
  return psramFound() ? ps_malloc(size) : malloc(size);
}

#endif
