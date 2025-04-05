#include "constants.hpp"
#ifdef TonUINO_Esp32

#include "webservice.hpp"

#include <ArduinoJson.h>
#include <Update.h>

#include "logger.hpp"
#include "state_machine.hpp"
#include "tonuino.hpp"
#include "timer.hpp"
#include "version.hpp"

#include "webservice_style.hpp"
#include "webservice_pages.hpp"

#define W_STRING2(x) #x
#define W_STRING(x) W_STRING2(x)

namespace {

const char ota_user  [] = "admin";
const char ota_passwd[] = "admin";

Tonuino        &tonuino   = Tonuino::getTonuino();

/**************
Static HTML Definition
**************/
int getRSSIasQuality(int RSSI) {
  int quality = 0;

  if (RSSI <= -100) {
    quality = 0;
  } else if (RSSI >= -50) {
    quality = 100;
  } else {
    quality = 2 * (RSSI + 100);
  }
  return quality;
}


} // namespace

void WifiSettings::init() {
  prefs.begin("wifi");
  if (not prefs.isKey("WiFi_ssid") or not prefs.isKey("WiFi_password" )) {
    LOG(webserv_log, s_info, "no wifi settings");
    set("", "");
  }
  ssid     = prefs.getString("WiFi_ssid"    );
  password = prefs.getString("WiFi_password");
  LOG(webserv_log, s_info, "wifi settings - ssid: ", ssid);
}

void WifiSettings::set(const String& n_ssid, const String& n_password) {
  ssid     = n_ssid;
  password = n_password;
  prefs.putString("WiFi_ssid"    , ssid    );
  prefs.putString("WiFi_password", password);
}

void Webservice::init() {

  wifi_settings.init();

  if ((digitalRead(buttonUpPin) != getLevel(buttonPinType, level::active)) && (wifi_settings.get_ssid() != "")) {
    WiFi.setHostname("tonuino");
    WiFi.mode(WIFI_MODE_STA);
    WiFi.begin(wifi_settings.get_ssid(), wifi_settings.get_password());
    LOG(webserv_log, s_info, "Connecting to WiFi ", wifi_settings.get_ssid(), " ...");
    Timer timer;
    timer.start(10*1000);
    while ((WiFi.status() != WL_CONNECTED) && not timer.isExpired()) {
      delay(500);
    }
  }
  if (WiFi.status() == WL_CONNECTED) {
    connected = true;
    LOG(webserv_log, s_info, "Connected to WiFi ", wifi_settings.get_ssid(), " with IP: ", WiFi.localIP());
  }
  else {
    connected = false;
    LOG(webserv_log, s_info, "Not connected to WiFi ", wifi_settings.get_ssid(), ", starting AP TonUINO");
    WiFi.softAPsetHostname("tonuino");
    WiFi.mode(WIFI_MODE_AP);
    WiFi.softAP("TonUINO");
    delay(500); // slight delay to make sure we get an AP IP
    LOG(webserv_log, s_info, "AP IP address: ",WiFi.softAPIP());
    dns_server.start(53, "*", WiFi.softAPIP());
  }

  webserial.begin(&webserver);

  ws.onEvent([this](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) { onWebSocketEvent(server, client, type, arg, data, len); });
  webserver.addHandler(&ws);

  webserver.begin();
  webserver.onNotFound(                       [this](AsyncWebServerRequest *request){ page_notfound  (request); });

  webserver.on("/style.css"      , HTTP_GET,  [this](AsyncWebServerRequest *request){ page_style_css (request); });

  webserver.on("/"               , HTTP_GET,  [this](AsyncWebServerRequest *request){ page_main      (request); });
  webserver.on("/service"        , HTTP_POST, [this](AsyncWebServerRequest *request){ service        (request); });
  webserver.on("/modifier"       , HTTP_POST, [this](AsyncWebServerRequest *request){ modifier       (request); });
  webserver.on("/card"           , HTTP_POST, [this](AsyncWebServerRequest *request){ card           (request); });

  webserver.on("/settings"       , HTTP_GET,  [this](AsyncWebServerRequest *request){ page_settings  (request); });
  webserver.on("/get_settings"   , HTTP_GET,  [this](AsyncWebServerRequest *request){ get_settings   (request); });
  webserver.on("/update_settings", HTTP_POST, [this](AsyncWebServerRequest *request){ update_settings(request); });

  webserver.on("/system"         , HTTP_GET,  [this](AsyncWebServerRequest *request){ page_system    (request); });

  webserver.on("/wifi"           , HTTP_GET,  [this](AsyncWebServerRequest *request){ page_wifi      (request); });
  webserver.on("/scan_networks"  , HTTP_GET,  [this](AsyncWebServerRequest *request){ scan_networks  (request); });
  webserver.on("/wifisave"       , HTTP_POST, [this](AsyncWebServerRequest *request){ wifi_save      (request); });

  webserver.on("/info"           , HTTP_GET,  [this](AsyncWebServerRequest *request){ page_info      (request); });

  webserver.on("/upgrade"        , HTTP_GET,  [this](AsyncWebServerRequest *request){ page_upgrade   (request); });
  webserver.on("/upgrade"        , HTTP_POST, [this](AsyncWebServerRequest *request){ upgrade_start  (request); },
                                              [this](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool f)
                                                                                    { onOtaUpload(request, filename, index, data, len, f); });
}

void Webservice::loop() {
  if (ota_reboot) {
    LOG(webserv_log, s_info, "OTA finished, Rebooting ESP32...");
    if (SM_tonuino::is_in_state<Play>())
      cmd = commandRaw::pause;
    delay(2000);
    ESP.restart();
  }
  if (not connected)
    dns_server.processNextRequest();
  ws.cleanupClients();
  webserial.loop();
  push_status();
}

commandRaw Webservice::getCommandRaw() {
  commandRaw ret = cmd;
  cmd = commandRaw::none;
  return ret;
}

void Webservice::page_notfound(AsyncWebServerRequest *request) {
  request->send(200, "text/html", notfound_html);
}

void Webservice::page_style_css(AsyncWebServerRequest *request)
{
  request->send(200, "text/css", style_css);
}

void Webservice::page_main(AsyncWebServerRequest *request)
{
  request->send(200, "text/html", main_html, [this](const String& var) { return process_page(var);});
}

void Webservice::page_settings(AsyncWebServerRequest *request) {
  request->send(200, "text/html", settings_html, [this](const String& var) { return process_page(var);});
}

void Webservice::page_system(AsyncWebServerRequest *request) {
  request->send(200, "text/html", system_html, [this](const String& var) { return process_page(var);});
}

void Webservice::page_wifi(AsyncWebServerRequest *request) {
  request->send(200, "text/html", wifi_html, [this](const String& var) { return process_page(var);});
}

void Webservice::page_info(AsyncWebServerRequest *request) {
  request->send(200, "text/html", info_html, [this](const String& var) { return process_page(var);});
}

void Webservice::page_upgrade(AsyncWebServerRequest *request) {
  if (!request->authenticate(ota_user, ota_passwd)) {
    request->requestAuthentication();
    return;
  }

  request->send(200, "text/html", upgrade_html, [this](const String& var) { return process_page(var);});
}

void Webservice::update_settings(AsyncWebServerRequest *request) {
  LOG(webserv_log, s_info, "Webservice::update_settings");

  int params = request->params();
  for (int i = 0; i < params; i++) {
    const AsyncWebParameter *p = request->getParam(i);
    LOG(webserv_log, s_info, "parameter name[", p->name(), "]: ", p->value());
  }
  if (request->hasArg("spkMaxVolume"        )) settings.spkMaxVolume         = request->arg("spkMaxVolume"        ).toInt();
  if (request->hasArg("spkMinVolume"        )) settings.spkMinVolume         = request->arg("spkMinVolume"        ).toInt();
  if (request->hasArg("spkInitVolume"       )) settings.spkInitVolume        = request->arg("spkInitVolume"       ).toInt();
  if (request->hasArg("hpMaxVolume"         )) settings.hpMaxVolume          = request->arg("hpMaxVolume"         ).toInt();
  if (request->hasArg("hpMinVolume"         )) settings.hpMinVolume          = request->arg("hpMinVolume"         ).toInt();
  if (request->hasArg("hpInitVolume"        )) settings.hpInitVolume         = request->arg("hpInitVolume"        ).toInt();
  if (request->hasArg("eq"                  )) settings.eq                   = request->arg("eq"                  ) == "Normal"  ? 1 :
                                                                               request->arg("eq"                  ) == "Pop"     ? 2 :
                                                                               request->arg("eq"                  ) == "Rock"    ? 3 :
                                                                               request->arg("eq"                  ) == "Jazz"    ? 4 :
                                                                               request->arg("eq"                  ) == "Classic" ? 5 :
                                                                               request->arg("eq"                  ) == "Bass"    ? 6 : 1;
  if (request->hasArg("standbyTimer"        )) settings.standbyTimer         = request->arg("standbyTimer"        ).toInt();
  if (request->hasArg("invertVolumeButtons" )) settings.invertVolumeButtons  = 1; else settings.invertVolumeButtons = 0;
  if (request->hasArg("adminMenuLocked"     )) settings.adminMenuLocked      = request->arg("adminMenuLocked"     ) == "kein Lock"? 0 :
                                                                               request->arg("adminMenuLocked"     ) == "nur Karte"? 1 :
                                                                               request->arg("adminMenuLocked"     ) == "Pin"      ? 2 : 0;
  if (request->hasArg("adminMenuPin1"       )) settings.adminMenuPin[0]      = request->arg("adminMenuPin1"       ).toInt();
  if (request->hasArg("adminMenuPin2"       )) settings.adminMenuPin[1]      = request->arg("adminMenuPin2"       ).toInt();
  if (request->hasArg("adminMenuPin3"       )) settings.adminMenuPin[2]      = request->arg("adminMenuPin3"       ).toInt();
  if (request->hasArg("adminMenuPin4"       )) settings.adminMenuPin[3]      = request->arg("adminMenuPin4"       ).toInt();
  if (request->hasArg("pauseWhenCardRemoved")) settings.pauseWhenCardRemoved = 1; else settings.pauseWhenCardRemoved = 0;

  if (request->hasArg("sc_mode1"            )) settings.shortCuts[0].mode    = request->arg("sc_mode1"            ) == "--------"        ? pmode_t::none     :
                                                                               request->arg("sc_mode1"            ) == "Hörspiel"        ? pmode_t::hoerspiel    :
                                                                               request->arg("sc_mode1"            ) == "Album"           ? pmode_t::album        :
                                                                               request->arg("sc_mode1"            ) == "Party"           ? pmode_t::party        :
                                                                               request->arg("sc_mode1"            ) == "Einzel"          ? pmode_t::einzel       :
                                                                               request->arg("sc_mode1"            ) == "Hörbuch"         ? pmode_t::hoerbuch     :
                                                                               request->arg("sc_mode1"            ) == "Hörspiel von bis"? pmode_t::hoerspiel_vb :
                                                                               request->arg("sc_mode1"            ) == "Album von bis"   ? pmode_t::album_vb     :
                                                                               request->arg("sc_mode1"            ) == "Party von bis"   ? pmode_t::party_vb     :
                                                                               request->arg("sc_mode1"            ) == "Hörbuch einzel"  ? pmode_t::hoerbuch_1   :
                                                                               request->arg("sc_mode1"            ) == "Wiederhole"      ? pmode_t::repeat_last  :
                                                                               request->arg("sc_mode1"            ) == "Quiz Spiel"      ? pmode_t::quiz_game    :
                                                                               request->arg("sc_mode1"            ) == "Memory Spiel"    ? pmode_t::memory_game  :
                                                                                                                                           pmode_t::none         ;
  if (request->hasArg("sc_folder1"          )) settings.shortCuts[0].folder  = request->arg("sc_folder1"          ).toInt();
  if (request->hasArg("sc_special11"        )) settings.shortCuts[0].special = request->arg("sc_special11"        ).toInt();
  if (request->hasArg("sc_Special21"        )) settings.shortCuts[0].special2= request->arg("sc_special21"        ).toInt();

  if (request->hasArg("sc_mode2"            )) settings.shortCuts[1].mode    = request->arg("sc_mode2"            ) == "--------"        ? pmode_t::none     :
                                                                               request->arg("sc_mode2"            ) == "Hörspiel"        ? pmode_t::hoerspiel    :
                                                                               request->arg("sc_mode2"            ) == "Album"           ? pmode_t::album        :
                                                                               request->arg("sc_mode2"            ) == "Party"           ? pmode_t::party        :
                                                                               request->arg("sc_mode2"            ) == "Einzel"          ? pmode_t::einzel       :
                                                                               request->arg("sc_mode2"            ) == "Hörbuch"         ? pmode_t::hoerbuch     :
                                                                               request->arg("sc_mode2"            ) == "Hörspiel von bis"? pmode_t::hoerspiel_vb :
                                                                               request->arg("sc_mode2"            ) == "Album von bis"   ? pmode_t::album_vb     :
                                                                               request->arg("sc_mode2"            ) == "Party von bis"   ? pmode_t::party_vb     :
                                                                               request->arg("sc_mode2"            ) == "Hörbuch einzel"  ? pmode_t::hoerbuch_1   :
                                                                               request->arg("sc_mode2"            ) == "Wiederhole"      ? pmode_t::repeat_last  :
                                                                               request->arg("sc_mode2"            ) == "Quiz Spiel"      ? pmode_t::quiz_game    :
                                                                               request->arg("sc_mode2"            ) == "Memory Spiel"    ? pmode_t::memory_game  :
                                                                                                                                           pmode_t::none         ;
  if (request->hasArg("sc_folder2"          )) settings.shortCuts[1].folder  = request->arg("sc_folder2"          ).toInt();
  if (request->hasArg("sc_special12"        )) settings.shortCuts[1].special = request->arg("sc_special12"        ).toInt();
  if (request->hasArg("sc_Special22"        )) settings.shortCuts[1].special2= request->arg("sc_special22"        ).toInt();

  if (request->hasArg("sc_mode3"            )) settings.shortCuts[2].mode    = request->arg("sc_mode3"            ) == "--------"        ? pmode_t::none     :
                                                                               request->arg("sc_mode3"            ) == "Hörspiel"        ? pmode_t::hoerspiel    :
                                                                               request->arg("sc_mode3"            ) == "Album"           ? pmode_t::album        :
                                                                               request->arg("sc_mode3"            ) == "Party"           ? pmode_t::party        :
                                                                               request->arg("sc_mode3"            ) == "Einzel"          ? pmode_t::einzel       :
                                                                               request->arg("sc_mode3"            ) == "Hörbuch"         ? pmode_t::hoerbuch     :
                                                                               request->arg("sc_mode3"            ) == "Hörspiel von bis"? pmode_t::hoerspiel_vb :
                                                                               request->arg("sc_mode3"            ) == "Album von bis"   ? pmode_t::album_vb     :
                                                                               request->arg("sc_mode3"            ) == "Party von bis"   ? pmode_t::party_vb     :
                                                                               request->arg("sc_mode3"            ) == "Hörbuch einzel"  ? pmode_t::hoerbuch_1   :
                                                                               request->arg("sc_mode3"            ) == "Wiederhole"      ? pmode_t::repeat_last  :
                                                                               request->arg("sc_mode3"            ) == "Quiz Spiel"      ? pmode_t::quiz_game    :
                                                                               request->arg("sc_mode3"            ) == "Memory Spiel"    ? pmode_t::memory_game  :
                                                                                                                                           pmode_t::none         ;
  if (request->hasArg("sc_folder3"          )) settings.shortCuts[2].folder  = request->arg("sc_folder3"          ).toInt();
  if (request->hasArg("sc_special13"        )) settings.shortCuts[2].special = request->arg("sc_special13"        ).toInt();
  if (request->hasArg("sc_Special23"        )) settings.shortCuts[2].special2= request->arg("sc_special23"        ).toInt();

  if (request->hasArg("sc_mode4"            )) settings.shortCuts[3].mode    = request->arg("sc_mode4"            ) == "--------"        ? pmode_t::none         :
                                                                               request->arg("sc_mode4"            ) == "Hörspiel"        ? pmode_t::hoerspiel    :
                                                                               request->arg("sc_mode4"            ) == "Album"           ? pmode_t::album        :
                                                                               request->arg("sc_mode4"            ) == "Party"           ? pmode_t::party        :
                                                                               request->arg("sc_mode4"            ) == "Einzel"          ? pmode_t::einzel       :
                                                                               request->arg("sc_mode4"            ) == "Hörbuch"         ? pmode_t::hoerbuch     :
                                                                               request->arg("sc_mode4"            ) == "Hörspiel von bis"? pmode_t::hoerspiel_vb :
                                                                               request->arg("sc_mode4"            ) == "Album von bis"   ? pmode_t::album_vb     :
                                                                               request->arg("sc_mode4"            ) == "Party von bis"   ? pmode_t::party_vb     :
                                                                               request->arg("sc_mode4"            ) == "Hörbuch einzel"  ? pmode_t::hoerbuch_1   :
                                                                               request->arg("sc_mode4"            ) == "Wiederhole"      ? pmode_t::repeat_last  :
                                                                               request->arg("sc_mode4"            ) == "Quiz Spiel"      ? pmode_t::quiz_game    :
                                                                               request->arg("sc_mode4"            ) == "Memory Spiel"    ? pmode_t::memory_game  :
                                                                                                                                           pmode_t::none         ;
  if (request->hasArg("sc_folder4"          )) settings.shortCuts[3].folder  = request->arg("sc_folder4"          ).toInt();
  if (request->hasArg("sc_special14"        )) settings.shortCuts[3].special = request->arg("sc_special14"        ).toInt();
  if (request->hasArg("sc_Special24"        )) settings.shortCuts[3].special2= request->arg("sc_special24"        ).toInt();

  // check values
  if (settings.spkMaxVolume  > 25                   ) settings.spkMaxVolume  = 25;
  if (settings.spkMinVolume  > settings.spkMaxVolume) settings.spkMinVolume  = settings.spkMaxVolume;
  if (settings.spkInitVolume > settings.spkMaxVolume) settings.spkInitVolume = settings.spkMaxVolume;
  if (settings.spkInitVolume < settings.spkMinVolume) settings.spkInitVolume = settings.spkMinVolume;

  if (settings.standbyTimer > 60) settings.standbyTimer = 60;

  for (uint8_t i = 0; i <= 3; ++i) {
    if (settings.adminMenuPin[i] > 3) settings.adminMenuPin[i] = 3;
    if (settings.adminMenuPin[i] < 1) settings.adminMenuPin[i] = 1;
  }

  if (settings.hpMaxVolume  > 25                   ) settings.hpMaxVolume  = 25;
  if (settings.hpMinVolume  > settings.hpMaxVolume) settings.hpMinVolume  = settings.hpMaxVolume;
  if (settings.hpInitVolume > settings.hpMaxVolume) settings.hpInitVolume = settings.hpMaxVolume;
  if (settings.hpInitVolume < settings.hpMinVolume) settings.hpInitVolume = settings.hpMinVolume;

  // update real values
  if (mp3.getVolume() > mp3.getMaxVolume()) mp3.setVolume(mp3.getMaxVolume());
  if (mp3.getVolume() < mp3.getMinVolume()) mp3.setVolume(mp3.getMinVolume());
  mp3.setEq(static_cast<DfMp3_Eq>(settings.eq - 1));

  settings.writeSettingsToFlash();

  AsyncWebServerResponse *response = request->beginResponse(302, "text/plain", "Moved");
  response->addHeader("Location", "/settings");
  request->send(response);
}

void Webservice::get_settings(AsyncWebServerRequest *request) {

  LOG(webserv_log, s_info, "Webservice::get_settings");

  String jsonStr;

  JsonDocument doc;
  doc["version"             ] = String(settings.version             );
  doc["spkMaxVolume"        ] = String(settings.spkMaxVolume        );
  doc["spkMinVolume"        ] = String(settings.spkMinVolume        );
  doc["spkInitVolume"       ] = String(settings.spkInitVolume       );
  doc["hpMaxVolume"         ] = String(settings.hpMaxVolume         );
  doc["hpMinVolume"         ] = String(settings.hpMinVolume         );
  doc["hpInitVolume"        ] = String(settings.hpInitVolume        );
  doc["eq"                  ] = settings.eq==1 ? "Normal":
                                settings.eq==2 ? "Pop"    :
                                settings.eq==3 ? "Rock"   :
                                settings.eq==4 ? "Jazz"   :
                                settings.eq==5 ? "Classic":
                                settings.eq==6 ? "Bass"   : "?";
  doc["standbyTimer"        ] = String(settings.standbyTimer        );
  doc["invertVolumeButtons" ] = String(settings.invertVolumeButtons );
  doc["adminMenuLocked"     ] = settings.adminMenuLocked==0 ? "kein Lock":
                                settings.adminMenuLocked==1 ? "nur Karte":
                                settings.adminMenuLocked==2 ? "Pin"      : "?";
  doc["adminMenuPin1"       ] = String(settings.adminMenuPin[0]     );
  doc["adminMenuPin2"       ] = String(settings.adminMenuPin[1]     );
  doc["adminMenuPin3"       ] = String(settings.adminMenuPin[2]     );
  doc["adminMenuPin4"       ] = String(settings.adminMenuPin[3]     );
  doc["pauseWhenCardRemoved"] = String(settings.pauseWhenCardRemoved);

  for (uint8_t i = 1; i <= 4; ++i) {
    doc["sc_mode"+String(i)   ] = settings.shortCuts[i-1].mode == pmode_t::none         ? "--------"        :
                                  settings.shortCuts[i-1].mode == pmode_t::hoerspiel    ? "Hörspiel"        :
                                  settings.shortCuts[i-1].mode == pmode_t::album        ? "Album"           :
                                  settings.shortCuts[i-1].mode == pmode_t::party        ? "Party"           :
                                  settings.shortCuts[i-1].mode == pmode_t::einzel       ? "Einzel"          :
                                  settings.shortCuts[i-1].mode == pmode_t::hoerbuch     ? "Hörbuch"         :
                                  settings.shortCuts[i-1].mode == pmode_t::hoerspiel_vb ? "Hörspiel von bis":
                                  settings.shortCuts[i-1].mode == pmode_t::album_vb     ? "Album von bis"   :
                                  settings.shortCuts[i-1].mode == pmode_t::party_vb     ? "Party von bis"   :
                                  settings.shortCuts[i-1].mode == pmode_t::hoerbuch_1   ? "Hörbuch einzel"  :
                                  settings.shortCuts[i-1].mode == pmode_t::repeat_last  ? "Wiederhole"      :
                                  settings.shortCuts[i-1].mode == pmode_t::quiz_game    ? "Quiz Spiel"      :
                                  settings.shortCuts[i-1].mode == pmode_t::memory_game  ? "Memory Spiel"    :
                                                                                          "--------"        ;
    doc["sc_folder"+String(i)  ] = String(settings.shortCuts[i-1].folder  );
    doc["sc_special1"+String(i)] = String(settings.shortCuts[i-1].special );
    doc["sc_special2"+String(i)] = String(settings.shortCuts[i-1].special2);
  }

  serializeJson(doc, jsonStr);

  LOG(webserv_log, s_debug, "jsonStr (", jsonStr, ")");
  request->send(200, "text/html", jsonStr);
}

void Webservice::service(AsyncWebServerRequest *request) {
  LOG(webserv_log, s_info, "Webservice::service");

  int params = request->params();
  for (int i = 0; i < params; i++) {
    const AsyncWebParameter *p = request->getParam(i);
    LOG(webserv_log, s_info, "parameter name[", p->name(), "]: ", p->value());
  }

  if (request->hasArg("button")) {
    if      (request->arg("button") == "down"      ) cmd = commandRaw::down;
    else if (request->arg("button") == "pause"     ) cmd = commandRaw::pause;
    else if (request->arg("button") == "up"        ) cmd = commandRaw::up;
    else if (request->arg("button") == "down_long" ) cmd = commandRaw::downLong;
    else if (request->arg("button") == "pause_long") cmd = commandRaw::pauseLong;
    else if (request->arg("button") == "up_long"   ) cmd = commandRaw::upLong;
#ifdef FIVEBUTTONS
    else if (request->arg("button") == "five"      ) cmd = commandRaw::five;
    else if (request->arg("button") == "four"      ) cmd = commandRaw::four;
    else if (request->arg("button") == "five_long" ) cmd = commandRaw::fiveLong;
    else if (request->arg("button") == "four_long" ) cmd = commandRaw::fourLong;
#endif
  }
  if (request->hasArg("command")) {
    if      (request->arg("command") == "shutdown" ) {
      if (SM_tonuino::is_in_state<Play>())
        cmd = commandRaw::pause;
      delay(1000);
      tonuino.shutdown();
    }
  }
  request->send(200);
}

void Webservice::modifier(AsyncWebServerRequest *request) {
  LOG(webserv_log, s_info, "Webservice::modifier");

  int params = request->params();
  for (int i = 0; i < params; i++) {
    const AsyncWebParameter *p = request->getParam(i);
    LOG(webserv_log, s_info, "parameter name[", p->name(), "]: ", p->value());
  }

  folderSettings mod;
  mod.folder   = 0;
  mod.special2 = 0;
  mod.special  = request->arg("mod_special").toInt();
  mod.mode     = request->arg("mod_mode") == "Sleep-Timer"       ? pmode_t::sleep_timer   :
                 request->arg("mod_mode") == "Stopptanz"         ? pmode_t::freeze_dance  :
                 request->arg("mod_mode") == "Feuer-Wasser-Luft" ? pmode_t::fi_wa_ai      :
                 request->arg("mod_mode") == "Gesperrt"          ? pmode_t::toddler       :
                 request->arg("mod_mode") == "Kita Modus"        ? pmode_t::kindergarden  :
                 request->arg("mod_mode") == "Wiederhole Track"  ? pmode_t::repeat_single :
                                                                   pmode_t::none          ;

  if (request->arg("mod_action") == "delete") {
    LOG(webserv_log, s_info, "delete modifier");
    tonuino.resetActiveModifier();
  }
  else if (request->arg("mod_action") == "activate") {
    LOG(webserv_log, s_info, "activate modifier mode: ", static_cast<uint8_t>(mod.mode), " special: ", mod.special);
    tonuino.specialCard(mod);
  }
  else if (request->arg("mod_action") == "write") {
    LOG(webserv_log, s_info, "write modifier mode: ", static_cast<uint8_t>(mod.mode), " special: ", mod.special);
    settings.setShortCut(0, mod);
    cmd = commandRaw::write_card_from_web;
  }


  request->send(200);
}

void Webservice::card(AsyncWebServerRequest *request) {
  LOG(webserv_log, s_info, "Webservice::card");

  int params = request->params();
  for (int i = 0; i < params; i++) {
    const AsyncWebParameter *p = request->getParam(i);
    LOG(webserv_log, s_info, "parameter name[", p->name(), "]: ", p->value());
  }

  folderSettings card;
  card.folder   = request->arg("folder").toInt();;
  card.special  = request->arg("special1").toInt();
  card.special2 = request->arg("special2").toInt();
  card.mode     = request->arg("mode") == "Hörspiel"         ? pmode_t::hoerspiel    :
                  request->arg("mode") == "Album"            ? pmode_t::album        :
                  request->arg("mode") == "Party"            ? pmode_t::party        :
                  request->arg("mode") == "Einzel"           ? pmode_t::einzel       :
                  request->arg("mode") == "Hörbuch"          ? pmode_t::hoerbuch     :
                  request->arg("mode") == "Hörspiel von bis" ? pmode_t::hoerspiel_vb :
                  request->arg("mode") == "Album von bis"    ? pmode_t::album_vb     :
                  request->arg("mode") == "Party von bis"    ? pmode_t::party_vb     :
                  request->arg("mode") == "Hörbuch einzel"   ? pmode_t::hoerbuch_1   :
                  request->arg("mode") == "Wiederhole"       ? pmode_t::repeat_last  :
                  request->arg("mode") == "Quiz Spiel"       ? pmode_t::quiz_game    :
                  request->arg("mode") == "Memory Spiel"     ? pmode_t::memory_game  :
                                                               pmode_t::none         ;

  if (request->arg("card_action") == "start") {
    LOG(webserv_log, s_info, "start card mode: ", static_cast<uint8_t>(card.mode), " folder: ", card.folder, " special: ", card.special, " special2: ", card.special2);
    settings.setShortCut(0, card);
    cmd = commandRaw::card_from_web;
  }
  else if (request->arg("card_action") == "write") {
    LOG(webserv_log, s_info, "write card mode: ", static_cast<uint8_t>(card.mode), " folder: ", card.folder, " special: ", card.special, " special2: ", card.special2);
    settings.setShortCut(0, card);
    cmd = commandRaw::write_card_from_web;
  }

  request->send(200);
}

void Webservice::onWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    String text(data, len);
    LOG(webserv_log, s_debug, "websocket message: ", text);
  }
}

void Webservice::onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
                                  void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_DISCONNECT:
      LOG(webserv_log, s_info, "websocket [", client->id(), "] Disconnected!");
      break;
    case WS_EVT_CONNECT:
      LOG(webserv_log, s_info, "websocket [", client->id(), "] Connected!");
      old_status = "";
      break;
    case WS_EVT_DATA:
      onWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
    case WS_EVT_PING:
      break;
  }
}

String Webservice::get_status() {
  String status;

  status += String("State: ") + SM_tonuino::state_str;
  if (mp3.isPlayingFolder()) {
    String mode = (tonuino.getMyFolder().mode == pmode_t::hoerspiel   ) ? String("Hörspiel"        ) :
                  (tonuino.getMyFolder().mode == pmode_t::album       ) ? String("Album"           ) :
                  (tonuino.getMyFolder().mode == pmode_t::party       ) ? String("Party"           ) :
                  (tonuino.getMyFolder().mode == pmode_t::einzel      ) ? String("Einzel"          ) :
                  (tonuino.getMyFolder().mode == pmode_t::hoerbuch    ) ? String("Hörbuch"         ) :
                  (tonuino.getMyFolder().mode == pmode_t::hoerspiel_vb) ? String("Hörspiel von bis") :
                  (tonuino.getMyFolder().mode == pmode_t::album_vb    ) ? String("Album von bis"   ) :
                  (tonuino.getMyFolder().mode == pmode_t::party_vb    ) ? String("Party von bis"   ) :
                  (tonuino.getMyFolder().mode == pmode_t::hoerbuch_1  ) ? String("Hörbuch einzel"  ) :
                  (tonuino.getMyFolder().mode == pmode_t::repeat_last ) ? String("Wiederhole"      ) :
                  (tonuino.getMyFolder().mode == pmode_t::quiz_game   ) ? String("Quiz Spiel"      ) :
                  (tonuino.getMyFolder().mode == pmode_t::memory_game ) ? String("Memory Spiel"    ) :String("?");
    status += String(" (Mode: ") + mode + String(", Folder: ") + String(mp3.getCurrentFolder()) + String(")");
  }

  status += String("<br>Queue: ") + mp3.getQueue();

  status += String("<br>Volume: ") + String(mp3.getVolume());
#ifdef HPJACKDETECT
  status += mp3.isHeadphoneJackDetect() ? String(" (Kopfhörer)") : String(" (Lautsprecher)");
#endif
  String active_modifier;
  switch (tonuino.getActiveModifier().getActive()) {
  case pmode_t::none:          active_modifier = "kein"             ; break;
  case pmode_t::sleep_timer  : active_modifier = "Sleep-Timer"      ; break;
  case pmode_t::freeze_dance : active_modifier = "Stopptanz"        ; break;
  case pmode_t::fi_wa_ai     : active_modifier = "Feuer-Wasser-Luft"; break;
  case pmode_t::toddler      : active_modifier = "Gesperrt"         ; break;
  case pmode_t::kindergarden : active_modifier = "Kita Modus"       ; break;
  case pmode_t::repeat_single: active_modifier = "Wiederhole Track" ; break;
  default                    : active_modifier = "kein"             ; break;
  }

  status += String("<br>Modifier: ") + active_modifier;

  return status;
}

void Webservice::push_status() {

  if (ws.count() == 0)
    return;

  String status = get_status();;

  if (old_status != status) {
    LOG(webserv_log, s_info, "Webservice::push_status");

    LOG(webserv_log, s_debug, "push_status() status: \n", status);

    old_status = status;
    ws.textAll(status);
  }
}

void Webservice::scan_networks(AsyncWebServerRequest *request) {

  LOG(webserv_log, s_info, "Webservice::scan_networks");

  String jsonStr;

  JsonDocument doc;

  JsonArray networks = doc["networks"].to<JsonArray>();

  int16_t num_networks = WiFi.scanNetworks();
  LOG(webserv_log, s_info, "networks found: ", num_networks);

  int indices[num_networks];
  for (int i = 0; i < num_networks; i++) {
    indices[i] = i;
  }

  // RSSI SORT
  for (int i = 0; i < num_networks; i++) {
    for (int j = i + 1; j < num_networks; j++) {
      if (WiFi.RSSI(indices[j]) > WiFi.RSSI(indices[i])) {
        std::swap(indices[i], indices[j]);
      }
    }
  }

  // remove duplicates
  String cssid;
  for (int i = 0; i < num_networks; i++) {
    if (indices[i] == -1) continue;
    cssid = WiFi.SSID(indices[i]);
    for (int j = i + 1; j < num_networks; j++) {
      if (cssid == WiFi.SSID(indices[j])) {
        LOG(webserv_log, s_debug, "DUP AP: ",WiFi.SSID(indices[j]));
        indices[j] = -1;
      }
    }
  }

  int n = 0;
  for (int i = 0; i < num_networks; i++) {
    if (indices[i] == -1) continue; // skip dups

    int rssiperc = getRSSIasQuality(WiFi.RSSI(indices[i]));

    networks[n]["ssid"] = WiFi.SSID(indices[i]);
    networks[n]["rssi"] = (String)rssiperc;
    networks[n]["q"   ] = (String)int(round(map(rssiperc,0,100,1,4)));
    networks[n]["auth"] = WiFi.encryptionType(indices[i]) == WIFI_AUTH_OPEN ? "" : "l";

    ++n;
  }

  serializeJson(doc, jsonStr);

  LOG(webserv_log, s_debug, "jsonStr (", jsonStr, ")");
  request->send(200, "text/html", jsonStr);
}

void Webservice::wifi_save(AsyncWebServerRequest *request) {
  LOG(webserv_log, s_info, "Webservice::wifi_save");

  int params = request->params();
  for (int i = 0; i < params; i++) {
    const AsyncWebParameter *p = request->getParam(i);
    LOG(webserv_log, s_info, "parameter name[", p->name(), "]: ", p->value());
  }
  if (request->hasArg("ssid" ) && request->hasArg("password"))
    wifi_settings.set(request->arg("ssid"), request->arg("password"));

  if (request->hasArg("reboot" ) && request->arg("reboot") == "on")
    ota_reboot = true;

  AsyncWebServerResponse *response = request->beginResponse(302, "text/plain", "Moved");
  response->addHeader("Location", "/wifi");
  request->send(response);
}

void Webservice::upgrade_start(AsyncWebServerRequest *request) {
  LOG(webserv_log, s_info, "Webservice::upgrade_start");

  int params = request->params();
  for (int i = 0; i < params; i++) {
    const AsyncWebParameter *p = request->getParam(i);
    LOG(webserv_log, s_info, "parameter name[", p->name(), "]: ", p->value());
  }

  request->send(200);
}

void Webservice::onOtaUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool f) {
  LOG(webserv_log, s_debug, "Webservice::onOtaUpload");

  if (!request->authenticate(ota_user, ota_passwd)) {
    request->requestAuthentication();
    return;
  }

  if (!index) {
    ota_progress_size = 0;
    if (!Update.begin(UPDATE_SIZE_UNKNOWN, U_FLASH)) {
      // Save error to string
      StreamString str;
      Update.printError(str);
      LOG(webserv_log, s_error, "Failed to start update process: ", str.c_str());
      request->send(400, "text/plain", "Failed to start update process");
      return;
    }
  }

  if(len){
    if (Update.write(data, len) != len) {
        LOG(webserv_log, s_error, "Failed to write chunked data to free space");
        request->send(400, "text/plain", "Failed to write chunked data to free space");
        return;
    }
    ota_progress_size += len;
    if (millis() - ota_progress_millis > 1000) {
      ota_progress_millis = millis();
      LOG(webserv_log, s_info, "OTA Progress Current: ", ota_progress_size, " bytes, Final: ", request->contentLength(), " bytes");
    }
  }

  if (f) {
    if (!Update.end(true)) {
        StreamString str;
        Update.printError(str);
        LOG(webserv_log, s_error, "Failed to end update process: ", str.c_str());
        request->send(400, "text/plain", "Failed to end update process");
    }
    ota_reboot = true;
  }
}


String Webservice::process_page(const String& var) {

  if (var == "STATE")
    return get_status();
  else if (var == "TOPNAV")
    return topnav_html;
  else if (var == "SERVICE") {
#ifdef FIVEBUTTONS
    return service_button_5_html;
#else
    if (settings.invertVolumeButtons)
      return service_button_3_inv_html;
    else
      return service_button_3_html;
#endif
  }
  else if (var == "SSID")
    return wifi_settings.get_ssid();
  else if (var == "PASSWORD")
    return "";

  return getInfoData(var);

}

String Webservice::getInfoData(const String& id){

  String p;
  if(id==F("esphead")){
    p = ESP.getChipModel();
  }
  else if(id==F("wifihead")){
    const char* const WIFI_MODES[] PROGMEM = { "NULL", "STA", "AP", "STA+AP" };
    p = WiFi.getMode()<=3? WIFI_MODES[WiFi.getMode()] : "n/a";
  }
  else if(id==F("uptime")){
    int64_t seconds = esp_timer_get_time()  / 1000000ULL;
    p = (String)(seconds / 60) + " mins " + (String)(seconds % 60) + " secs";
  }
  else if(id==F("chipid")){
    p = String(ESP.getEfuseMac(),HEX);
  }
  else if(id==F("chiprev")){
      p = ESP.getChipRevision();
  }
  else if(id==F("idesize")){
    p = ESP.getFlashChipSize();
  }
  else if(id==F("flashsize")){
      p = ESP.getPsramSize();
  }
  else if(id==F("cpufreq")){
    p = ESP.getCpuFreqMHz();
  }
  else if(id==F("freeheap")){
    p = ESP.getFreeHeap();
  }
  else if(id==F("memsketch1")){
    p = ESP.getSketchSize();
  }
  else if(id==F("memsketch2")){
    p = ESP.getSketchSize()+ESP.getFreeSketchSpace();
  }
  else if(id==F("memsmeter1")){
    p = ESP.getSketchSize();
  }
  else if(id==F("memsmeter2")){
    p = ESP.getSketchSize()+ESP.getFreeSketchSpace();
  }
  else if(id==F("apip")){
    p = WiFi.softAPIP().toString();
  }
  else if(id==F("apmac")){
    p = WiFi.softAPmacAddress();
  }
  else if(id==F("aphost")){
      p = WiFi.softAPgetHostname();
  }
  else if(id==F("apbssid")){
    p = WiFi.BSSIDstr();
  }
  else if(id==F("stassid")){
    p = wifi_settings.get_ssid();
  }
  else if(id==F("staip")){
    p = WiFi.localIP().toString();
  }
  else if(id==F("stagw")){
    p = WiFi.gatewayIP().toString();
  }
  else if(id==F("stasub")){
    p = WiFi.subnetMask().toString();
  }
  else if(id==F("dnss")){
    p = WiFi.dnsIP().toString();
  }
  else if(id==F("host")){
    p = WiFi.getHostname();
  }
  else if(id==F("stamac")){
    p = WiFi.macAddress();
  }
  else if(id==F("conx")){
    p = WiFi.isConnected() ? "yes" : "no";
  }
  else if(id==F("temp1")){
    p = temperatureRead();
  }
  else if(id==F("aboutver")){
    p = FPSTR(TONUINO_TNG_VERSION);
  }
  else if(id==F("aboutarduinover")){
    p = String(W_STRING(ESP_ARDUINO_VERSION_MAJOR)  "."  W_STRING(ESP_ARDUINO_VERSION_MINOR)  "."  W_STRING(ESP_ARDUINO_VERSION_PATCH));
  }
  else if(id==F("aboutsdkver")){
    p = esp_get_idf_version();
  }
  else if(id==F("aboutdate")){
    p = String(__DATE__ " " __TIME__);
  }
  return p;
}


#endif
