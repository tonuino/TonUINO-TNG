#ifdef TonUINO_Esp32

#include "webservice.hpp"

#include <ArduinoJson.h>

#include "logger.hpp"
#include "state_machine.hpp"
#include "tonuino.hpp"

namespace {

Tonuino        &tonuino   = Tonuino::getTonuino();

/**************
Static HTML Definition
**************/

const char style_css[] PROGMEM = R"rawliteral(

@media (min-width: 768px) {
    body {
        font-family: Arial, Helvetica, sans-serif;
        background-color: #99ccff;
        font-size: 14px;
    }
}
@media (max-width: 767px) {
    body {
        font-family: Arial, Helvetica, sans-serif;
        background-color: #99ccff;
        font-size: 16px;
    }
}

.topnav {
  overflow: hidden;
  background-color: #333;
  position: relative;
}

.topnav #nav_links {
  display: none;
}

.topnav a {
  color: white;
  padding: 14px 16px;
  text-decoration: none;
  font-size: 17px;
  display: block;
}

.topnav a.icon {
  background: black;
  display: block;
  position: absolute;
  right: 0;
  top: 0;
}

.topnav a:hover {
  background-color: #ddd;
  color: black;
}

.active {
  background-color: #ff9900;
  color: white;
}

input[type=number] {
  width: 3ch;
  display: inline-block
  padding: 2px;
  -moz-appearance: textfield;
}

input[type=number]::-webkit-inner-spin-button,
    input[type=number]::-webkit-outer-spin-button {
    -webkit-appearance: none;
     margin: 0;
}

.as_input_number {
  width: 3ch;
  display: inline-block;
  text-align: center;
  padding: 2px;
}

input[type=text] {
  width: 20ch;
  display: inline-block
  padding: 2px;
}

input[type=submit] {
  width: 8ch;
  height: 4ch;
}

label {
  width: 18ch;
  display: inline-block;
}

select {
  height: 4ch;
  margin: .3em;
}

.service_button_5 button {
  width: 7ch;
  height: 5ch;
}

.service_button_3 button {
  width: 7ch;
  height: 5ch;
}

.service1 button {
  margin-top: 2em;
  width: 9ch;
  height: 5ch;
}

button + button {
  margin-left: 1ch;
}

.status {
  width: auto;
  height: auto;
  border-style: solid;
  border-width: 1px;
  margin: .3em;
  text-align: left;
  padding: .3em;
  background-color: #cccccc
}

.service_button_5 {
  display: block;
  border: none;
  padding: .3em 1ch;
  text-align: left;
}

.service_button_3 {
  display: block;
  border: none;
  padding: .3em 8ch;
  text-align: left;
}


.service1 {
  display: block;
  width: 95%;
  border: none;
  text-align: left;
}

.tooltip {
    display: inline-block; 
    position: relative;
}
.tooltip .tooltiptext {
    visibility: hidden;
    background-color: #292929;
    color: #fff;
    text-align: left;
    border-radius:6px;
    padding: .5rem;
    position: absolute;
    z-index: 1;
    bottom: 130%;
    right: -5ch;
    left: -18ch;
    opacity: 0;
    transition: opacity 0.5s;
}
.tooltip .tooltiptext::after {
    content: "";
    position: absolute;
    top: 100%;
    left: 75%;  
    margin-left: -5px;
    border-width: 5px;
    border-style: solid;
    border-color: #292929 transparent transparent transparent;
}
.tooltip:hover .tooltiptext {
    visibility: visible;
    opacity: 1;
}

)rawliteral";

const char notfound_html[] PROGMEM = R"rawliteral(

<!DOCTYPE html>
Ooups - page not found (404)";
<script>
  async function tryRedirect() {
    try {
      var url = "/";
      const response = await fetch(url);
      window.location.href = url;
    }
    catch (error) {
      console.log(error);
      setTimeout(tryRedirect, 2000);
    }
  }
  tryRedirect();
</script>";

)rawliteral";

const char topnav_html[] PROGMEM = R"rawliteral(

<!-- Top Navigation Menu -->
<div class="topnav">
  <a href="/" class="active">TonUINO</a>
  <div id="nav_links">
    <a href="/">Home</a>
    <a href="/settings">Einstellungen</a>
  </div>
  <a href="javascript:void(0);" class="icon" onclick="show_hide_nav()">
    <i class="fa fa-bars"></i>
  </a>
</div>
<script>
function show_hide_nav() {
  var x = document.getElementById("nav_links");
  if (x.style.display === "block") {
    x.style.display = "none";
  } else {
    x.style.display = "block";
  }
}
</script>

)rawliteral";

const char service_button_5_html[] PROGMEM = R"rawliteral(

<form class="service_button_5" id="service">
    normal:
    <br>
    <button onclick="return submit_form('service', 'button'  , 'five') ;">Vol-</button>
    <button onclick="return submit_form('service', 'button'  , 'down') ;">Prev</button>
    <button onclick="return submit_form('service', 'button'  , 'pause');">Pause</button>
    <button onclick="return submit_form('service', 'button'  , 'up')   ;">Next</button>
    <button onclick="return submit_form('service', 'button'  , 'four') ;">Vol+</button>
    <br>
    lang:
    <br>
    <button onclick="return submit_form('service', 'button'  , 'five_long') ;">Vol-</button>
    <button onclick="return submit_form('service', 'button'  , 'down_long') ;">Prev</button>
    <button onclick="return submit_form('service', 'button'  , 'pause_long');">Pause</button>
    <button onclick="return submit_form('service', 'button'  , 'up_long')   ;">Next</button>
    <button onclick="return submit_form('service', 'button'  , 'four_long') ;">Vol+</button>
</form>
<br>

)rawliteral";

const char service_button_3_html[] PROGMEM = R"rawliteral(

<form class="service_button_3" id="service">
    normal:
    <br>
    <button onclick="return submit_form('service', 'button'  , 'down') ;">Prev</button>
    <button onclick="return submit_form('service', 'button'  , 'pause');">Pause</button>
    <button onclick="return submit_form('service', 'button'  , 'up')   ;">Next</button>
    <br>
    lang:
    <br>
    <button onclick="return submit_form('service', 'button'  , 'down_long') ;">Vol-</button>
    <button onclick="return submit_form('service', 'button'  , 'pause_long');">Track</button>
    <button onclick="return submit_form('service', 'button'  , 'up_long')   ;">Vol+</button>
</form>
<br>

)rawliteral";

const char service_button_3_inv_html[] PROGMEM = R"rawliteral(

<form class="service_button_3" id="service">
    normal:
    <br>
    <button onclick="return submit_form('service', 'button'  , 'down') ;">Vol-</button>
    <button onclick="return submit_form('service', 'button'  , 'pause');">Pause</button>
    <button onclick="return submit_form('service', 'button'  , 'up')   ;">Vol+</button>
    <br>
    lang:
    <br>
    <button onclick="return submit_form('service', 'button'  , 'down_long') ;">Prev</button>
    <button onclick="return submit_form('service', 'button'  , 'pause_long');">Track</button>
    <button onclick="return submit_form('service', 'button'  , 'up_long')   ;">Next</button>
</form>
<br>

)rawliteral";

const char main_html[] PROGMEM = R"rawliteral(

<!DOCTYPE html>
<html>
<head>
  <title>TonUINO</title>
  <meta charset=utf-8>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css">
  <link rel="stylesheet" href="style.css">
</head>
<body>
<script>
  function submit_form(address, key, button) {
    console.log(address, key, button);
    var data = new FormData(document.getElementById(address));
    data.append(key, button);
    var xhr = new XMLHttpRequest();
    xhr.open("POST", '/'+address);
    xhr.onload = () => {
      console.log(xhr.response);
    };
    xhr.send(data);
    return false;
  }
</script>

%TOPNAV%

<h2>TonUINO</h2>

<div class="status" id="tonuino_status">%STATE%</div>
<br>

%SERVICE%

<form class="service1" id="card">
<br><label for="mode"              >Karte:              </label><select size=1       name="mode"             id="mode">      
                                                                  <option>Hörspiel</option>                                    
                                                                  <option>Album</option>                                       
                                                                  <option>Party</option>                                       
                                                                  <option>Einzel</option>                                      
                                                                  <option>Hörbuch</option>                                     
                                                                  <option>Hörspiel von bis</option>                            
                                                                  <option>Album von bis</option>                               
                                                                  <option>Party von bis</option>                               
                                                                  <option>Hörbuch einzel</option>                              
                                                                  <option>Wiederhole</option>                                  
                                                                  <option>Quiz Spiel</option>                                  
                                                                  <option>Memory Spiel</option>                                
                                                                </select>                                                      
<br><label for="folder"         >Folder / Sp1 / Sp2     </label><div class="tooltip"><input type="number" name="folder"             id="folder">  
                                                                  <span class="tooltiptext">Folder</span></div>
                                                              / <div class="tooltip"><input type="number" name="special1"           id="special1">
                                                                  <span class="tooltiptext">Einzel: Track
                                                                                        <br>* von bis: erster Track
                                                                                        <br>Hörbuch einzel: Anzahl der Tracks
                                                                                        <br>Quiz: Anzahl der Antworten (0, 2 oder 4)</span></div>
                                                              / <div class="tooltip"><input type="number" name="special2"           id="special2">
                                                                  <span class="tooltiptext">* von bis: letzter Track
                                                                                        <br>Quiz: Anzahl der Lösungen (0 oder 1)</span></div>
<br>
<button onclick="return submit_form('card', 'card_action', 'start');">Start</button>
<button onclick="return submit_form('card', 'card_action', 'write' );">Schreibe</button>
</form>
<br>

<form class="service1" id="modifier">
<br><label for="mod_mode"    >Modifier:  </label><select size=1 name="mod_mode" id="mod_mode">
                                                   <option>Sleep-Timer      </option>
                                                   <option>Stopptanz        </option>
                                                   <option>Feuer-Wasser-Luft</option>
                                                   <option>Gesperrt         </option>
                                                   <option>Kita Modus       </option>
                                                   <option>Wiederhole Track </option>
                                                 </select>
<br><label for="mod_special" >Parameter  </label><div class="tooltip"><input type="number" name="mod_special" id="mod_special">
                                                   <span class="tooltiptext">Sleep-Timer: Timeout in min
                                                                         <br>Feuer-Wasser-Luft/
                                                                         <br>Stopptanz: Pausezeit (min/max)
                                                                         <br>0: 15/30, 1: 25/40, 2: 35/50</span></div>
<br>
<button onclick="return submit_form('modifier', 'mod_action', 'activate');">Activate</button>
<button onclick="return submit_form('modifier', 'mod_action', 'delete'  );">Delete</button>
<button onclick="return submit_form('modifier', 'mod_action', 'write'   );">Schreibe</button>
</form>
<br>

<script>
  var ws;
  function connect() {
    console.log('create new websocket');
    ws = new WebSocket('ws://' + window.location.hostname + '/ws');
    ws.onmessage = function(event) {
      document.getElementById("tonuino_status").innerHTML = event.data;
    };
    ws.onclose = function (e) {
      console.log('websocket closed, reason: ', e.reason);
      /// try to reconnect websocket in 5 seconds
      setTimeout(function () {
        connect();
      }, 5000);
    }
    ws.onerror = function(err) {
      console.error('websocket error: ', err.message);
      ws.close();
    }
  };
  connect();
</script>

</body>
</html>

)rawliteral";

const char settings_html[] PROGMEM = R"rawliteral(

<!DOCTYPE HTML>
<html>
<head>
  <title>TonUINO Einstellungen</title>
  <meta charset=utf-8>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css">
  <link rel="stylesheet" href="style.css">
</head>
<body onload="pageLoad()">

%TOPNAV%

<h2>Einstellungen</h2>

  <br>  <label for="version">Version:                     </label><input type="number" name="version"             id="version" readonly>

  <form action='update_settings' method='post' id='mainConfigForm' accept-charset='utf-8' >
  <div id="form">
    <br><label for="max"             >Lautstärke          </label><div class='as_input_number' id='max' >max </div>
                                                                  <div class='as_input_number' id='min' >min </div>
                                                                  <div class='as_input_number' id='init'>init</div>
    <br><label for="spkMaxVolume"    >Lautsprecher:       </label><div class="tooltip"><input type="number" name="spkMaxVolume"        id="spkMaxVolume">
                                                                    <span class="tooltiptext">Maximale Lautstärke (Lautsprecher) [0...25]</span></div>
                                                                  <div class="tooltip"><input type="number" name="spkMinVolume"        id="spkMinVolume">
                                                                    <span class="tooltiptext">Minimale Lautstärke (Lautsprecher) [0...max]</span></div>
                                                                  <div class="tooltip"><input type="number" name="spkInitVolume"       id="spkInitVolume">
                                                                    <span class="tooltiptext">Initiale Lautstärke (Lautsprecher) [min...max]</span></div>
    <br><label for="hpMaxVolume"     >Kopfhörer:          </label><div class="tooltip"><input type="number" name="hpMaxVolume"         id="hpMaxVolume">
                                                                    <span class="tooltiptext">Maximale Lautstärke (Kopfhörer) [0...25]</span></div>
                                                                  <div class="tooltip"><input type="number" name="hpMinVolume"         id="hpMinVolume">
                                                                    <span class="tooltiptext">Minimale Lautstärke (Kopfhörer) [0...max]</span></div>
                                                                  <div class="tooltip"><input type="number" name="hpInitVolume"        id="hpInitVolume">
                                                                    <span class="tooltiptext">Initiale Lautstärke (Kopfhörer) [min...max]</span></div>
    <br>
    <br><label for="eq"              >Equalizer:          </label><select size=1       name="eq"                  id="eq">
                                                                    <option>Normal</option>
                                                                    <option>Pop</option>
                                                                    <option>Rock</option>
                                                                    <option>Jazz</option>
                                                                    <option>Classic</option>
                                                                    <option>Bass</option>
                                                                  </select>
    <br>
    <br><label for="standbyTimer"    >Standby Timer:      </label><div class="tooltip"><input type="number" name="standbyTimer"        id="standbyTimer">
                                                                    <span class="tooltiptext">Timeout für Ausschalten in min [0...60] (0: deaktiviert)</span></div>
    <br>
    <br><label for="invertVolButtons">Inv. Volume Buttons:</label><input type="checkbox" name="invertVolumeButtons" id="invertVolumeButtons">
    <br>
    <br><label for="adminMenuLocked" >Admin-Menü Lock:    </label><select size=1       name="adminMenuLocked"     id="adminMenuLocked">
                                                                    <option>kein Lock</option>
                                                                    <option>nur Karte</option>
                                                                    <option>Pin</option>
                                                                  </select>
    <br>
    <br><label for="adminMenuPin1"   >Admin-Menü Pin:     </label><div class="tooltip"><input type="number" name="adminMenuPin1"       id="adminMenuPin1">
                                                                    <span class="tooltiptext">1: Pause, 2: Next, 3: Prev</span></div>
                                                                  <div class="tooltip"><input type="number" name="adminMenuPin2"       id="adminMenuPin2">
                                                                    <span class="tooltiptext">1: Pause, 2: Next, 3: Prev</span></div>
                                                                  <div class="tooltip"><input type="number" name="adminMenuPin3"       id="adminMenuPin3">
                                                                    <span class="tooltiptext">1: Pause, 2: Next, 3: Prev</span></div>
                                                                  <div class="tooltip"><input type="number" name="adminMenuPin4"       id="adminMenuPin4">
                                                                    <span class="tooltiptext">1: Pause, 2: Next, 3: Prev</span></div>
    <br>
    <br><label for="pauseWhenCardRemoved">Pause w. Kart. entf.:</label><input type="checkbox" name="pauseWhenCardRemoved" id="pauseWhenCardRemoved">
    <br>

    </div>

    <br>
    <br><input type="submit" value="Save">
  </form>

<script>
  var form_shortcut =
  '<br><label for="sc_mode_n_"      >Shortcut_n_:         </label><select size=1       name="sc_mode_n_"         id="sc_mode_n_">    '
 +'                                                                  <option>--------</option>                                       '
 +'                                                                  <option>Hörspiel</option>                                       '
 +'                                                                  <option>Album</option>                                          '
 +'                                                                  <option>Party</option>                                          '
 +'                                                                  <option>Einzel</option>                                         '
 +'                                                                  <option>Hörbuch</option>                                        '
 +'                                                                  <option>Hörspiel von bis</option>                               '
 +'                                                                  <option>Album von bis</option>                                  '
 +'                                                                  <option>Party von bis</option>                                  '
 +'                                                                  <option>Hörbuch einzel</option>                                 '
 +'                                                                  <option>Wiederhole</option>                                     '
 +'                                                                  <option>Quiz Spiel</option>                                     '
 +'                                                                  <option>Memory Spiel</option>                                   '
 +'                                                                </select>                                                         '
 +'<br><label for="sc_folder_n_"   >Folder / Sp1 / Sp2     </label><div class="tooltip"><input type="number" name="sc_folder_n_"      id="sc_folder_n_">  '  
 +'                                                                 <span class="tooltiptext">Folder</span></div>                                         '
 +'                                                              / <div class="tooltip"><input type="number" name="sc_special1_n_"    id="sc_special1_n_">'
 +'                                                                 <span class="tooltiptext">Einzel: Track                                               '
 +'                                                                                       <br>* von bis: erster Track                                     '
 +'                                                                                       <br>Hörbuch einzel: Anzahl der Tracks                           '
 +'                                                                                       <br>Quiz: Anzahl der Antworten (0, 2 oder 4)</span></div>       '
 +'                                                              / <div class="tooltip"><input type="number" name="sc_special2_n_"    id="sc_special2_n_">'
 +'                                                                 <span class="tooltiptext">* von bis: letzter Track                                    '
 +'                                                                                       <br>Quiz: Anzahl der Lösungen (0 oder 1)</span></div>           '
 +'<br>';

  function pageLoad() {

    for (i = 1; i <= 4; ++i)
      document.getElementById("form").innerHTML += form_shortcut.replaceAll('_n_', i.toString());

    // load json into field
    // Request JSON from sever. comment out block for offline testing
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        // Action to be performed when the document is ready:
        var jsox = xhttp.responseText;
        var json = JSON.parse( jsox );
        document.getElementById( "version"              ).value = json.version;
        document.getElementById( "spkMaxVolume"         ).value = json.spkMaxVolume;
        document.getElementById( "spkMinVolume"         ).value = json.spkMinVolume;
        document.getElementById( "spkInitVolume"        ).value = json.spkInitVolume;
        document.getElementById( "hpMaxVolume"          ).value = json.hpMaxVolume;
        document.getElementById( "hpMinVolume"          ).value = json.hpMinVolume;
        document.getElementById( "hpInitVolume"         ).value = json.hpInitVolume;
        document.getElementById( "eq"                   ).value = json.eq;
        document.getElementById( "standbyTimer"         ).value = json.standbyTimer;
        document.getElementById( "invertVolumeButtons"  ).checked = (json.invertVolumeButtons == "1");
        document.getElementById( "adminMenuLocked"      ).value = json.adminMenuLocked;
        document.getElementById( "adminMenuPin1"        ).value = json.adminMenuPin1;
        document.getElementById( "adminMenuPin2"        ).value = json.adminMenuPin2;
        document.getElementById( "adminMenuPin3"        ).value = json.adminMenuPin3;
        document.getElementById( "adminMenuPin4"        ).value = json.adminMenuPin4;
        document.getElementById( "pauseWhenCardRemoved" ).checked = (json.pauseWhenCardRemoved=="1");

        document.getElementById( "sc_mode1"             ).value = json.sc_mode1;
        document.getElementById( "sc_folder1"           ).value = json.sc_folder1;
        document.getElementById( "sc_special11"         ).value = json.sc_special11;
        document.getElementById( "sc_special21"         ).value = json.sc_special21;

        document.getElementById( "sc_mode2"             ).value = json.sc_mode2;
        document.getElementById( "sc_folder2"           ).value = json.sc_folder2;
        document.getElementById( "sc_special12"         ).value = json.sc_special12;
        document.getElementById( "sc_special22"         ).value = json.sc_special22;

        document.getElementById( "sc_mode3"             ).value = json.sc_mode3;
        document.getElementById( "sc_folder3"           ).value = json.sc_folder3;
        document.getElementById( "sc_special13"         ).value = json.sc_special13;
        document.getElementById( "sc_special23"         ).value = json.sc_special23;

        document.getElementById( "sc_mode4"             ).value = json.sc_mode4;
        document.getElementById( "sc_folder4"           ).value = json.sc_folder4;
        document.getElementById( "sc_special14"         ).value = json.sc_special14;
        document.getElementById( "sc_special24"         ).value = json.sc_special24;
      }
    };
    xhttp.open("GET", "/get_settings", true);  // true => async, false => synchronous
    xhttp.send();
    // end of Request JSON from sever
  }

</script>

</body>
</html>

)rawliteral";

} // namespace

void Webservice::init() {

//  wm.resetSettings();

  WiFi.mode(WIFI_STA);
  wm.setConfigPortalBlocking(false);
  wm.setConfigPortalTimeout(300);
  wm.setConnectTimeout(20);
  wm.setHostname("TonUINO");
  wm.setDebugOutput(true, "WiFi ");

  if (digitalRead(buttonUpPin) == getLevel(buttonPinType, level::active)) {
    bool res = wm.startConfigPortal("TonUINO_IpConfig");
    LOG(webserv_log, s_info, "WiFi startConfigPortal: ", res);
  }
  else if(wm.autoConnect("TonUINO_IpConfig")){
    LOG(webserv_log, s_info, "WiFi connected");
  }
  else {
    LOG(webserv_log, s_info, "WiFi not connected, Configportal running");
  }

  ws.onEvent([this](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) { onWebSocketEvent(server, client, type, arg, data, len); });
  webserver.addHandler(&ws);

  webserver.begin();
  webserver.onNotFound(                       [this](AsyncWebServerRequest *request){ page_notfound  (request); });

  webserver.on("/style.css"      , HTTP_GET,  [this](AsyncWebServerRequest *request){ page_style_css (request); });
  webserver.on("/"               , HTTP_GET,  [this](AsyncWebServerRequest *request){ page_main      (request); });

  webserver.on("/settings"       , HTTP_GET,  [this](AsyncWebServerRequest *request){ page_settings  (request); });
  webserver.on("/get_settings"   , HTTP_GET,  [this](AsyncWebServerRequest *request){ get_settings   (request); });
  webserver.on("/update_settings", HTTP_POST, [this](AsyncWebServerRequest *request){ update_settings(request); });
  webserver.on("/service"        , HTTP_POST, [this](AsyncWebServerRequest *request){ service        (request); });
  webserver.on("/modifier"       , HTTP_POST, [this](AsyncWebServerRequest *request){ modifier       (request); });
  webserver.on("/card"           , HTTP_POST, [this](AsyncWebServerRequest *request){ card           (request); });
}

void Webservice::loop() {
  connected = wm.process();

  ws.cleanupClients();
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

void Webservice::page_main(AsyncWebServerRequest *request)
{
  request->send(200, "text/html", main_html, [this](const String& var) { return process_page(var);});
}

void Webservice::page_style_css(AsyncWebServerRequest *request)
{
  request->send(200, "text/css", style_css);
}

void Webservice::page_settings(AsyncWebServerRequest *request) {
  request->send(200, "text/html", settings_html, [this](const String& var) { return process_page(var);});
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

  LOG(webserv_log, s_info, "jsonStr (", jsonStr, ")");
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
  status += mp3.isHeadphoneJackDetect() ? String("(Kopfhörer)") : String("(Lautsprecher)");
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

  String status = get_status();;

  if (old_status != status) {
    LOG(webserv_log, s_info, "Webservice::push_status");

    LOG(webserv_log, s_debug, "push_status() status: \n", status);

    old_status = status;
    ws.textAll(status);
  }
}

String Webservice::process_page(const String& var) {

  if (var == "STATE")
    return get_status();
  if (var == "TOPNAV")
    return topnav_html;
  if (var == "SERVICE") {
#ifdef FIVEBUTTONS
    return service_button_5_html;
#else
    if (settings.invertVolumeButtons)
      return service_button_3_inv_html;
    else
      return service_button_3_html;
#endif
  }

  return "";

}




#endif
