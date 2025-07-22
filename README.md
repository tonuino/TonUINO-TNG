# TonUINO
Die DIY Musikbox (nicht nur) für Kinder

Dies ist die offizielle Software für die Musikbox, die [hier](https://www.tonuino.de/TNG) beschrieben ist. 

Falls du Interesse daran hast, zur Weiterentwicklung des TonUINO-Projekts beizutragen, bist du herzlich eingeladen, dich zu beteiligen. Für Diskussionen verwende bitte das [Forum](https://discourse.voss.earth). Dort findest du auch weitere Anleitungen und bekommst Hilfe bei Problemen. 

# Anleitung zum Compilieren

## Arduino IDE
Allgemeine Anleitungen zum Einrichten der IDE findet man hier [www.tonuino.de/TNG](https://www.tonuino.de/TNG) und hier [www.leiterkartenpiraten.de](https://www.leiterkartenpiraten.de)

- Es ist unbedingt darauf zu achten, dass das Verzeichnis in das das Repository gecloned oder heruntergeladen wird (also das Verzeichnis, in dem schließlich auch die TonUINO-TNG.ino zu finden ist, genau so heißt, wie die ino Datei, also in diesem Fall "TonUINO-TNG"!

- Bei der classic (Nano, Every, Every4808 und Esp32) sowie AiO HW Variante muss die Datei 'platform.local.txt' in den avr HW Ordner kopiert werden. Dieser Ordner ist gewöhnlich folgender:  

```
    Windows:  
      Classic:  C:\Users\<Benutzer>\AppData\Local\Arduino15\packages\arduino\hardware\avr\1.8.6
      Every:    C:\Users\<Benutzer>\AppData\Local\Arduino15\packages\arduino\hardware\megaavr\1.8.8
      AiO:      C:\Users\<Benutzer>\AppData\Local\Arduino15\packages\LGT8fx Boards\hardware\avr\1.0.7
      Esp32:    C:\Users\<Benutzer>\AppData\Local\Arduino15\packages\arduino\hardware\esp32\2.0.18-arduino.5

    Linux:  
      Classic:  ~/.arduino15/packages/arduino/hardware/avr/1.8.6
      Every:    ~/.arduino15/packages/arduino/hardware/megaavr/1.8.8
      Aio:      ~/.arduino15/packages/LGT8fx Boards/hardware/avr/1.0.7
      Esp32:    ~/.arduino15/packages/arduino/hardware/esp32/2.0.18-arduino.5/

    MacOS 13.x:  
      Classic:  ~/Library/Arduino15/packages/arduino/hardware/avr/1.8.6
      Every:    ~/Library/Arduino15/packages/arduino/hardware/megaavr/1.8.8
      AiO:      ~/Library/Arduino15/packages/LGT8fx Boards/hardware/avr/1.0.7
      Esp32:    ~/Library/Arduino15/packages/arduino/hardware/esp32/2.0.18-arduino.5/
```

- Man findet den Ordner auch, wenn man die Datei platform.txt sucht.  

- Für die AiOplus HW Variante sind keine Änderungen notwendig  

- Die HW Variante (TonUINO_*, ALLinONE oder ALLinONE_Plus) sowie die Button Konfiguration (THREEBUTTONS, FIVEBUTTONS oder BUTTONS3X3) muss in der Datei constants.hpp durch Entfernen des entsprechenden Kommentars angegeben werden. (nur wenn die Arduino IDE verwendet wird) 

**Libraries**
- Es müssen folgende Versionen der Libraries verwendet werden:  
    - jchristensen/JC_Button: 2.1.2  
    - boerge1/MFRC522_fix: 1.4.12  
    - makuna/DFPlayer Mini Mp3 by Makuna: 1.2.3
    - adafruit/Adafruit NeoPixel: 1.11.0 (optional, nur bei Feature NEO_RING notwendig)

- Für die Esp32 HW Variante zusätzlich:
    - plerup/espsoftwareserial: 8.1.0
    - esp32async/AsyncTCP: 3.3.6
    - esp32async/ESPAsyncWebServer: 3.7.2
    - bblanchon/ArduinoJson: 7.3.0

**ESP32 Nano**
Bei der Arduino IDE muss unter "Werkzeuge/Tools" --> "Pin Numbering" unbedingt "By GPIO number (legacy)" eingestellt werden! 

## platform.io

- Es werden die gleichen HW Varianten angeboten wie beim Online Upload ohne die Datei constants.hpp editieren zu müssen

```
  Classic, Every, Every4808, AiO, AiOplus und Esp32
    3 Buttons
    5 Buttons
    3x3 Button Board
    5 Buttons mit vollem Feature-Umfang (nur Every, Esp32 und AiOplus)
```

**Verwendung zusammen mit Visual Code**

Eine Anleitung dafür findet man [hier](https://discourse.voss.earth/t/tonuino-software-mit-platformio-aufspielen/13468)

**Nur platform.io (CLI)**
- platform.io installieren

```
  pip install platformio
```

- Build (wähle [spezielle Variante](platformio.ini) mit dem Flag `-e <environment>`)

```
  pio run
```

- Upload (Der Port kann mit `pio device list` gefunden werden)

```
  pio run -e <environment> -t upload --upload-port <port>
```

- Monitor

```
  pio device monitor -p <port>
```

# Installation

Die SD Karte (Ordner mp3 und advert) hat sich gegenüber der Version 3.1.11 geändert. Hier kann man die Dateien downloaden: [tonuino.github.io/TonUINO-TNG/sd-card.zip](https://tonuino.github.io/TonUINO-TNG/sd-card.zip)

# Hinweise zur WLAN Konfigurattion des ESP32

Wenn man ausschließlich mit dem TonUINO AP mit der SSID "TonUINO" verbunden ist, erreicht man die Webseite mit 
jeder beliebigen Adresse (mit mindestens einem Punkt), also z.B. "http://tonuino.t". 
Wenn man zusätzlich noch mit dem Internet verbunden ist, muss man die IP Adresse nehmen: "http://192.168.4.1". 

# Change Log

## Version 3.3.1 (22.07.2025)
- [Issue 270](https://github.com/tonuino/TonUINO-TNG/issues/270): Add LedManager to control button LEDs with animated states
- [Issue 272](https://github.com/tonuino/TonUINO-TNG/issues/272): Refactor constants.hpp - Avoid unnecessary global definitions
- [Issue 268](https://github.com/tonuino/TonUINO-TNG/issues/268): Compile Error when using Rotary-Encoder and Classic HW

## Version 3.3.0 (09.07.2025)
- [Issue 259](https://github.com/tonuino/TonUINO-TNG/issues/259): New Hardware Type ESP32 Nano

## Version 3.2.1 (25.03.2025)
- [Issue 265](https://github.com/tonuino/TonUINO-TNG/issues/265): Optonal Feature: Jukebox modifier card

## Version 3.2.1 (16.01.2025)
- [Issue 261](https://github.com/tonuino/TonUINO-TNG/issues/261): Increase the number of addidional Tracks for Mode "Hoerbuch einzeln" to 30
- [Issue 263](https://github.com/tonuino/TonUINO-TNG/issues/263): Initialization: increase delay between the tries of setting volume

## Version 3.2.1 (13.01.2025)
- [Issue 135](https://github.com/tonuino/TonUINO-TNG/issues/135): Make new variants available for online uploads II
- [Issue 254](https://github.com/tonuino/TonUINO-TNG/issues/254): Enhance configuration in constants.hpp
- [Issue 245](https://github.com/tonuino/TonUINO-TNG/issues/245): Support NFC 215 Tags

## Version 3.2.1 (23.11.2024)
- [Issue 251](https://github.com/tonuino/TonUINO-TNG/issues/251): Changes to enable configuration of showdown behavior
- [Issue 250](https://github.com/tonuino/TonUINO-TNG/issues/250): Update the non-German texts for advert and mp3 tracks
- [Issue 248](https://github.com/tonuino/TonUINO-TNG/issues/248): Add the possibility to change the antenna gain of the RFRC522 module

## Version 3.2.1 (14.11.2024)
- [Issue 246](https://github.com/tonuino/TonUINO-TNG/issues/246): With LKP Player there is no pling on startup

## Version 3.2.1 (13.10.2024)
- [Issue 238](https://github.com/tonuino/TonUINO-TNG/issues/238): Pololu sometimes does not switch off in sleep mode
- [Issue 240](https://github.com/tonuino/TonUINO-TNG/issues/240): Some code restructuring
- [Issue 239](https://github.com/tonuino/TonUINO-TNG/issues/239): Starting Quiz or Memory game the "pling" is missing
- [Issue 236](https://github.com/tonuino/TonUINO-TNG/issues/236): Improve hardware diagnostic on startup

## Version 3.2.0 (05.09.2024)
- [Issue 231](https://github.com/tonuino/TonUINO-TNG/issues/231): Fix logging of card data (bad order)
- [Issue 229](https://github.com/tonuino/TonUINO-TNG/issues/229): playAdvertisement does not work for some DF Player
- [Issue 228](https://github.com/tonuino/TonUINO-TNG/issues/228): NeoPixel for two Rings with Sleep modification card: no update on one ring
- [Issue 226](https://github.com/tonuino/TonUINO-TNG/issues/226): Use MegaCoreX as Board package for Nano Every instead of Arduino megaAVR Boards
- [Issue 225](https://github.com/tonuino/TonUINO-TNG/issues/225): Support Nano Every with Atmega4808

## Version 3.1.12 (22.08.2024)
- [Issue 222](https://github.com/tonuino/TonUINO-TNG/issues/222): Fix bug: TB modification card ends active game modification card
- [Issue 220](https://github.com/tonuino/TonUINO-TNG/issues/220): New shortcut for switching BT on/off
- [Issue 219](https://github.com/tonuino/TonUINO-TNG/issues/219): Disable modification cards during quiz and memory game

## Version 3.1.11 (31.07.2024)
- [Issue 212](https://github.com/tonuino/TonUINO-TNG/issues/212): Sleep Timer Modification card doesn't work properly with NeoPixel Ring
- [Issue 215](https://github.com/tonuino/TonUINO-TNG/issues/215): New optional Feature: Support BT Modul
- [Issue 217](https://github.com/tonuino/TonUINO-TNG/issues/217): Improve and fix handling of some DF Player

## Version 3.1.10 (27.06.2024)
- [Issue 210](https://github.com/tonuino/TonUINO-TNG/issues/210): Make text in the admin menu for switching volume button more clearer

## Version 3.1.9 (05.06.2024)
- [Issue 207](https://github.com/tonuino/TonUINO-TNG/issues/207): Use poti for setting the volume: add threshold to prevent continuously switching volume
- [Issue 205](https://github.com/tonuino/TonUINO-TNG/issues/205): Implement the game "Feuer, Wasser, Luft" as modification card

## Version 3.1.8 (26.04.2024)
- [Issue 202](https://github.com/tonuino/TonUINO-TNG/issues/202): Enhance modifier card SleepTimer to stop only after the track finished
- [Issue 200](https://github.com/tonuino/TonUINO-TNG/issues/200): Add possibility to use Pololu-Powerswitch for shutdown
- [Issue 196](https://github.com/tonuino/TonUINO-TNG/issues/196): Enhance Hoerbuch_1 mode to play more tracks
- [Issue 197](https://github.com/tonuino/TonUINO-TNG/issues/197): SPECIAL_START_SHORTCUT: pin A6 cannot be read digital on Nano

## Version 3.1.7 (11.04.2024)
- [Issue 193](https://github.com/tonuino/TonUINO-TNG/issues/193): Hoerbuch mode: after playing last track it does not change to first track on next start
- [Issue 190](https://github.com/tonuino/TonUINO-TNG/issues/190): Neo Pixel Ring: Add the possibility to have 2 rings remains
- [Issue 188](https://github.com/tonuino/TonUINO-TNG/issues/188): Pause when card removed modus: do not go to Play via button if card is not present

## Version 3.1.7 (29.03.2024)
- [Issue 184](https://github.com/tonuino/TonUINO-TNG/issues/184): #define DONT_ACCEPT_SAME_RFID_TWICE makes the error: 'class Tonuino' has no member named 'getCard'
- [Issue 181](https://github.com/tonuino/TonUINO-TNG/issues/181): Implement battery voltage measurement
- [Issue 180](https://github.com/tonuino/TonUINO-TNG/issues/180): Play special shortcut on startup if a GPIO is set
- [Issue 056](https://github.com/tonuino/TonUINO-TNG/issues/56): Implement headphone jack detection
- [Issue 178](https://github.com/tonuino/TonUINO-TNG/issues/178): Use Nano Every optional with HW Serial connection to the DfPlayer

## Version 3.1.7 (25.03.2024)
- [Issue 182](https://github.com/tonuino/TonUINO-TNG/issues/182): Quiz game: do not repeat a question until no question remains
- [Issue 176](https://github.com/tonuino/TonUINO-TNG/issues/176): Implement the memory game

## Version 3.1.6 (18.02.2024)
- [Issue 173](https://github.com/tonuino/TonUINO-TNG/issues/173): Improve Rorary Encoder implementation (also for NANO and optional for next/previous)
- [Issue 135](https://github.com/tonuino/TonUINO-TNG/issues/135): Make new variants available for online upload (3 butonn variants)
- [Issue 167](https://github.com/tonuino/TonUINO-TNG/issues/167): Save the last played card in EEPROM and restore it at startup
- [Issue 155](https://github.com/tonuino/TonUINO-TNG/issues/155): Implement a Quiz Game

## Version 3.1.5 (30.01.2024)
- [Issue 166](https://github.com/tonuino/TonUINO-TNG/issues/166): Issue_166: generateRamdomSeed() does not generate a random value
- [Issue 165](https://github.com/tonuino/TonUINO-TNG/issues/165): 'Play last card' does not work as ShortCut
- [Issue 162](https://github.com/tonuino/TonUINO-TNG/issues/162): Prepare optional feature ROTARY_ENCODER for Nano Every
- [Issue 160](https://github.com/tonuino/TonUINO-TNG/issues/160): Improve the description for platform.io in the Readme
- [Issue 153](https://github.com/tonuino/TonUINO-TNG/issues/153): Some improvements of the DF Player handling
- [Issue 149](https://github.com/tonuino/TonUINO-TNG/issues/149): Add possibility to reset the current track on hoerbuch mode
- [Issue 148](https://github.com/tonuino/TonUINO-TNG/issues/148): New handling of prev and next button on first and last track
- [Issue 147](https://github.com/tonuino/TonUINO-TNG/issues/147): No or bad saving of current track in hoerbuch mode when using prev, prev10 or next10 button
- [Issue 143](https://github.com/tonuino/TonUINO-TNG/issues/143): With some players the start of a track stutters or goes into pause
- [Issue 142](https://github.com/tonuino/TonUINO-TNG/issues/142): Restart last playback if Play/Pause pressed
- [Issue 141](https://github.com/tonuino/TonUINO-TNG/issues/141): Enhance Features for Neo Pixel Ring
- [Issue 132](https://github.com/tonuino/TonUINO-TNG/issues/132): Support DF Player MP3-TF-16P V3.0 with the chip MH2024K-24SS

## Version 3.1.4 (20.11.2023)
- [Issue 138](https://github.com/tonuino/TonUINO-TNG/issues/138): Two new options for when the same RFID card is inserted
- [Issue 130](https://github.com/tonuino/TonUINO-TNG/issues/130): Add circuit diagram
- [Issue 133](https://github.com/tonuino/TonUINO-TNG/issues/133): Cards with version 1 don't work
- [Issue 125](https://github.com/tonuino/TonUINO-TNG/issues/125): platform.local.txt on MacOS
- [Issue 126](https://github.com/tonuino/TonUINO-TNG/issues/126): Support Speaker on/off for Classic Variant to suppress Noise on startup and shutdown 
- [Issue 123](https://github.com/tonuino/TonUINO-TNG/issues/123): Setting of pauseWhenCardRemoved not disabled when upgraded from Version 2.x 
- [Issue 117](https://github.com/tonuino/TonUINO-TNG/issues/117): Support potentiometer for setting the 
- [Issue 120](https://github.com/tonuino/TonUINO-TNG/issues/120): Change to version 1.2.2 of the DFMiniMp3 library
- [Issue 118](https://github.com/tonuino/TonUINO-TNG/issues/118): In modus pause_if_card_removed no shortcut is played
- [Issue 115](https://github.com/tonuino/TonUINO-TNG/issues/115): Sometimes initial SetVolume does not come to an end
- [Issue 103](https://github.com/tonuino/TonUINO-TNG/issues/103): Deploy pages for online upload
- [Issue 111](https://github.com/tonuino/TonUINO-TNG/issues/111): TonUINO crashes if if the player gives a track count >255
- [Issue 108](https://github.com/tonuino/TonUINO-TNG/issues/108): Missing OnPlayFinished: the progress is not saved for Hoerbuch mode
- [Issue 106](https://github.com/tonuino/TonUINO-TNG/issues/106): Support LISP3 DF Player
- [Issue 100](https://github.com/tonuino/TonUINO-TNG/issues/100): Support Nano Every with classic HW
- [Issue 104](https://github.com/tonuino/TonUINO-TNG/issues/104): Use bad framework-lgt8fx
- [Issue 099](https://github.com/tonuino/TonUINO-TNG/issues/99): Use new DFPlayer Mini Mp3 v1.2.1 Library

## Version 3.1.3 (03.08.2023)
- [Issue 073](https://github.com/tonuino/TonUINO-TNG/issues/73): Support LED or NeoPixel Ring
- [Issue 095](https://github.com/tonuino/TonUINO-TNG/issues/95): React on 3x3 Button Board also in Play State
- [Issue 088](https://github.com/tonuino/TonUINO-TNG/issues/88): Flash reset during startup should not open the admin menu
- [Issue 039](https://github.com/tonuino/TonUINO-TNG/issues/39): No pause if card is removed too early
- [Issue 091](https://github.com/tonuino/TonUINO-TNG/issues/91): Revise configuration part of the file constants.hpp
- [Issue 093](https://github.com/tonuino/TonUINO-TNG/issues/93): location for platfrom.txt on macOS does not exist
- [Issue 079](https://github.com/tonuino/TonUINO-TNG/issues/79): Support DF Player GD3200B
- [Issue 085](https://github.com/tonuino/TonUINO-TNG/issues/85): Bad audio message on writing card if card is already present
- [Issue 069](https://github.com/tonuino/TonUINO-TNG/issues/69): Enhance serial input as command source to jump into menu entries
- [Issue 075](https://github.com/tonuino/TonUINO-TNG/issues/75): Optimize Memory (RAM and FLASH) usage
- [Issue 082](https://github.com/tonuino/TonUINO-TNG/issues/82): Speed up VolumeUp/Down when using longpress
- [Issue 070](https://github.com/tonuino/TonUINO-TNG/issues/70): Revise modification cards
- [Issue 076](https://github.com/tonuino/TonUINO-TNG/issues/76): \<\<Phopp\>\> sound at poweroff
- [Issue 072](https://github.com/tonuino/TonUINO-TNG/issues/72): Support Rotary Encoder KY-040 for setting the volume
- Viele Fehlerkorrekturen und Verbesserungen
- [Issue 019](https://github.com/tonuino/TonUINO-TNG/issues/19): Implement support for the 3x3 Button board

## Version 3.1.2 (03.03.2023)
- [Issue 062](https://github.com/tonuino/TonUINO-TNG/issues/62): Add offline TTS coqui to text_to_speach
- [Issue 067](https://github.com/tonuino/TonUINO-TNG/issues/67): Add a hint in the admin menu that the card has to be removed
- [Issue 065](https://github.com/tonuino/TonUINO-TNG/issues/65): Increase dfPlayer_timeUntilStarts
- [Issue 061](https://github.com/tonuino/TonUINO-TNG/issues/61): Fix pin assignment for classic with 5 buttons
- [Issue 054](https://github.com/tonuino/TonUINO-TNG/issues/54): For classic variant: the shutdownPin should be HIGH on shutdown for the POLOLU switch
- [Issue 050](https://github.com/tonuino/TonUINO-TNG/issues/50): Cards with bad version handled as modification cards
- [Issue 038](https://github.com/tonuino/TonUINO-TNG/issues/38): Bad initialization of setting 'pause when card removed'
- [Issue 039](https://github.com/tonuino/TonUINO-TNG/issues/39): No pause if card is removed too early
- [Issue 028](https://github.com/tonuino/TonUINO-TNG/issues/28): Revise Button behavior

## Version 3.1.1 (15.01.2023)
- [Issue 045](https://github.com/tonuino/TonUINO-TNG/issues/45): audio_messages_de.txt isn't up to date.
- [Issue 044](https://github.com/tonuino/TonUINO-TNG/issues/44): Statemachine remains in StartPlay forever is mp3 files missing
- [Issue 034](https://github.com/tonuino/TonUINO-TNG/issues/34): Unit Test Framework and example Tests
- [Issue 026](https://github.com/tonuino/TonUINO-TNG/issues/26): DFMiniMp3 lib support T_CHIP_VARIANT
- Remove strange noise during startup
- [Issue 015](https://github.com/tonuino/TonUINO-TNG/issues/15): Implement command sources
- [Issue 016](https://github.com/tonuino/TonUINO-TNG/issues/16): Shortcut at startup doesn't work
- Bug Fix: Cannot enter Admin Menue with buttons
- [Issue 003](https://github.com/tonuino/TonUINO-TNG/issues/3): Shortcut does not work after power-up
- Bug Fix: Hoerbuch mode: Bad handling of start track
- [Issue 004](https://github.com/tonuino/TonUINO-TNG/issues/4): Implement reaction to empty card
- [Issue 005](https://github.com/tonuino/TonUINO-TNG/issues/5): Update to use new mp3 library version 1.1.0

## Version 3.1 (13.10.2022)
- Unterstützung für alle Platinen der Leiterkartenpiraten hinzugefügt (über einfaches #define in der `src/constants.hpp` konfigurierbar)
  - TonUINO Classic
  - All-in-One
  - All-in-One Plus
  - fix for NTAG213

## Version 3.0 (xx.xx.xxxx) - by Boerge1
- vollständiges Refactoring mit State-Machine
- Die Main-Loop läuft jetzt stabil mit 50 ms
- Neues Feature: neuer Mode: Hörbuch einzeln (nur ein Titel wird gespielt und Fortschritt gespeichert)
- Neues Feature: Pause, wenn Karte entfernt wird (lässt sich per Einstellungen steuern)
- Das Admin-Menü wird nach einer Einstellung nicht verlassen (kann in der Software leicht geändert werden)
- Das Admin-Menü kann an jeder Stelle abgebrochen werden
- Viele weitere Verbesserungen und Bug-Fixes

## Version 2.1 (xx.xx.xxxx) noch WIP
- Partymodus hat nun eine Queue -> jedes Lied kommt nur genau 1x vorkommt
- Neue Wiedergabe-Modi "Spezialmodus Von-Bis" - Hörspiel, Album und Party -> erlaubt z.B. verschiedene Alben in einem Ordner zu haben und je mit einer Karte zu verknüpfen
- Admin-Menü
- Maximale, Minimale und Initiale Lautstärke
- Karten werden nun über das Admin-Menü neu konfiguriert
- die Funktion der Lautstärketasten (lauter/leiser oder vor/zurück) kann im Adminmenü vertauscht werden
- Shortcuts können konfiguriert werden!
- Support für 5 Knöpfe hinzugefügt
- Reset der Einstellungen ins Adminmenü verschoben
- Modikationskarten (Sleeptimer, Tastensperre, Stopptanz, KiTa-Modus)
- Admin-Menü kann abgesichert werden

## Version 2.01 (01.11.2018)
- kleiner Fix um die Probleme beim Anlernen von Karten zu reduzieren

## Version 2.0 (26.08.2018)

- Lautstärke wird nun über einen langen Tastendruck geändert
- bei kurzem Tastendruck wird der nächste / vorherige Track abgespielt (je nach Wiedergabemodus nicht verfügbar)
- Während der Wiedergabe wird bei langem Tastendruck auf Play/Pause die Nummer des aktuellen Tracks angesagt
- Neuer Wiedergabemodus: **Einzelmodus**
  Eine Karte kann mit einer einzelnen Datei aus einem Ordner verknüpft werden. Dadurch sind theoretisch 25000 verschiedene Karten für je eine Datei möglich
- Neuer Wiedergabemodus: **Hörbuch-Modus**
  Funktioniert genau wie der Album-Modus. Zusätzlich wir der Fortschritt im EEPROM des Arduinos gespeichert und beim nächsten mal wird bei der jeweils letzten Datei neu gestartet. Leider kann nur der Track, nicht die Stelle im Track gespeichert werden
- Um mehr als 100 Karten zu unterstützen wird die Konfiguration der Karten nicht mehr im EEPROM gespeichert sondern direkt auf den Karten - die Karte muss daher beim Anlernen aufgelegt bleiben!
- Durch einen langen Druck auf Play/Pause kann **eine Karte neu konfiguriert** werden
- In den Auswahldialogen kann durch langen Druck auf die Lautstärketasten jeweils um 10 Ordner oder Dateien vor und zurück gesprungen werden
- Reset des MP3 Moduls beim Start entfernt - war nicht nötig und hat "Krach" gemacht

# Open Source 

- jchristensen/JC_Button - GPL 2007
- boerge1/MFRC522_fix - free PD
- makuna/DFPlayer Mini Mp3 by Makuna - LGPL 2007
- adafruit/Adafruit NeoPixel - LGPL 2007
- plerup/espsoftwareserial - LGPL 1999
- esp32async/AsyncTCP - LGPL 2007 
- esp32async/ESPAsyncWebServer - LGPL 2007
- bblanchon/ArduinoJson - MIT
- digint.ch/tinyfsm - free PD (code geändert)
- circuitcode/AsyncWebSerial - MIT (code geändert)
- tuniii/LogRingBuffer - GPL V3+ (code geändert)



