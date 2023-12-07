#ifndef SRC_CONSTANTS_HPP_
#define SRC_CONSTANTS_HPP_

#include <Arduino.h>

/* Select the right PCB by uncommenting one of the following lines
 * Bitte die passende Platine durch entfernen der Kommentare in einer der folgenden Zeilen auswählen
 */
//#define TonUINO_Classic
//#define TonUINO_Every
//#define ALLinONE
//#define ALLinONE_Plus

/* uncomment one of the below lines to enable special button support
 * um die Tasten zu konfigurieren, bitte eine der nächsten Zeilen auskommentieren
 * default: THREEBUTTONS for classic/every
 *          FIVEBUTTONS  for AiO and AiO+
 */
//#define THREEBUTTONS
//#define FIVEBUTTONS
//#define BUTTONS3X3

/* uncomment the below line to enable serial input as additional command source
 * um den Serial Monitor als zusätzliche Kommandoquelle zu haben bitte in der nächste Zeile den Kommentar entfernen
 * -7:             -8: up         -9: upLong
 * -4: allLong     -5: pause      -6: pauseLong
 * -1: up/downLong -2: down       -3: downLong
 * number n > 0: Springe im Voice Menü zum n-ten Eintrag und selektiere ihn
 */
#define SerialInputAsCommand

/* uncomment one of the below lines to support a special chip on the DfMiniMp3 player
 * um einen speziellen Chip auf dem DfMiniMp3 Player zu ünterstützen bitte in eine der nächste Zeilen den Kommentar entfernen
 */
//#define DFMiniMp3_T_CHIP_GD3200B
//#define DFMiniMp3_T_CHIP_MH2024K16SS
//#define DFMiniMp3_T_CHIP_LISP3
#define DFMiniMp3_T_CHIP_Mp3ChipIncongruousNoAck

/* uncomment the below line to disable shutdown via button (long press play/pause)
 * um ein Shutdown via Taste (long press Play/Pause) zu unterdrücken bitte in der nächste Zeile den Kommentar entfernen
 */
//#define DISABLE_SHUTDOWN_VIA_BUTTON

/* uncomment the below line to enable the rotary encoder for volume setting (only for AiOplus)
 * um den Drehgeber zu unterstützen bitte in der nächste Zeile den Kommentar entfernen (nur für AioPlus)
 */
//#define ROTARY_ENCODER
#ifdef ALLinONE_Plus
// if using Rotary Encoder Buchse
//inline constexpr uint8_t   rotaryEncoderClkPin    = 31; // PE2
//inline constexpr uint8_t   rotaryEncoderDtPin     = 32; // PE3

// if using Opt Leiste (Male)
inline constexpr uint8_t   rotaryEncoderClkPin    = 36; // PF2
inline constexpr uint8_t   rotaryEncoderDtPin     = 37; // PF3
#endif // ALLinONE_Plus


/* uncomment the below line to enable the poti for volume setting
 * um den Poti zu unterstützen bitte in der nächste Zeile den Kommentar entfernen
 */
//#define POTI
#ifdef ALLinONE_Plus
inline constexpr uint8_t   potiPin    = A14; // AiO+ PF4
#else
inline constexpr uint8_t   potiPin    = A5 ; // AiO/Classic A5
#endif // ALLinONE_Plus

/* uncomment the below line to enable the neo ring
 * To have more features (show volume setting) uncomment also NEO_RING_EXT
 * um den Neo Ring zu unterstützen bitte in der nächste Zeile den Kommentar entfernen
 * um weitere Features einzuschalten, auch den Kommentar für NEO_RING_EXT entfernen
 */
//#define NEO_RING
//#define NEO_RING_EXT
#ifdef ALLinONE_Plus
inline constexpr uint8_t neoPixelRingPin = 10; // PB2 on AiOplus (Erweiterungsleiste (Female))
#else
inline constexpr uint8_t neoPixelRingPin =  5; // D5 on AiO/Classic
#endif // ALLinONE_Plus
inline constexpr uint8_t neoPixelNumber  = 24; // Total Number of Pixels

/* uncomment the below line to enable the Speaker on/off on Pin D6 for Classic to suppress noise
 * on startup and shutdown
 * um den Lautsprecher ein/aus Schalter über D6 für die Classic Variante zu unterstützen bitte
 * in der nächste Zeile den Kommentar entfernen (zur Unterdrückung der Ein- und Ausschaltgeräusche)
 */
//#define SPKONOFF

/* uncomment the below line to ignore the RFID if it is already playing
 * um die selbe RFID zu ignorieren, wenn die bereits spielt, in der nächste
 * Zeile den Kommentar entfernen
 */
//#define DONT_ACCEPT_SAME_RFID_TWICE

/* uncomment the below line to resume playback if the RFID that started the
 * playback is detected again
 * um die Wiedergabe fortzusetzen, wenn die selbe RFID erkannt wird, die bereits
 * abgespielt wird, in der nächste Zeile den Kommentar entfernen
 */
//#define RESUME_ON_SAME_RFID

/* uncomment the below line to replay the last card or short cut if pressed play/pause
 * in Idle state
 * um die letzte Karte oder den letzten Short Cut wieder abzuspielen, wenn die Play/Pause Taste
 * im Idle State gedrückt wird, in der nächste Zeile den Kommentar entfernen
 */
//#define REPLAY_ON_PLAY_BUTTON

/* #################################################################################################
 * ##### normally, you don't have to edit lines below                   ############################
 * ##### mormalerweise müssen die folgende Zeilen nicht editiert werden ############################
 * #################################################################################################
 */

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
// ####### rules for buttons ############################

#ifdef BUTTONS3X3
#ifdef FIVEBUTTONS
static_assert(false, "The 3x3 Button board doesn't have 5 Buttons");
#endif
inline constexpr uint8_t buttonExtSC_begin   = 101;
inline constexpr uint8_t buttonExtSC_buttons =  18;
#endif // BUTTONS3X3

inline constexpr uint32_t  buttonLongPress       = 1000; // timeout for long press button in ms
inline constexpr uint32_t  buttonLongPressRepeat =  200; // timeout for long press button repeat in ms

/***************************************************************************
 ** Classic ****************************************************************
 ***************************************************************************/

#if defined(TonUINO_Classic) or defined(TonUINO_Every)
// ####### buttons #####################################

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

inline constexpr uint8_t       shutdownPin      = 7;
inline constexpr levelType     shutdownPinType  = levelType::activeHigh;
inline constexpr uint8_t       ampEnablePin     = 6;
inline constexpr levelType     ampEnablePinType = levelType::activeHigh;
inline constexpr uint8_t       openAnalogPin    = A7;
inline constexpr unsigned long cycleTime        = 50;
#endif /* TonUINO_Classic or TonUINO_Every */

/***************************************************************************
 ** AiO plus ***************************************************************
 ***************************************************************************/

#ifdef ALLinONE_Plus
// ####### buttons #####################################

#if not defined(THREEBUTTONS) and not defined(BUTTONS3X3)
#define FIVEBUTTONS
#endif

inline constexpr uint8_t   buttonPausePin  = A0;

#ifdef BUTTONS3X3
inline constexpr uint8_t   button3x3Pin    = A2;
inline constexpr uint8_t   buttonUpPin     = A4;
inline constexpr uint8_t   buttonDownPin   = A3;
inline constexpr uint32_t  button3x3DbTime = 50; // Debounce time in milliseconds (default 50ms)
#else
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

/***************************************************************************
 ** AiO ********************************************************************
 ***************************************************************************/

#ifdef ALLinONE
// ####### buttons #####################################

#if not defined(THREEBUTTONS) and not defined(BUTTONS3X3)
#define FIVEBUTTONS
#endif

inline constexpr uint8_t   buttonPausePin  = A0;

#ifdef BUTTONS3X3
inline constexpr uint8_t   button3x3Pin    = A2;
inline constexpr uint8_t   buttonUpPin     = A4;
inline constexpr uint8_t   buttonDownPin   = A3;
inline constexpr uint32_t  button3x3DbTime = 50; // Debounce time in milliseconds (default 50ms)
#else
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
