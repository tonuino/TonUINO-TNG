
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
    <a href="/system">System</a>
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

const char system_html[] PROGMEM = R"rawliteral(

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

%TOPNAV%

<h2>TonUINO System</h2>

<form class="system" action='/wifi'    method='get'><button>Configure WiFi</button></form><br/>
<form class="system" action='/info'    method='get'><button>Info</button></form><br/>

</body>
</html>

)rawliteral";

const char wifi_html[] PROGMEM = R"rawliteral(

<!DOCTYPE html>
<html>
<head>
  <title>TonUINO</title>
  <meta charset=utf-8>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css">
  <link rel="stylesheet" href="style.css">
</head>
<body onload="refresh()">

%TOPNAV%

<h2>TonUINO WiFi Konfiguration</h2>

<div class='wifi'>
  <button onclick='refresh()'>Refresh</button>
  <br><br>
</div>
<form class='wifi' method='POST' action='wifisave'>
  
  <div id='networks'></div>
  <br><br>
  <label for='s'>SSID</label><input id='s' name='ssid' type='text' maxlength='32' autocorrect='off' autocapitalize='none' placeholder='%SSID%'>
  <br><br>
  <label for='p'>Password</label><input id='p' name='password' type='password' maxlength='64' placeholder='%PASSWORD%'/>
  <br>
  <label for='showpass'>Show Password</label><input type='checkbox' id='showpass' onclick='toggle_pass()'/>

  <br><br>
  <button type='submit'>Save</button>
</form>

<script>
  function select_network(l){
    document.getElementById('s').value = l.getAttribute('data-ssid')||l.innerText||l.textContent;
    p = l.nextElementSibling.classList.contains('l');
    document.getElementById('p').disabled = !p;
    if(p)
      document.getElementById('p').focus();
  };

  function toggle_pass() {
    var x = document.getElementById('p');
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
        console.log(networks);
        document.getElementById("networks").innerHTML = "";
        for (var i in networks) {
          var network = networks[i];
          network_str = "<div><a href='#p' onclick='select_network(this)' data-ssid='" + network.ssid + "'>" + network.ssid + "</a>"
                      + "<div role='img' aria-label='" + network.rssi + "%' title='" + network.rssi + "%' class='q q-" + network.q + " " + network.auth + "'></div>"
                      + "</div>";
          console.log(network_str);
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
  <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css">
  <link rel="stylesheet" href="style.css">
</head>
<body>

%TOPNAV%

<h2>TonUINO Info</h2>

<h3>esp32</h3><hr><dl>%esphead%<br><br>
<dt>Uptime</dt><dd>%uptime%</dd>
<dt>Chip ID</dt><dd>%chipid%</dd>
<dt>Chip rev</dt><dd>%chiprev%</dd>
<dt>Flash size</dt><dd>%idesize% bytes</dd> 
<dt>PSRAM Size</dt><dd>%flashsize% bytes</dd>      
<dt>CPU frequency</dt><dd>%cpufreq%MHz</dd>
<dt>Memory - Free heap</dt><dd>%freeheap% bytes available</dd>
<dt>Memory - Sketch size</dt><dd>Used / Total bytes<br/>%memsketch1% / %memsketch2%
<br/><progress value='%memsmeter1%' max='%memsmeter2%'></progress></dd>      
<dt>Temperature</dt><dd>%temp1% C&deg;</dd>
<br/><h3>WiFi</h3><hr>%wifihead%<br><br>
<dt>Connected</dt><dd>%conx%</dd>
<dt>Station SSID</dt><dd>%stassid%</dd>
<dt>Station IP</dt><dd>%staip%</dd>
<dt>Station gateway</dt><dd>%stagw%</dd>
<dt>Station subnet</dt><dd>%stasub%</dd>
<dt>DNS Server</dt><dd>%dnss%</dd>
<dt>Hostname</dt><dd>%host%</dd>
<dt>Station MAC</dt><dd>%stamac%</dd>
<dt>Access point SSID</dt><dd>%apssid%</dd>
<br/><h3>WiFi AP</h3><hr>
<dt>Access point IP</dt><dd>%apip%</dd>
<dt>Access point MAC</dt><dd>%apmac%</dd>
<dt>Access point hostname</dt><dd>%aphost%</dd>
<dt>BSSID</dt><dd>%apbssid%</dd>

</dl>

<h3>About</h3><hr><dl>
<dt>TonUINO TNG Version</dt><dd>%aboutver%</dd>
<dt>Arduino</dt><dd>%aboutarduinover%</dd>
<dt>Build date</dt><dd>%aboutdate%</dd>
</dl>

</body>
</html>

)rawliteral";

