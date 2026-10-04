// ##############################################################################################################################
//                                            Konfiguration des TonUINO
// ##############################################################################################################################

// -------------------------------------------------------------------------------------------------------------------------------
//            Auswahl des verwendeten Ptozessor-Boards
// -------------------------------------------------------------------------------------------------------------------------------
//#define TonUINO_Classic                       // Basisversion mit Arduino Nano V3
//#define TonUINO_Every                         // Arduino Nano Every (Original)
//#define TonUINO_Every_4808                    // Arduino Nano Evers (Klon)
//#define ALLinONE                              // AllInOne Platine
//#define ALLinONE_Plus                         // AllInOne Plus Platine
//#define TonUINO_Esp32 100 // Esp32 Nano       // Arduino Nano ESP32
//#define TonUINO_Esp32 200 // Esp32 Wroom 32   // ESP32 Wroom

// -------------------------------------------------------------------------------------------------------------------------------
//            Anpassung des Bedienkonzeptes
// -------------------------------------------------------------------------------------------------------------------------------
//#define THREEBUTTONS                          // 3 Tasten mit Doppelfunktion
//#define FIVEBUTTONS                           // 5 Tasten
//#define FIVEBUTTONS_SWITCHED                  // Zurück- / Vor-Tasten werden mit Leiser / Lauter Tasten getauscht
//#define BUTTONS3X3                            // 3 Tasten mit Doppelfungtion + 9 Tasten für Shortcuts

// -------------------------------------------------------------------------------------------------------------------------------
//            Befehlseingabe über serielle Schnittstelle
// -------------------------------------------------------------------------------------------------------------------------------
//#define SerialInputAsCommand                  // Befehlseingabe möglich über Konsole und TonUINO Manager

// -------------------------------------------------------------------------------------------------------------------------------
//            Verwendung der internen serielle Schnittstelle zum DF.Player
// -------------------------------------------------------------------------------------------------------------------------------
//#define DFPlayerUsesHardwareSerial            // nicht möglich für Arduino Nano V3

// -------------------------------------------------------------------------------------------------------------------------------
//            Auswahl / Anpassung des Decoder Cips vom DF.Player
// -------------------------------------------------------------------------------------------------------------------------------
//#define DFMiniMp3_T_CHIP_GD3200B
//#define DFMiniMp3_T_CHIP_MH2024K16SS
//#define DFMiniMp3_T_CHIP_LISP3
//#define DFMiniMp3_T_CHIP_MH2024K24SS_MP3_TF_16P_V3_0
//#define DFMiniMp3_T_CHIP_Mp3ChipIncongruousNoAck
//#define DFMiniMp3_T_CHIP_Original

// -------------------------------------------------------------------------------------------------------------------------------
//            Verhinderung Abschalten / Energiesparmodus über Pause-Taste
// -------------------------------------------------------------------------------------------------------------------------------
//#define DISABLE_SHUTDOWN_VIA_BUTTON


// -------------------------------------------------------------------------------------------------------------------------------
//            Anpassung des Abschalt-Signals (HW erforderlich, elektronischer Schalter)
// -------------------------------------------------------------------------------------------------------------------------------
//#define USE_POLOLU_SHUTDOWN           // HIGH --> Shutdown
//#define USE_TRAEGER_PLATINE_SHUTDOWN  // LOW  --> Shutdown

// -------------------------------------------------------------------------------------------------------------------------------
//            Aktivierung der Steuerung über Drehencoder - Kombination mit Potenziometer nicht möglich (HW erforderlich)
// -------------------------------------------------------------------------------------------------------------------------------
//#define ROTARY_ENCODER                // einfache Steuerung der Lautstärke, Vor- / Zurück-Tasen erforderlich
//#define ROTARY_ENCODER_LONGPRESS      // Steuerung mit Doppelbelegung des Drehencoders, keine zusätzlichen Tasten notwendig

// -------------------------------------------------------------------------------------------------------------------------------
//            Lautstärkeregelung über Potenziometer -  Kombination mit Drehencoder nicht möglich (HW erforderlich)
// -------------------------------------------------------------------------------------------------------------------------------
//#define POTI                          // zusätzliche Tasten erforderlich, Kombination mit Drehencoder nicht möglich

// -------------------------------------------------------------------------------------------------------------------------------
//            Ansteuerung von Neopixel Elementen - Anzahl der Elemente in constants.hpp angeben
// -------------------------------------------------------------------------------------------------------------------------------
//#define NEO_RING                      // Anzeige von Betriebszuständen / Wiedergabe
//#define NEO_RING_EXT                  // zusätzliche Anzeige bei Änderung der Lautstärke
//#define NEO_RING_2                    // gegenläufige Ansteuerung einer 2-ten Neopixel Gruppe

// -------------------------------------------------------------------------------------------------------------------------------
//            Stummschaltung Lautsprecher während Einschalten (HW erforderlich)
// -------------------------------------------------------------------------------------------------------------------------------
//#define SPKONOFF

// -------------------------------------------------------------------------------------------------------------------------------
//            Erkennung Kopfhörer --> Stummschaltung des Lautsprechers, getrennte Lautstärkeregelung möglich (HW erforderlich)
// -------------------------------------------------------------------------------------------------------------------------------
//#define HPJACKDETECT

// -------------------------------------------------------------------------------------------------------------------------------
//            Modifikation von Funktionen
// -------------------------------------------------------------------------------------------------------------------------------
//#define DONT_ACCEPT_SAME_RFID_TWICE       // Wiedergabe beginnt nicht von vorn, wenn dieselbe Karte erneut aufgelegt wird
//#define RESUME_ON_SAME_RFID               // nur in Verbindung mit "DONT_ACCEPT_SAME_RFID_TWICE", beendet bei Wiederauflegen möglichen Pause-Status
//#define REPLAY_ON_PLAY_BUTTON             // wenn aktuell nichts abgespielt wird, startet die Pause-Taste den zuletzt gehörten Track erneut

// -------------------------------------------------------------------------------------------------------------------------------
//            Aktivierung integrierter Spiele (besondere Datenstruktur in Ordner der SD-Karte erforderlich)
// -------------------------------------------------------------------------------------------------------------------------------
//#define QUIZ_GAME         // Quiz-Spiel mit Antwortvorschlägen oder Buzzer
//#define MEMORY_GAME       // Memory-Spiel, akustische Paare erforderlich
//#define TEAPOT_GAME       // Teekesselchenspiel mit 5 Hinweistexten

// -------------------------------------------------------------------------------------------------------------------------------
//            Speicherung des zuletzt gehörten Tracks, kann nach Neustart erneut abgespielt, ohne Karte aufzulegen
// -------------------------------------------------------------------------------------------------------------------------------
//#define STORE_LAST_CARD

// -------------------------------------------------------------------------------------------------------------------------------
//            Soundtrack als Weckersignal wählbar (HW erforderlich, externer Wecker mit Signalausgang)
// -------------------------------------------------------------------------------------------------------------------------------
//#define SPECIAL_START_SHORTCUT

// -------------------------------------------------------------------------------------------------------------------------------
//            Erkennung Kopfhörer --> Stummschaltung des Lautsprechers, getrennte Lautstärkeregelung möglich (HW erforderlich)
// -------------------------------------------------------------------------------------------------------------------------------
//#define BT_MODULE

// -------------------------------------------------------------------------------------------------------------------------------
//            Überwachung der Batteriespannung, Warnton bei Schwellwert (HW erforderlich, Spannungsteiler)
// -------------------------------------------------------------------------------------------------------------------------------
//#define BAT_VOLTAGE_MEASUREMENT

//#define LFP     //  Schwellwert für LiFePO4 Akku einstellen
//#define LiPo    //  Schwellwert für LiPo Akku einstellen

// -------------------------------------------------------------------------------------------------------------------------------
//            Einstellung der Empfindlichkeit des Kartenlesers
// -------------------------------------------------------------------------------------------------------------------------------
//#define MRFC522_RX_GAIN RxGain_18dB
//#define MRFC522_RX_GAIN RxGain_23dB
//#define MRFC522_RX_GAIN RxGain_33dB // default
//#define MRFC522_RX_GAIN RxGain_38dB
//#define MRFC522_RX_GAIN RxGain_43dB
//#define MRFC522_RX_GAIN RxGain_48dB
//#define MRFC522_RX_GAIN RxGain_min  // 18dB
//#define MRFC522_RX_GAIN RxGain_avg  // 33dB
//#define MRFC522_RX_GAIN RxGain_max  // 48dB

// -------------------------------------------------------------------------------------------------------------------------------
//            Zusatzfunktionen des TonUINO
// -------------------------------------------------------------------------------------------------------------------------------
//#define MODIFICATION_CARD_JUKEBOX                // Simulation einer Jukebox, Einzelkarten werden in eine Playliste eingetragen
//#define MODIFICATION_CARD_PAUSE_AFTER_TRACK      // per Modifikationskarte kann Pause-Status nach jedem Track erzwungen werden

// -------------------------------------------------------------------------------------------------------------------------------
//            Animation mit 3 LEDs zur Signalisierung des Betriebszustandes (nicht kombinierbar mit Neopixel)
// -------------------------------------------------------------------------------------------------------------------------------
//#define USE_LED_BUTTONS

// -------------------------------------------------------------------------------------------------------------------------------
//            Signalisierung, wenn das Power-Hold Signal verfügbar ist
// -------------------------------------------------------------------------------------------------------------------------------
//#define POWER_HOLD_READY

// -------------------------------------------------------------------------------------------------------------------------------
//            Mehrere Hörbücher in einem gemeinsamen Verzeichnis, Fortschrittszähler nur für aktuelles Hörbuch
// -------------------------------------------------------------------------------------------------------------------------------
//#define FOLDER_QUEUE_HOERBUCH
