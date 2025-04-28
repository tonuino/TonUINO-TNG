#ifndef SRC_WEBSERVICE_HPP_
#define SRC_WEBSERVICE_HPP_

#include <Arduino.h>
#include <WiFi.h>
#include <DNSServer.h>
#include <ESPAsyncWebServer.h>
#include <Preferences.h>

#include "settings.hpp"
#include "commands.hpp"

class Mp3;

class WifiSettings {
public:
  WifiSettings() { }
  void init();
  void set(const String& ssid,
           const String& password,
           const String& hostname,
           bool static_ip,
           const String& static_ip_address,
           const String& static_ip_gw,
           const String& static_ip_subnet,
           const String& static_ip_dns1,
           const String& static_ip_dns2
           );
  String get_ssid             () { return ssid             ; }
  String get_password         () { return password         ; }
  String get_hostname         () { return hostname         ; }
  bool   get_static_ip        () { return static_ip        ; }
  String get_static_ip_address() { return static_ip_address; }
  String get_static_ip_gw     () { return static_ip_gw     ; }
  String get_static_ip_subnet () { return static_ip_subnet ; }
  String get_static_ip_dns1   () { return static_ip_dns1   ; }
  String get_static_ip_dns2   () { return static_ip_dns2   ; }
private:
  Preferences prefs            {};
  String      ssid             {};
  String      password         {};
  String      hostname         {};
  bool        static_ip        {false};
  String      static_ip_address{};
  String      static_ip_gw     {};
  String      static_ip_subnet {};
  String      static_ip_dns1   {};
  String      static_ip_dns2   {};
};

class Webservice: public CommandSource {

public:
  Webservice(Settings& settings, Mp3& mp3)
  : settings(settings)
  , mp3(mp3)
  {};

  void init();
  void loop();

  void push_shutdown  ();

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
  void wifi_save      (AsyncWebServerRequest *request);

  void page_info      (AsyncWebServerRequest *request);

  void page_upgrade   (AsyncWebServerRequest *request);
  void upgrade_start  (AsyncWebServerRequest *request);
  void onOtaUpload    (AsyncWebServerRequest *request, String filename, size_t index,
                       uint8_t *data, size_t len, bool f);

  void onWebSocketMessage(void *arg, uint8_t *data, size_t len);
  void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
                        void *arg, uint8_t *data, size_t len);
  String get_status   ();
  void push_status    ();
  String process_page (const String& var);
  String getInfoData  (const String& id);

  bool connected = false;

  Settings&       settings;
  Mp3&            mp3;
  WifiSettings    wifi_settings{};
  DNSServer       dns_server{};
  AsyncWebServer  webserver {80};
  AsyncWebSocket  ws        {"/ws"};
  String          old_status{};

  unsigned long   ota_progress_millis = 0;
  size_t          ota_progress_size = 0;
  bool            ota_reboot{false};



  commandRaw      cmd{commandRaw::none};
};


#endif // SRC_WEBSERVICE_HPP_
