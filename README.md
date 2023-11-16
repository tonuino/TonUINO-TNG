# TonUINO
Die DIY Musikbox (nicht nur) für Kinder

Dies ist die offizielle Software für die Musikbox, die [hier](https://www.tonuino.de/TNG) beschrieben ist. 

Falls du Interesse daran hast, zur Weiterentwicklung des TonUINO-Projekts beizutragen, bist du herzlich eingeladen, dich zu beteiligen. Für Diskussionen verwende bitte das [Forum](https://discourse.voss.earth). Dort findest du auch weitere Anleitungen und bekommst Hilfe bei Problemen. 

# Anleitung zum Compilieren

Allgemeine Anleitungen zum Einrichten der IDE findet man hier [www.tonuino.de/TNG](https://www.tonuino.de/TNG) und hier [www.leiterkartenpiraten.de](https://www.leiterkartenpiraten.de)

## Arduino IDE
- Es ist unbedingt darauf zu achten, das das Verzeichnis in das das Repository gecloned oder heruntergeladen wird (also das Verzeichnis, in dem schließlich auch die TonUINO-TNG.ino zu finden ist, genau so heißt, wie die ino Datei, also in diesem Fall "TonUINO-TNG"!

- Bei der classic und AiO HW Variante muss die Datei 'platform.local.txt' in den avr HW Ordner kopiert werden. Dieser Ordner ist gewöhnlich folgender:  

```
    Windows:  
      Classic:  C:\Users\<Benutzer>\AppData\Local\Arduino15\packages\arduino\hardware\avr\1.8.6
      AiO:      C:\Users\<Benutzer>\AppData\Local\Arduino15\packages\LGT8fx Boards\hardware\avr\1.0.7

    Linux:  
      Classic:  ~/.arduino15/packages/arduino/hardware/avr/1.8.6
      Aio:      ~/.arduino15/packages/LGT8fx Boards/hardware/avr/1.0.7

    MacOS 13.x:  
      Classic:  ~/Library/Arduino15/packages/arduino/hardware/avr/1.8.6
      AiO:      ~/Library/Arduino15/packages/LGT8fx Boards/hardware/avr/1.0.7
```

- Man findet den Ordner auch, wenn man die Datei platform.txt sucht.  

- Für die AiOplus HW Variante sind keine Änderungen notwendig  

- Die HW Variante (TonUINO_Classic, ALLinONE oder ALLinONE_Plus) sowie die Button Konfiguration (THREEBUTTONS, FIVEBUTTONS oder BUTTONS3X3) muss in der Datei constants.hpp durch Entfernen des entsprechenden Kommentars angegeben werden. 

## platform.io
- geht out-of-the-box

## Libraries
- Es müssen folgende Versionen der Libraries verwendet werden:  
    - jchristensen/JC_Button: 2.1.2  
    - miguelbalboa/MFRC522: 1.4.10  
    - makuna/DFPlayer Mini Mp3 by Makuna: 1.2.2
    - adafruit/Adafruit NeoPixel: 1.11.0 (optional, nur bei Feature NEO_RING notwendig)


# Installation

Die SD Karte (Ordner mp3 und advert) hat sich gegenüber der Version 3.1.2 geändert. Hier kann man die Dateien downloaden: [tonuino.github.io/TonUINO-TNG/sd-card.zip](https://tonuino.github.io/TonUINO-TNG/sd-card.zip)

# Change Log

## Version 3.1.4 (16.11.2023)
- [Issue 133](https://github.com/tonuino/TonUINO-TNG/issues/133): Cards with version 1 don't work


## Version 3.1.4 (29.10.2023)
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
