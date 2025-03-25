#ifndef SRC_WEBSERVICE_HPP_
#define SRC_WEBSERVICE_HPP_

#include <WiFiManager.h>
#include <ESPAsyncWebServer.h>

#include "settings.hpp"
#include "commands.hpp"

class Mp3;

class Webservice: public CommandSource {

public:
  Webservice(Settings& settings, Mp3& mp3)
  : settings(settings)
  , mp3(mp3)
  {};

  void init();
  void loop();

  commandRaw getCommandRaw() override;

private:
  void page_notfound  (AsyncWebServerRequest *request);
  void page_style_css (AsyncWebServerRequest *request);

  void page_main      (AsyncWebServerRequest *request);
  void service        (AsyncWebServerRequest *request);
  void modifier       (AsyncWebServerRequest *request);
  void card           (AsyncWebServerRequest *request);

  void page_settings  (AsyncWebServerRequest *request);
  void update_settings(AsyncWebServerRequest *request);
  void get_settings   (AsyncWebServerRequest *request);

  void page_system    (AsyncWebServerRequest *request);
  void page_wifi      (AsyncWebServerRequest *request);
  void scan_networks  (AsyncWebServerRequest *request);


  void onWebSocketMessage(void *arg, uint8_t *data, size_t len);
  void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
                        void *arg, uint8_t *data, size_t len);
  String get_status   ();
  void push_status    ();
  String process_page (const String& var);

  bool connected = false;

  Settings&       settings;
  Mp3&            mp3;
  WiFiManager     wm        {};
  AsyncWebServer  webserver {80};
  AsyncWebSocket  ws        {"/ws"};
  String          old_status{};

  commandRaw      cmd{commandRaw::none};
};


#endif // SRC_WEBSERVICE_HPP_
