#include "constants.hpp"
#ifdef TonUINO_Esp32

#include "webserial.hpp"

#include "webserial_pages.hpp"
#include "webservice_pages.hpp"

Webserial::Webserial() {
  ringBuffer = new WebserialBuffer;
}

void Webserial::begin(AsyncWebServer *server, const String& n_hostname) {
  hostname  = n_hostname;
  webserver = server;
  websocket = new AsyncWebSocket("/ws_serial");

  webserver->on("/webserial_style.css", HTTP_GET, [this](AsyncWebServerRequest *request) {
    request->send(200, "text/css", webserial_style);
  });
  webserver->on("/webserial", HTTP_GET, [this](AsyncWebServerRequest *request) {
    request->send(200, "text/html", webserial_page, [this](const String& var) { return process_page(var);});
  });

  websocket->onEvent([this](AsyncWebSocket *server, AsyncWebSocketClient *client,
                       AwsEventType type, void *arg, uint8_t *data, size_t len) {
    switch (type) {
    case WS_EVT_CONNECT:
      // set the client to not close when the queue is full
      client->setCloseClientOnQueueFull(false);
      websocket->text(client->id(), ringBuffer->get());
      break;
    default:
      break;
    }
  });

  webserver->addHandler(websocket);
}

void Webserial::loop() {
  if (websocket)
    websocket->cleanupClients();
}

size_t Webserial::write(uint8_t m) {
  return write(&m, 1);
}

size_t Webserial::write(const uint8_t *buffer, size_t size) {
  String message((const char *)buffer, size);
  if (not message.endsWith("\n")) {
    messageBuffer += message;
  }
  else {
    messageBuffer = "<pre>" + messageBuffer + "</pre>";
    ringBuffer->write(messageBuffer);
    if (websocket)
      websocket->textAll(messageBuffer);
    messageBuffer = "";
  }
  return size;
}

String Webserial::process_page(const String& var) {

if (var == "TOPNAV")
    return topnav_html;
else if (var == "HOSTNAME")
  return hostname;

return "";

}



#endif

