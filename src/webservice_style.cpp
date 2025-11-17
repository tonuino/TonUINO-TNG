#include "constants.hpp"
#ifdef TonUINO_Esp32

#include <Arduino.h>

#include "webservice_style.hpp"

const char style_css[] PROGMEM = R"rawliteral(

@media (min-width: 768px) {
    body {
        font-family: Arial, Helvetica, sans-serif;
        background-color: #99ccff;
        font-size: 16px;
        text-align:left;
        max-width:30em
   }
}
@media (max-width: 767px) {
    body {
        font-family: Arial, Helvetica, sans-serif;
        background-color: #99ccff;
        font-size: 16px;
        text-align:left;
        max-width:30em
    }
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

input[type=text], input[type=password]{
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
  width: 16ch;
  height: 5ch;
}

.system button {
  width: 100%;
  height: 5ch;
}

.wifi button {
  margin-top: 0.5em;
  width: 10ch;
  height: 4ch;
}

body.waiting,
body.waiting a:hover,
body.waiting *
{
    cursor: wait !important;
}

button + button {
  margin-left: 1ch;
}

.status {
  line-height:1.1em;
  min-height:4.5em;
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

a{
  color:#000;
  font-weight:700;
  text-decoration:none
}
a:hover{
  color:#1fa3ec;
  text-decoration:underline
}
.q{
  height:14px;
  margin:0;
  padding:0 5px;
  text-align:right;
  min-width:38px;
  float:right
}
.q.q-0:after{
  background-position-x:0
}
.q.q-1:after{
  background-position-x:-16px
}
.q.q-2:after{
  background-position-x:-32px
}
.q.q-3:after{
  background-position-x:-48px
}
.q.q-4:after{
  background-position-x:-64px
}
.q.l:before{
  background-position-x:-80px;
  padding-right:5px
}
.ql .q{
  float:left
}
.q:after,.q:before{
  content:'';
  width:16px;
  height:16px;
  display:inline-block;
  background-repeat:no-repeat;
  background-position: 16px 0;
  background-image:url('data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAGAAAAAQCAMAAADeZIrLAAAAJFBMVEX///8AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAADHJj5lAAAAC3RSTlMAIjN3iJmqu8zd7vF8pzcAAABsSURBVHja7Y1BCsAwCASNSVo3/v+/BUEiXnIoXkoX5jAQMxTHzK9cVSnvDxwD8bFx8PhZ9q8FmghXBhqA1faxk92PsxvRc2CCCFdhQCbRkLoAQ3q/wWUBqG35ZxtVzW4Ed6LngPyBU2CobdIDQ5oPWI5nCUwAAAAASUVORK5CYII=');
}
dt{
  font-weight:bold
}
dd{
  padding:0 0 1em 0;
  min-height:12px
}

.red {
  color: red;
}

dialog[open] {
  position: fixed;
  width: 20em;
  background: #fffbf0;
  border: thin solid #ff1a1a;
  margin: 5em auto;
}


)rawliteral";

#endif
