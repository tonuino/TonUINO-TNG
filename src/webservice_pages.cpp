#include "constants.hpp"
#ifdef TonUINO_Esp32

#include <Arduino.h>

#include "webservice_pages.hpp"

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

<style>

.topnav {
  overflow: hidden;
  background-color: #333;
  position: relative;
}

.topnav #nav_links {
  display: none;
}

.topnav a {
  height: 18px;
  font-size: 17px;
  color: white;
  padding: 14px 16px;
  text-decoration: none;
  display: block;
}

.topnav a.icon {
  background: black;
  display: block;
  position: absolute;
  left: 0;
  top: 0;
}

.topnav a.icon_right {
  background: transparent;
  display: none;
  position: absolute;
  right: 0;
  top: 0;
}

.topnav a:hover {
  text-decoration: none;
}

.bar1, .bar2, .bar3 {
  width: 25px;
  height: 3px;
  background-color: #fff;
  margin: 3px 0;
  transition: 0.4s;
}

.change .bar1 {
  transform: translate(0, 6px) rotate(-45deg);
}

.change .bar2 {opacity: 0;}

.change .bar3 {
  transform: translate(0, -6px) rotate(45deg);
}

.active {
  background-color: #ff9900;
  color: white;
  text-align: center;
}

</style>
<!-- Top Navigation Menu -->
<div class="topnav">
  <a id='nav_hostname' href="/" class="active">TonUINO</a>
  <div id="nav_links">
    <a href="/">Home</a>
    <a href="/settings">Einstellungen</a>
    <a href="/system">System</a>
    <a href="/webserial">Log</a>
  </div>
  <a class="icon" onclick="show_hide_nav(this)">
    <div class="bar1"></div>
    <div class="bar2"></div>
    <div class="bar3"></div>
  </a>
  <a id="f_right1" class="icon_right" onclick="f_right1()">
  </a> 
</div>
<script>
function show_hide_nav(i) {
  i.classList.toggle("change");
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
    <button onclick="return submit_form(this, 'service', 'button'  , 'five') ;">Vol-</button>
    <button onclick="return submit_form(this, 'service', 'button'  , 'down') ;">Prev</button>
    <button onclick="return submit_form(this, 'service', 'button'  , 'pause');">Pause</button>
    <button onclick="return submit_form(this, 'service', 'button'  , 'up')   ;">Next</button>
    <button onclick="return submit_form(this, 'service', 'button'  , 'four') ;">Vol+</button>
    <br>
    lang:
    <br>
    <button onclick="return submit_form(this, 'service', 'button'  , 'five_long') ;">Vol-</button>
    <button onclick="return submit_form(this, 'service', 'button'  , 'down_long') ;">Prev</button>
    <button onclick="return submit_form(this, 'service', 'button'  , 'pause_long');">Pause</button>
    <button onclick="return submit_form(this, 'service', 'button'  , 'up_long')   ;">Next</button>
    <button onclick="return submit_form(this, 'service', 'button'  , 'four_long') ;">Vol+</button>
</form>
<br>

)rawliteral";

const char service_button_3_html[] PROGMEM = R"rawliteral(

<form class="service_button_3" id="service">
    normal:
    <br>
    <button onclick="return submit_form(this, 'service', 'button'  , 'down') ;">Prev</button>
    <button onclick="return submit_form(this, 'service', 'button'  , 'pause');">Pause</button>
    <button onclick="return submit_form(this, 'service', 'button'  , 'up')   ;">Next</button>
    <br>
    lang:
    <br>
    <button onclick="return submit_form(this, 'service', 'button'  , 'down_long') ;">Vol-</button>
    <button onclick="return submit_form(this, 'service', 'button'  , 'pause_long');">Track</button>
    <button onclick="return submit_form(this, 'service', 'button'  , 'up_long')   ;">Vol+</button>
</form>
<br>

)rawliteral";

const char service_button_3_inv_html[] PROGMEM = R"rawliteral(

<form class="service_button_3" id="service">
    normal:
    <br>
    <button onclick="return submit_form(this, 'service', 'button'  , 'down') ;">Vol-</button>
    <button onclick="return submit_form(this, 'service', 'button'  , 'pause');">Pause</button>
    <button onclick="return submit_form(this, 'service', 'button'  , 'up')   ;">Vol+</button>
    <br>
    lang:
    <br>
    <button onclick="return submit_form(this, 'service', 'button'  , 'down_long') ;">Prev</button>
    <button onclick="return submit_form(this, 'service', 'button'  , 'pause_long');">Track</button>
    <button onclick="return submit_form(this, 'service', 'button'  , 'up_long')   ;">Next</button>
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
  <link rel="stylesheet" href="style.css">
</head>
<body>
<script>
  function submit_form(btn, address, key, button, no_check=false) {
    console.log(address, key, button);
    var form = document.getElementById(address);
    if (!no_check && !form.checkValidity()) {
      console.log("not valid");
      form.reportValidity();
      return false;
    }
    document.body.classList.add("waiting");
    var btn_text = btn.innerHTML;
    btn.innerHTML = "Wait";
    btn.disabled=true;
    var data = new FormData(form);
    data.append(key, button);
    var xhr = new XMLHttpRequest();
    xhr.open("POST", '/'+address);
    var timer = setTimeout(() => {
      btn.innerHTML = btn_text;
      btn.disabled=false;
      alert("keine Antwort vom TonUINO");
    }, 8000)
    xhr.onload = () => {
      console.log(xhr);
      clearTimeout(timer);
      document.body.classList.remove("waiting");
      btn.innerHTML = btn_text;
      btn.disabled=false;
      if (xhr.status != 200)
        alert(xhr.response);
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
                                                                  <option>Bluetooth ein/aus</option>                                
                                                                </select>                                                      
<br><label for="folder"         >Folder / Sp1 / Sp2     </label><div class="tooltip"><input type="number" name="folder"             id="folder" value="1" min="1" max="99">  
                                                                  <span class="tooltiptext">Folder</span></div>
                                                              / <div class="tooltip"><input type="number" name="special1"           id="special1" value="0" min="0" max="255">
                                                                  <span class="tooltiptext">Einzel: Track
                                                                                        <br>* von bis: erster Track
                                                                                        <br>Hörbuch einzel: Anzahl der Tracks - 1
                                                                                        <br>Quiz: Anzahl der Antworten (0, 2 oder 4)</span></div>
                                                              / <div class="tooltip"><input type="number" name="special2"           id="special2" value="0" min="0" max="255">
                                                                  <span class="tooltiptext">* von bis: letzter Track
                                                                                        <br>Quiz: Anzahl der Lösungen (0 oder 1)</span></div>
<br>
<button onclick="return submit_form(this, 'card', 'card_action', 'start');">Start</button>
<button onclick="return submit_form(this, 'card', 'card_action', 'write' );">Schreibe</button>
</form>
<br>

<form class="service1" id="modifier">
<br><label for="mod_mode"    >Modifier:  </label><select size=1 name="mod_mode" id="mod_mode" onchange="mod_mode_changed(this)">
                                                   <option>Sleep-Timer           </option>
                                                   <option>Stopptanz             </option>
                                                   <option>Feuer-Wasser-Luft     </option>
                                                   <option>Gesperrt              </option>
                                                   <option>Kita Modus            </option>
                                                   <option>Wiederhole Track      </option>
                                                   <option>Jukebox               </option>
                                                   <option>Pause nach jedem Track</option>
                                                   <option>Standby Timer Off     </option>
                                                 </select>
<br><label for="mod_special" >Parameter  </label><div class="tooltip"><input type="number" name="mod_special" id="mod_special" value="1" min="-127" max="127">
                                                   <span class="tooltiptext">Sleep-Timer: Timeout in min [1..127] (neg.: Tr. wird beendet)
                                                                         <br>Feuer-Wasser-Luft/
                                                                         <br>Stopptanz: Zeit zw. Pausen (min/max)
                                                                         <br>0: 15/30, 1: 25/40, 2: 35/50</span></div>
<br>
<button onclick="return submit_form(this, 'modifier', 'mod_action', 'activate'    );">Activate/Deact.</button>
<button onclick="return submit_form(this, 'modifier', 'mod_action', 'write'       );">Schreibe</button>
</form>
<br>

<script>
  function mod_mode_changed(el) {
    var mod_special = document.getElementById("mod_special");
    if (el.value == "Sleep-Timer") {
      mod_special.min = -127;
      mod_special.max =  127;
    }
    else if (el.value == "Feuer-Wasser-Luft" || el.value == "Stopptanz") {
      mod_special.min = 0;
      mod_special.max = 2;
    }
    else {
      mod_special.min = 0;
      mod_special.max = 0;
      mod_special.value = 0;
    }
  }
  document.getElementById("nav_hostname").innerHTML = '%HOSTNAME%';
  var f_right1_html = 
   '<svg fill="#FFF" width="24px" height="24px" viewBox="0 0 512 512" xmlns="http://www.w3.org/2000/svg" transform="matrix(-1, 0, 0, 1, 0, 0)" stroke="#000000"><g id="SVGRepo_bgCarrier" stroke-width="0"></g><g id="SVGRepo_tracerCarrier" stroke-linecap="round" stroke-linejoin="round"></g><g id="SVGRepo_iconCarrier"><path d="M400 54.1c63 45 104 118.6 104 201.9 0 136.8-110.8 247.7-247.5 248C120 504.3 8.2 393 8 256.4 7.9 173.1 48.9 99.3 111.8 54.2c11.7-8.3 28-4.8 35 7.7L162.6 90c5.9 10.5 3.1 23.8-6.6 31-41.5 30.8-68 79.6-68 134.9-.1 92.3 74.5 168.1 168 168.1 91.6 0 168.6-74.2 168-169.1-.3-51.8-24.7-101.8-68.1-134-9.7-7.2-12.4-20.5-6.5-30.9l15.8-28.1c7-12.4 23.2-16.1 34.8-7.8zM296 264V24c0-13.3-10.7-24-24-24h-32c-13.3 0-24 10.7-24 24v240c0 13.3 10.7 24 24 24h32c13.3 0 24-10.7 24-24z"></path></g></svg>';
  document.getElementById("f_right1").innerHTML = f_right1_html;
  document.getElementById("f_right1").style.display = "block";
  var ws;
  function f_right1() {
    return submit_form(this, 'service', 'command', 'shutdown');
  }
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
  document.getElementById("nav_hostname").innerHTML = '%HOSTNAME%';
  var form_shortcut =
  '<br><label for="sc_mode_n_"      >Shortcut_t_:         </label><select size=1       name="sc_mode_n_"         id="sc_mode_n_">    '
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
 +'                                                                  <option>Bluetooth ein/aus</option>                                   '
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

    document.getElementById("form").innerHTML += form_shortcut.replaceAll('_n_', "1").replaceAll('_t_', " Next+Prev");
    document.getElementById("form").innerHTML += form_shortcut.replaceAll('_n_', "2").replaceAll('_t_', " Next");
    document.getElementById("form").innerHTML += form_shortcut.replaceAll('_n_', "3").replaceAll('_t_', " Prev");
    document.getElementById("form").innerHTML += form_shortcut.replaceAll('_n_', "4").replaceAll('_t_', " Start");

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

const char system_html[] PROGMEM = R"rawliteral(

<!DOCTYPE html>
<html>
<head>
  <title>TonUINO</title>
  <meta charset=utf-8>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="style.css">
</head>
<body>

%TOPNAV%

<h2>TonUINO System</h2>

<form class="system" action='/wifi'    method='get'><button>Configure WiFi</button></form><br>
<form class="system" action='/info'    method='get'><button>Info</button></form><br>
<form class="system" action='/upgrade' method='get'><button>Firmware Update OTA</button></form><br>

</body>
</html>

<script>
  document.getElementById("nav_hostname").innerHTML = '%HOSTNAME%';
</script>
)rawliteral";

const char wifi_html[] PROGMEM = R"rawliteral(

<!DOCTYPE html>
<html>
<head>
  <title>TonUINO</title>
  <meta charset=utf-8>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="style.css">
</head>
<body onload="refresh()">

%TOPNAV%

<h2>TonUINO WiFi Konfiguration</h2>

<div class='wifi'>
  <button onclick='refresh()'>Refresh</button>
  <br><br>
</div>
<form id='wifisave' class='wifi'>
  
  <div id='networks'></div>
  <br><br>
  <label for='ssid'>SSID</label><input id='ssid' name='ssid' type='text' maxlength='32' autocorrect='off' autocapitalize='none' value='%SSID%'>
  <br><br>
  <label for='password'>Password</label><input id='password' name='password' type='password' maxlength='64' value='%PASSWORD%'/>
  <br>
  <label for='showpass'>Show Password</label><input type='checkbox' id='showpass' onclick='toggle_pass()'/>
  <br><br>
  <label for='hostname'>Hostname</label><input id='hostname' name='hostname' type='text' maxlength='32' autocorrect='off' autocapitalize='none' value='%HOSTNAME%'>
  <br><br>
  <label for='static_ip'>Static IP</label><input type='checkbox' id='static_ip' name='static_ip' %CHECKED_STIP%/>
  <br><br>
  <label for='static_ip_addr'>Static IP address</label><input id='static_ip_addr' name='static_ip_addr' type='text' minlength="7" maxlength="15" size="15" pattern="^((25[0-5]|(2[0-4]|1\d|[1-9]|)\d)(\.(?!$)|$)){4}$" value='%STIPADDRESS%'>
  <br><br>
  <label for='static_ip_gw'>Gateway</label><input id='static_ip_gw' name='static_ip_gw' type='text' minlength="7" maxlength="15" size="15" pattern="^((25[0-5]|(2[0-4]|1\d|[1-9]|)\d)(\.(?!$)|$)){4}$" value='%GW%'>
  <br><br>
  <label for='static_ip_subnet'>Subnet</label><input id='static_ip_subnet' name='static_ip_subnet' type='text' minlength="7" maxlength="15" size="15" pattern="^((25[0-5]|(2[0-4]|1\d|[1-9]|)\d)(\.(?!$)|$)){4}$" value='%SUBNET%'>
  <br><br>
  <label for='static_ip_dns1'>Primary DNS</label><input id='static_ip_dns1' name='static_ip_dns1' type='text' minlength="7" maxlength="15" size="15" pattern="^((25[0-5]|(2[0-4]|1\d|[1-9]|)\d)(\.(?!$)|$)){4}$" value='%DNS1%'>
  <br><br>
  <label for='static_ip_dns2'>Secondary DNS</label><input id='static_ip_dns2' name='static_ip_dns2' type='text' minlength="7" maxlength="15" size="15" pattern="^((25[0-5]|(2[0-4]|1\d|[1-9]|)\d)(\.(?!$)|$)){4}$" value='%DNS2%'>

  <br><br>
  <label for='reboot'>Reboot on Save</label><input type='checkbox' id='reboot' name='reboot'/>

  <br><br>
  <button onclick='return save()'>Save</button>
</form>
<dialog id="dialog">
    <p>Die WLAN Daten wurden gespeichert. Falls Reboot nicht ausgewählt, 
       starte den TonUINO neu. Verbinde dich mit dem jetzt gespeicherten WLAN 
       und drücke dann Ok. Du wirst nach 5 Sekunden auf die TonUINO 
       Seite umgeleitet</p>
    <p><button id="ok">Ok</button></p>
</dialog>

<script>
  document.getElementById("nav_hostname").innerHTML = '%HOSTNAME%';
  var hostname = "";
  document.addEventListener('DOMContentLoaded', function () {
    document.getElementById("ok")
      .addEventListener("click", () => {
        document.getElementById("dialog").close();
        setTimeout(function () {
          document.location="http://"+hostname;
        }, 5000);
    });
  });
  function save() {
    var form_wifisave = document.getElementById("wifisave");
    if (!form_wifisave.checkValidity()) {
      console.log("not valid");
      form_wifisave.reportValidity();
      return false;
    }
    hostname =  document.getElementById("hostname").value;
    submit_form("wifisave", "wifi", "save");
    document.getElementById("dialog").showModal();
    return false;
  }
  function submit_form(address, key, button) {
    console.log(address, key, button);
    var data = new FormData(document.getElementById(address));
    data.append(key, button);
    var xhr = new XMLHttpRequest();
    xhr.open("POST", '/'+address);
    xhr.onload = () => {
      console.log(xhr);
    };
    xhr.send(data);
    return false;
  }

  function select_network(l){
    document.getElementById('ssid').value = l.getAttribute('data-ssid')||l.innerText||l.textContent;
    p = l.nextElementSibling.classList.contains('l');
    document.getElementById('password').disabled = !p;
    if(p)
      document.getElementById('password').focus();
  };

  function toggle_pass() {
    var x = document.getElementById('password');
    x.type==='password'?x.type='text':x.type='password';
  }

  function refresh() {

    document.getElementById("networks").innerHTML = "Scanning...";
    // Request JSON from sever. comment out block for offline testing
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        // Action to be performed when the document is ready:
        var jsox = xhttp.responseText;
        var json = JSON.parse( jsox );
        var networks = json.networks;
        document.getElementById("networks").innerHTML = "";
        for (var i in networks) {
          var network = networks[i];
          network_str = "<div><a href='#p' onclick='select_network(this)' data-ssid='" + network.ssid + "'>" + network.ssid + "</a>"
                      + "<div role='img' aria-label='" + network.rssi + "%' title='" + network.rssi + "%' class='q q-" + network.q + " " + network.auth + "'></div>"
                      + "</div>";
          document.getElementById("networks").innerHTML += network_str;
        }
      }
    };
    xhttp.open("GET", "/scan_networks", true);  // true => async, false => synchronous
    xhttp.send();
    // end of Request JSON from sever
  }

</script>

</body>
</html>

)rawliteral";

const char info_html[] PROGMEM = R"rawliteral(

<!DOCTYPE html>
<html>
<head>
  <title>TonUINO</title>
  <meta charset=utf-8>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="style.css">
</head>
<body>

%TOPNAV%

<h2>TonUINO Info</h2>

<h3>About</h3><hr><dl>
<dt>TonUINO TNG Version</dt><dd>%aboutver%</dd>
<dt>TonUINO TNG Konfiguration</dt><dd>%aboutconfig%</dd>
<dt>Arduino</dt><dd>%aboutarduinover%</dd>
<dt>Build date</dt><dd>%aboutdate%</dd>
</dl>

<h3>esp32</h3><hr><dl>%esphead%<br><br>
<dt>Uptime</dt><dd>%uptime%</dd>
<dt>Chip ID</dt><dd>%chipid%</dd>
<dt>Chip rev</dt><dd>%chiprev%</dd>
<dt>CPU frequency</dt><dd>%cpufreq%MHz</dd>
<dt>Flash size</dt><dd>%idesize% bytes</dd> 
<dt>Memory - PSRAM</dt><dd>MinFree / Used / Total bytes<br>%minfreepsram% / %usedpsram% / %psram%      
<br><progress value='%usedpsram%' max='%psram%'></progress></dd>      
<dt>Memory - Heap</dt><dd>MinFree / Used / Total bytes<br>%minfreeheap% / %usedheap% / %heap%
<br><progress value='%usedheap%' max='%heap%'></progress></dd>      
<dt>Memory - Sketch</dt><dd>Used / Total bytes<br>%memsketch1% / %memsketch2%
<br><progress value='%memsketch1%' max='%memsketch2%'></progress></dd>      
<dt>Temperature</dt><dd>%temp1% C&deg;</dd>
<br><h3>WiFi</h3><hr>
<dt>Mode</dt><dd>%wifihead%</dd>
<br><h3>WiFi STA</h3><hr>
<dt>Connected</dt><dd>%conx%</dd>
<dt>SSID</dt><dd>%stassid%</dd>
<dt>BSSID</dt><dd>%apbssid%</dd>
<dt>Station IP</dt><dd>%staip%</dd>
<dt>Station gateway</dt><dd>%stagw%</dd>
<dt>Station subnet</dt><dd>%stasub%</dd>
<dt>DNS Server</dt><dd>%dnss%</dd>
<dt>Hostname</dt><dd>%host%</dd>
<dt>Station MAC</dt><dd>%stamac%</dd>
<br><h3>WiFi AP</h3><hr>
<dt>Access point IP</dt><dd>%apip%</dd>
<dt>Access point MAC</dt><dd>%apmac%</dd>
<dt>Access point hostname</dt><dd>%aphost%</dd>
</dl>

<script>
  document.getElementById("nav_hostname").innerHTML = '%HOSTNAME%';
</script>
</body>
</html>

)rawliteral";

const char upgrade_html[] PROGMEM = R"rawliteral(

<!DOCTYPE html>
<html><head>
<meta http-equiv="content-type" content="text/html; charset=UTF-8">
  <title>TonUINO</title>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="style.css">
</head>
<body>

%TOPNAV%

<h2>TonUINO OTA FW Upgrade</h2>

Installed firmware: %aboutver%<br>
Build date: %aboutdate%

  <p class='red' id="status"></p>
  <input type="file" name="file1" id="file1" accept=".bin"><br><br>
  <button onclick="uploadFile()">Upgrade</button><br><br>
  <progress id="progressBar" value="0" max="100" style="width:300px;"></progress>
  <p id="loaded_n_total"></p>

<script>
  document.getElementById("nav_hostname").innerHTML = '%HOSTNAME%';
  document.getElementById("status").innerHTML = "";
  document.getElementById("loaded_n_total").innerHTML = "";
function _(el) {
  return document.getElementById(el);
}
function uploadFile() {
  var file = _("file1").files[0];
  var formdata = new FormData();
  formdata.append("file1", file);
  var ajax = new XMLHttpRequest();
  ajax.upload.addEventListener("progress", progressHandler, false);
  ajax.addEventListener("load", completeHandler, false);
  ajax.addEventListener("error", errorHandler, false);
  ajax.addEventListener("abort", abortHandler, false);
  ajax.open("POST", "/upgrade");
  ajax.send(formdata);
}
function progressHandler(event) {
  _("loaded_n_total").innerHTML = "Uploaded " + event.loaded + " bytes";
  var percent = (event.loaded / event.total) * 100;
  _("progressBar").value = Math.round(percent);
  _("status").innerHTML = Math.round(percent) + "% uploaded... please wait";
  if (percent >= 100) {
    _("status").innerHTML = "Please wait, finishing upgrade";
  }
}
function refresh() {
  window.location.reload(true);
}
function completeHandler(event) {
  _("status").innerHTML = "Upload Complete, Rebooting...";
  _("progressBar").value = 0;
  setTimeout(refresh, 5000);
}
function errorHandler(event) {
  _("status").innerHTML = "Upload Failed";
}
function abortHandler(event) {
  _("status").innerHTML = "Upload Aborted";
}
</script>

</body></html>

)rawliteral";

#endif
