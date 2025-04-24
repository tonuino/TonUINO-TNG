#include "constants.hpp"
#ifdef TonUINO_Esp32

#include <Arduino.h>

#include "webserial_pages.hpp"
#include "webservice_style.hpp"

const char webserial_style[] PROGMEM = R"rawliteral(

div{
  display:block
}
html{
  height:100%
}
body{
  overscroll-behavior:none;
  height:100%;
  margin:0;
  padding:0;
  font-family: Arial, Helvetica, sans-serif;
  background-color: #99ccff;
}
.app{
  height:100%;
  display:flex;
  flex-direction:column
}
.content{
  flex:1 1 0%;
  align-content:flex-end;
  height:100%;
  color:#ddd;
  background-color: #000;
  font-family:monospace;
  overflow-x:hidden;
  overflow-y:auto;
  font-size:14px
}
.content pre{
  margin:0;
  text-wrap:wrap;
}
.buttons{
  display:flex;
  justify-content:flex-end;
  color:#c1c1c1
}
.buttons button{
  cursor:pointer;
  padding:8px 10px 8px;
  font-size:medium;
  outline-style:none;
  border:0;
  color:#000;
  background-color:transparent
}
.buttons button svg{
  width:1.4rem;
  height:1.4rem
}
.connection-status{
  display:flex;
  justify-content:flex-end;
  border-bottom-width:1px;
  padding:.5rem 1rem;
  font-size:.75rem;
  line-height:1rem;
  color:rgb(113 113 122);
  align-items:center;
  border-color:rgb(24 24 27);
  gap:.5rem
}
.badge{
  width:.375rem;
  height:.375rem;
  border-radius:9999px
}
.badge.green{
  background-color:#22c55e
}
.badge.orange{
  background-color:#f59e0b
}
.badge.red{
  background-color:#ef4444
}

)rawliteral";

const char webserial_page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html><head>
  <title>TonUINO Log</title>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width,initial-scale=1">
  <link rel="stylesheet" href="webserial_style.css">
</head>

<body>

  <div class="app">
    %TOPNAV%

    <h2>TonUINO Log</h2>

    <div class="content"></div>
    <footer>
      <div class="connection-status"><div class="badge red"></div> Disconnected</div>
    </footer>
  </div>
  
  <script>
    document.getElementById("nav_hostname").innerHTML = '%HOSTNAME%';
    let websocket,
        enableScroll=!0,
        maxLines = 2000,
        url=`ws://${window.location.hostname}:${window.location.port}/ws_serial`,
        contentArea=document.querySelector(".content"),
        connectionStatus=document.querySelector(".connection-status");
    var f_right1_html = 
     '  <svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">'
    +'    <path stroke="none" d="M0 0h24v24H0z" fill="none"></path>'
    +'    <path d="M5 13a2 2 0 0 1 2 -2h10a2 2 0 0 1 2 2v6a2 2 0 0 1 -2 2h-10a2 2 0 0 1 -2 -2v-6z"></path>'
    +'    <path d="M11 16a1 1 0 1 0 2 0a1 1 0 0 0 -2 0"></path>'
    +'    <path d="M8 11v-4a4 4 0 1 1 8 0v4"></path>'
    +'  </svg>';
    document.getElementById("f_right1").innerHTML = f_right1_html;
    document.getElementById("f_right1").style.display = "block";
    function f_right1() {
      enableScroll=!enableScroll;
      col = "transparent";
      if (!enableScroll)
        col = "#666";
      else
        terminalWrite("");
      document.getElementById("f_right1").style.backgroundColor=col;
    }
    function initApp(){
        connectionStatus.innerHTML="";
        initWebSocket();
    }
    function initWebSocket(){
        connectionStatus.innerHTML='<div class="badge orange"></div> Connecting...';
        websocket=new WebSocket(url);
        websocket.onopen=onOpen;
        websocket.onclose=onClose;
        websocket.onmessage=onMessage;
    }
    function onOpen(e){
        connectionStatus.innerHTML='<div class="badge green"></div> Connected';
        terminalWrite("Connected to "+url + "<br>");
    }
    function onClose(e){
        connectionStatus.innerHTML='<div class="badge red"></div> Disconnected';
        setTimeout(initWebSocket,2e3);
    }
    function onMessage(e){
        terminalWrite(e.data);
    }
    function terminalWrite(e){
        contentArea.innerHTML+=e;
        enableScroll&&(contentArea.scrollTop=contentArea.scrollHeight);
        let t=contentArea.querySelectorAll("pre");
        if(t.length>maxLines)
          for(let i=0;i<t.length-n;i++)
            contentArea.removeChild(t[i]);
    }
    initApp();
  </script>
</body>
</html>
)rawliteral";

#endif
