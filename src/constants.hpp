#ifndef SRC_CONSTANTS_HPP_
#define SRC_CONSTANTS_HPP_

#include <Arduino.h>

// Select the right PCB by uncommenting one of the following lines
// Bitte die passende Platine durch entfernen der Kommentare in einer der folgenden Zeilen auswählen
//#define TonUINO_Classic
//#define ALLinONE
//#define ALLinONE_Plus

// uncomment the below line to enable five button support (already enabled for AiO and AiO+)
// um dedizierte Lauter-/Leiserknöpfe zu haben bitte die nächste Zeile auskommentieren (Standard bei AiO und AiO+)
//#define FIVEBUTTONS

// uncomment the below line to enable serial input as additional command source
// um den Serial Monitor als zusätzliche Kommandoquelle zu haben bitte in der nächste Zeile den Kommentar entfernen
//#define SerialInputAsCommand
// 7:            8: up         9: upLong
// 4: allLong    5: pause      6: pauseLong
// 1:            2: down       3: downLong

// uncomment the below line if you have support for the 3x3Buttons
// um die Unterstützung für die 3x3 Buttons zu haben bitte in der nächste Zeile den Kommentar entfernen
//#define BUTTONS3X3

// uncomment the below line to support the MH2024K16SS chip on the DfMiniMp3 player
// um den Chip MH2024K16SS auf dem DfMiniMp3 Player zu ünterstützen bitte in der nächste Zeile den Kommentar entfernen
//#define DFMiniMp3_T_CHIP_VARIANT Mp3ChipMH2024K16SS

// uncomment the below line to disable shutdown via nutton (long press play/pause)
// um ein Shutdown via Taste (long press Play/Pause) zu unterdrücken bitte in der nächste Zeile den Kommentar entfernen
//#define DISABLE_SHUTDOWN_VIA_BUTTON

// ####### helper for level ############################

enum class level : uint8_t {
  inactive,
  active  ,
};
enum class levelType : uint8_t {
  activeHigh, // normally open
  activeLow , // normally closed
};

inline constexpr int getLevel(levelType t, level l) { return (l == level::inactive) ? (t == levelType::activeHigh ? LOW : HIGH)
                                                                                    : (t == levelType::activeHigh ? HIGH : LOW); }
#ifdef BUTTONS3X3
#ifdef FIVEBUTTONS
static_assert(false, "The 3x3 Button board doesn't have 5 Buttons");
#endif
inline constexpr uint8_t buttonExtSC_begin   = 101;
inline constexpr uint8_t buttonExtSC_buttons =  18;
#endif // BUTTONS3X3


#ifdef TonUINO_Classic
// ####### buttons #####################################

inline constexpr uint32_t  buttonLongPress = 1000; // timeout for long press button in ms
inline constexpr uint8_t   buttonPausePin  = A0;

#if defined(BUTTONS3X3)
inline constexpr uint8_t   button3x3Pin    = A3;
inline constexpr uint8_t   buttonUpPin     = A1;
inline constexpr uint8_t   buttonDownPin   = A2;
inline constexpr uint32_t  button3x3DbTime = 50; // Debounce time in milliseconds (default 50ms)
#elif defined(FIVEBUTTONS)
inline constexpr uint8_t   buttonUpPin     = A3;
inline constexpr uint8_t   buttonDownPin   = A4;
inline constexpr uint8_t   buttonFourPin   = A1;
inline constexpr uint8_t   buttonFivePin   = A2;
#else
inline constexpr uint8_t   buttonUpPin     = A1;
inline constexpr uint8_t   buttonDownPin   = A2;
#endif

inline constexpr levelType buttonPinType   = levelType::activeLow;
inline constexpr uint32_t  buttonDbTime    = 25; // Debounce time in milliseconds (default 25ms)

// ####### chip_card ###################################

inline constexpr uint32_t cardCookie      = 0x1337b347;
inline constexpr uint8_t  cardVersion     = 0x02;
inline constexpr byte     mfrc522_RSTPin  =  9;
inline constexpr byte     mfrc522_SSPin   = 10;
inline constexpr uint8_t  cardRemoveDelay =  3;

// ####### mp3 #########################################

#define DFPlayerUsesSoftwareSerial
#define CHECK_MISSING_ONPLAYFINISHED

inline constexpr uint8_t       maxTracksInFolder        = 255;
inline constexpr uint8_t       dfPlayer_receivePin      = 2;
inline constexpr uint8_t       dfPlayer_transmitPin     = 3;
inline constexpr uint8_t       dfPlayer_busyPin         = 4;
inline constexpr levelType     dfPlayer_busyPinType     = levelType::activeHigh;
inline constexpr unsigned long dfPlayer_timeUntilStarts = 1000;

// ####### tonuino #####################################

inline constexpr uint8_t       shutdownPin     = 7;
inline constexpr levelType     shutdownPinType = levelType::activeHigh;
inline constexpr uint8_t       openAnalogPin   = A7;
inline constexpr unsigned long cycleTime       = 50;
#endif /* TonUINO_Classic */

#ifdef ALLinONE_Plus
// ####### buttons #####################################

inline constexpr uint32_t  buttonLongPress = 1000; // timeout for long press button in ms
inline constexpr uint8_t   buttonPausePin  = A0;

#ifdef BUTTONS3X3
inline constexpr uint8_t   button3x3Pin    = A2;
inline constexpr uint8_t   buttonUpPin     = A4;
inline constexpr uint8_t   buttonDownPin   = A3;
inline constexpr uint32_t  button3x3DbTime = 50; // Debounce time in milliseconds (default 50ms)
#else
#define FIVEBUTTONS
inline constexpr uint8_t   buttonUpPin     = A2;
inline constexpr uint8_t   buttonDownPin   = A1;
#endif

#ifdef FIVEBUTTONS
inline constexpr uint8_t   buttonFourPin   = A4;
inline constexpr uint8_t   buttonFivePin   = A3;
#endif

inline constexpr levelType buttonPinType   = levelType::activeLow;
inline constexpr uint32_t  buttonDbTime    = 25; // Debounce time in milliseconds (default 25ms)

// ####### chip_card ###################################

inline constexpr uint32_t cardCookie      = 0x1337b347;
inline constexpr uint8_t  cardVersion     = 0x02;
inline constexpr byte     mfrc522_RSTPin  = 11;
inline constexpr byte     mfrc522_SSPin   =  7;
inline constexpr uint8_t  cardRemoveDelay =  3;

// ####### mp3 #########################################

#define DFPlayerUsesHardwareSerial
#define CHECK_MISSING_ONPLAYFINISHED

inline constexpr uint8_t        maxTracksInFolder        = 255;
// inline constexpr uint8_t        dfPlayer_receivePin      =  2;
// inline constexpr uint8_t        dfPlayer_transmitPin     =  3;
inline constexpr uint8_t        dfPlayer_busyPin         = 13;
inline constexpr levelType      dfPlayer_busyPinType     = levelType::activeHigh;
inline constexpr unsigned long  dfPlayer_timeUntilStarts = 1000;
inline constexpr HardwareSerial &dfPlayer_serial         = Serial3;
// ####### tonuino #####################################

inline constexpr uint8_t       shutdownPin      = 27;
inline constexpr levelType     shutdownPinType  = levelType::activeLow;
inline constexpr uint8_t       ampEnablePin     = 19;
inline constexpr levelType     ampEnablePinType = levelType::activeLow;
inline constexpr uint8_t       usbAccessPin     = 20;
inline constexpr levelType     usbAccessPinType = levelType::activeHigh;
inline constexpr uint8_t       openAnalogPin    = A7;
inline constexpr unsigned long cycleTime        = 50;
#endif /* ALLinONE_Plus */

#ifdef ALLinONE
// ####### buttons #####################################

inline constexpr uint32_t  buttonLongPress = 1000; // timeout for long press button in ms
inline constexpr uint8_t   buttonPausePin  = A0;

#ifdef BUTTONS3X3
inline constexpr uint8_t   button3x3Pin    = A2;
inline constexpr uint8_t   buttonUpPin     = A4;
inline constexpr uint8_t   buttonDownPin   = A3;
inline constexpr uint32_t  button3x3DbTime = 50; // Debounce time in milliseconds (default 50ms)
#else
#define FIVEBUTTONS
inline constexpr uint8_t   buttonUpPin     = A2;
inline constexpr uint8_t   buttonDownPin   = A1;
#endif

#ifdef FIVEBUTTONS
inline constexpr uint8_t   buttonFourPin   = A4;
inline constexpr uint8_t   buttonFivePin   = A3;
#endif

inline constexpr levelType buttonPinType   = levelType::activeLow;
inline constexpr uint32_t  buttonDbTime    = 25; // Debounce time in milliseconds (default 25ms)

// ####### chip_card ###################################

inline constexpr uint32_t cardCookie      = 0x1337b347;
inline constexpr uint8_t  cardVersion     = 0x02;
inline constexpr byte     mfrc522_RSTPin  =  9;
inline constexpr byte     mfrc522_SSPin   = 10;
inline constexpr uint8_t  cardRemoveDelay =  3;

// ####### mp3 #########################################

#define DFPlayerUsesSoftwareSerial
#define CHECK_MISSING_ONPLAYFINISHED

inline constexpr uint8_t       maxTracksInFolder        = 255;
inline constexpr uint8_t       dfPlayer_receivePin      = 2;
inline constexpr uint8_t       dfPlayer_transmitPin     = 3;
inline constexpr uint8_t       dfPlayer_busyPin         = 4;
inline constexpr levelType     dfPlayer_busyPinType     = levelType::activeHigh;
inline constexpr unsigned long dfPlayer_timeUntilStarts = 1000;

// ####### tonuino #####################################

inline constexpr uint8_t       shutdownPin      = 7;
inline constexpr levelType     shutdownPinType  = levelType::activeLow;
inline constexpr uint8_t       ampEnablePin     = 8;
inline constexpr levelType     ampEnablePinType = levelType::activeLow;
inline constexpr uint8_t       usbAccessPin     = A5;
inline constexpr levelType     usbAccessPinType = levelType::activeHigh;
inline constexpr uint8_t       openAnalogPin    = A7;
inline constexpr unsigned long cycleTime        = 50;
#endif /* ALLinONE */

// ####### some helper fuctions #####################################

template <typename T> void PROGMEM_read(const T * sce, T& dest)
{
  memcpy_P (&dest, sce, sizeof (T));
}



#endif /* SRC_CONSTANTS_HPP_ */
