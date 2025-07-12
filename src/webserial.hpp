#ifndef SRC_WEBSERIAL_HPP_
#define SRC_WEBSERIAL_HPP_

#include <Arduino.h>
#include <ESPAsyncWebServer.h>

#include "ringbuffer.hpp"

inline constexpr size_t LOG_BUFFER_SIZE = 20*1024u;

class Webserial: public Print {
public:
  Webserial();

  void begin(AsyncWebServer *server, const String& hostname);

  void loop();

  // Print interface
  virtual size_t write(uint8_t) override;
  virtual size_t write(const uint8_t *buffer, size_t size) override;

  using WebserialBuffer = RingBuffer<LOG_BUFFER_SIZE>;

private:
  String process_page (const String& var);

  AsyncWebServer  *webserver{ nullptr };
  AsyncWebSocket  *websocket{ nullptr };

  String           messageBuffer;
  WebserialBuffer *ringBuffer;

  String           hostname{};
};

#endif // SRC_WEBSERIAL_HPP_
