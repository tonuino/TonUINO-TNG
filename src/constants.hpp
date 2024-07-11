#ifndef SRC_CONSTANTS_HPP_
#define SRC_CONSTANTS_HPP_

#include <Arduino.h>
#include "gpioHelper.hpp"

// ######################################################################
// ####### default pins #################################################
// ######################################################################

/* #### Classic/Every ######################################################################
 *                         | A0| A1| A2| A3| A4| A5| A6| A7| D0| D1| D2| D3| D5| D6| D7| D8|
 * ------------------------+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
 * Com to DF Player        |   |   |   |   |   |   |   |   |RX*|TX*| RX| TX|   |   |   |   |
 * 3 Button                | P | U | D |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * 5 Button                | P | V+| V-| U | D |   |   |   |   |   |   |   |   |   |   |   |
 * 3x3 Button Board        | P | U | D | A |   |   |   |   |   |   |   |   |   |   |   |   |
 * Open pin for random     |   |   |   |   |   |   |   | x |   |   |   |   |   |   |   |   |
 * Rotary encoder          |   |   |   |CLK| DT|   |   |   |   |   |   |   |   |   |   |   |
 * Poti                    |   |   |   | x |   |   |   |   |   |   |   |   |   |   |   |   |
 * Neo Ring/LED animat.    |   |   |   |   |   |   |   |   |   |   |(x)|   | x |   |   |   |
 * Speaker off             |   |   |   |   |   |   |   |   |   |   |   |   |   | x |   |   |
 * Shutdown                |   |   |   |   |   |   |   |   |   |   |   |   |   |   | x |   |
 * headphone jack detection|   |   |   |   |   |   |   |   |   |   |   |   |   |   |   | x |
 * special start shortcut  |   |   |   |   |   |   | x |   |   |   |   |   |   |   |   |   |
 * bat voltage measurement |   |   |   |   |   | x |   |   |   |   |   |   |   |   |   |   |
 * #########################################################################################
 *
 * (*) Hardware Serial on Every
 */

/* ### AiOplus #########################################################################################
 *                         | A0| A1| A2| A3| A4| A5| A6| A7|D10|D14|D19|D21|D27|D31|D32|D33|D36|D37|A14|
 *                         |   |   |   |   |   |   |   |   |PB2|PC0|PC5|PC7|PD5|PE1|PE2|PE3|PF2|PF3|PF4|
 * ------------------------+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
 * 3 Button                | P | D | U |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * 5 Button                | P | D | U | V-| V+|   |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * 3x3 Button Board        | P |   | A | D | U |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * Open pin for random     |   |   |   |   |   |   |   | x |   |   |   |   |   |   |   |   |   |   |   |
 * Rotary encoder          |   |   |   |   |   |   |   |   |   |   |   |   |   |CLK| DT|   |CLK| DT|   |
 * Poti                    |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   | x |
 * Neo Ring/LED animat.    |   |   |   |   |   |   |   |   | x |(x)|   |   |   |   |   |   |   |   |   |
 * Speaker off             |   |   |   |   |   |   |   |   |   |   | x |   |   |   |   |   |   |   |   |
 * Shutdown                |   |   |   |   |   |   |   |   |   |   |   |   | x |   |   |   |   |   |   |
 * headphone jack detection|   |   |   |   |   |   |   |   |   |   |   | x |   |   |   |   |   |   |   |
 * special start shortcut  |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   | x |   |   |   |
 * bat voltage measurement |   |   |   |   |   |   | x |   |   |   |   |   |   |   |   |   |   |   |   |
 * #####################################################################################################
 */

/* ### AiO #################################################################
 *                         | A0| A1| A2| A3| A4| A5| A6| A7| D5| D6| D7| D8|
 * ------------------------+---+---+---+---+---+---+---+---+---+---+---+---+
 * 3 Button                | P | D | U |   |   |   |   |   |   |   |   |   |
 * 5 Button                | P | D | U | V-| V+|   |   |   |   |   |   |   |
 * 3x3 Button Board        | P |   | A | D | U |   |   |   |   |   |   |   |
 * Open pin for random     |   |   |   |   |   |   |   | x |   |   |   |   |
 * Rotary encoder          |   |   |   |CLK| DT|   |   |   |   |   |   |   |
 * Poti                    |   |   |   | x |   |   |   |   |   |   |   |   |
 * Neo Ring/LED animat.    |   |   |   |   |   |   |   |   | x |   |   |   |
 * Speaker off             |   |   |   |   |   |   |   |   |   |   |   | x |
 * Shutdown                |   |   |   |   |   |   |   |   |   |   | x |   |
 * special start shortcut  |   |   |   |   |   |   | x |   |   |   |   |   |
 * #########################################################################
 */

// ######################################################################
// ####### variant and feature configuration ############################
// ######################################################################

/* Select the right PCB by uncommenting one of the following lines
 * Bitte die passende Platine durch entfernen der Kommentare in einer der folgenden Zeilen auswählen
 */
#define TonUINO_Classic
//#define TonUINO_Every
//#define ALLinONE
//#define ALLinONE_Plus

// ######################################################################

/* uncomment one of the below lines to enable special button support
 * um die Tasten zu konfigurieren, bitte eine der nächsten Zeilen auskommentieren
 * default: THREEBUTTONS for classic/every
 *          FIVEBUTTONS  for AiO and AiO+
 */
//#define THREEBUTTONS
#define FIVEBUTTONS
//#define BUTTONS3X3

// ######################################################################

/* If using Nano Every with connected DfPlayer Rx/Tx to D0/D1 uncomment the following lines
 * Wenn der Nano Every verwendet wird und Rx/Tx vom DfPlayer mit D0/D1 verbunden ist, den Kommentare der folgenden Zeile entfernen
 */
//#define DFPlayerUsesHardwareSerial

// ######################################################################

/* uncomment the below line to enable serial input as additional command source
 * um den Serial Monitor als zusätzliche Kommandoquelle zu haben bitte in der nächste Zeile den Kommentar entfernen
 * -7:             -8: up         -9: upLong
 * -4: allLong     -5: pause      -6: pauseLong
 * -1: up/downLong -2: down       -3: downLong
 * number n > 0: Springe im Voice Menü zum n-ten Eintrag und selektiere ihn
 */
#define SerialInputAsCommand

// ######################################################################

/* uncomment one of the below lines to support a special chip on the DfMiniMp3 player
 * um einen speziellen Chip auf dem DfMiniMp3 Player zu ünterstützen bitte in eine der nächste Zeilen den Kommentar entfernen
 *
 * GD3200B:     bad behavior of getFolderTrackCount() - ignores the parameter folder
 * MH2024K16SS: no checksums
 * LISP3:       bad behavior of callback OnPlayFinished - it is also called on advertise tracks (also on some MH2024K24SS)
 * LKP Player:  no ACK for requests (use Mp3ChipIncongruousNoAck for them)
 */
//#define DFMiniMp3_T_CHIP_GD3200B
//#define DFMiniMp3_T_CHIP_MH2024K16SS
//#define DFMiniMp3_T_CHIP_LISP3
#define DFMiniMp3_T_CHIP_Mp3ChipIncongruousNoAck

// ######################################################################

/* uncomment the below line to disable shutdown via button (long press play/pause)
 * um ein Shutdown via Taste (long press Play/Pause) zu unterdrücken bitte in der nächste Zeile den Kommentar entfernen
 */
//#define DISABLE_SHUTDOWN_VIA_BUTTON

// ######################################################################

/* uncomment the below line to enable the rotary encoder for volume setting
 * um den Drehgeber zu unterstützen bitte in der nächste Zeile den Kommentar entfernen
 */
//#define ROTARY_ENCODER
/* uncomment the below line to enable the rotary encoder also for next and previous (only one click in one second)
 * um den Drehgeber auch für vor und zurück zu unterstützen bitte in der nächste Zeile den Kommentar entfernen
 */
//#define ROTARY_ENCODER_LONGPRESS

#ifdef ALLinONE_Plus
// if using Rotary Encoder Buchse
//inline constexpr uint8_t   rotaryEncoderClkPin    = 31; // PE1
//inline constexpr uint8_t   rotaryEncoderDtPin     = 32; // PE2

// if using Opt Leiste (Male)
inline constexpr uint8_t   rotaryEncoderClkPin    = 36; // PF2
inline constexpr uint8_t   rotaryEncoderDtPin     = 37; // PF3
#else
inline constexpr uint8_t   rotaryEncoderClkPin    = A3; // A3
inline constexpr uint8_t   rotaryEncoderDtPin     = A4; // A4
#endif // ALLinONE_Plus

// ######################################################################

/* uncomment the below line to enable the poti for volume setting
 * um den Poti zu unterstützen bitte in der nächste Zeile den Kommentar entfernen
 */
//#define POTI
#ifdef ALLinONE_Plus
inline constexpr uint8_t   potiPin    = A14; // AiO+ PF4
#else
inline constexpr uint8_t   potiPin    = A3 ; // AiO/Classic A3
#endif // ALLinONE_Plus

// ######################################################################

/* uncomment the below line to enable the neo ring
 * To have more features (show volume setting) uncomment also NEO_RING_EXT
 * um den Neo Ring zu unterstützen bitte in der nächste Zeile den Kommentar entfernen
 * um weitere Features einzuschalten, auch den Kommentar für NEO_RING_EXT entfernen
 */
//#define NEO_RING
//#define NEO_RING_EXT
//#define NEO_RING_2

#ifdef ALLinONE_Plus
inline constexpr uint8_t neoPixelRingPin = 10; // PB2 on AiOplus (Erweiterungsleiste (Female))
#else
inline constexpr uint8_t neoPixelRingPin =  5; // D5 on AiO/Classic
#endif // ALLinONE_Plus
inline constexpr uint8_t neoPixelNumber  = 24; // Total Number of Pixels
#ifdef NEO_RING_2
#ifdef ALLinONE_Plus
inline constexpr uint8_t neoPixelRingPin2= 14; // PC0 on AiOplus (Erweiterungsleiste (Female))
#else
inline constexpr uint8_t neoPixelRingPin2=  2; // D2 on AiO/Classic (only Every)
#endif // ALLinONE_Plus
inline constexpr uint8_t neoPixelNumber2 = 24; // Total Number of Pixels
#endif // NEO_RING_2

// ######################################################################

/* uncomment the below line to enable the Speaker on/off for Classic to suppress noise
 * on startup and shutdown (automatically enabled on AiO and AiOplus)
 * um den Lautsprecher ein/aus Schalter für die Classic Variante zu unterstützen bitte
 * in der nächste Zeile den Kommentar entfernen (zur Unterdrückung der Ein- und Ausschaltgeräusche)
 * (automatisch eingeschaltet für AiO und AiOplus)
 */
//#define SPKONOFF
#if not defined(ALLinONE_Plus) and not defined(ALLinONE)
inline constexpr uint8_t       ampEnablePin     = 6;
inline constexpr levelType     ampEnablePinType = levelType::activeHigh;
#endif

// ######################################################################

/* uncomment the below line to enable the Headphone Jack detection (automatically enabled on AiOplus)
 * um die Kopfhörer Erkennung einzuschalten bitte in der nächste Zeile den Kommentar entfernen
 * (automatisch eingeschaltet für AiOplus)
 */
//#define HPJACKDETECT
#ifndef ALLinONE_Plus
inline constexpr uint8_t       dfPlayer_noHeadphoneJackDetect     = 8;
inline constexpr levelType     dfPlayer_noHeadphoneJackDetectType = levelType::activeLow;
#endif

// ######################################################################

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

// ######################################################################

/* uncomment the below line(s) to enable the quiz or memory game
 * um das Quiz oder Memory Spiel zu aktivieren, in der nächste Zeile(n) den Kommentar entfernen
 */
//#define QUIZ_GAME
//#define MEMORY_GAME

// ######################################################################

/* uncomment the below line to store the last played card in EEPROM
 * um die letzte Karte im EEPROM zu speichern, in der nächste Zeile den Kommentar entfernen
 */
//#define STORE_LAST_CARD

// ######################################################################

/* uncomment the below line to enable special shortcut on startup via GPIO
 * um den spezial Shortcut beim Start via GPIO zu aktivieren, in der nächste Zeile den Kommentar entfernen
 */
//#define SPECIAL_START_SHORTCUT
#ifdef ALLinONE_Plus
inline constexpr uint8_t   specialStartShortcutPin     = 33; // PE3
#else
inline constexpr uint8_t   specialStartShortcutPin     = A6; // A6 on AiO/Classic
#endif // ALLinONE_Plus
inline constexpr levelType specialStartShortcutPinType = levelType::activeHigh;
inline constexpr uint8_t   specialStartShortcutFolder  = 1;
inline constexpr uint8_t   specialStartShortcutTrack   = 1;

// ######################################################################

/* uncomment the below line to enable battery voltage measurement (not for ALLinONE, not recommended for TonUINO_Classic)
 * um die Batterie Spannungsmessung zu aktivieren, in der nächste Zeile den Kommentar entfernen
 * (nicht für ALLinONE, nicht empfohlen für TonUINO_Classic)
 */
//#define BAT_VOLTAGE_MEASUREMENT
#if not defined(ALLinONE_Plus) and not defined(ALLinONE)
inline constexpr uint8_t voltageMeasurementPin         = A5;
#endif // ALLinONE_Plus

#ifdef ALLinONE_Plus
inline constexpr float   voltageMeasurementCorrection  = 2.075; // Spannungsteiler 100k/100k
#endif
#ifdef TonUINO_Classic
inline constexpr float   voltageMeasurementCorrection  = 1.960; // Spannungsteiler 100k/100k
#endif
#ifdef TonUINO_Every
inline constexpr float   voltageMeasurementCorrection  = 2.007; // Spannungsteiler 100k/100k
#endif

inline constexpr float   batVoltageLow                 = 2.95;
inline constexpr float   batVoltageEmpty               = 2.90;

// ######################################################################

/* uncomment the below line if you use Pololu Powerswitch for shutdown
 * wenn der Pololu Powerswitch für das Shutdown verwendet wird, in der nächste Zeile den Kommentar entfernen
 */
//#define USE_POLOLU_SHUTDOWN

/* #################################################################################################
 * ##### normally, you don't have to edit lines below                   ############################
 * ##### mormalerweise müssen die folgende Zeilen nicht editiert werden ############################
 * #################################################################################################
 */

// ####### rules for buttons ############################

inline constexpr uint8_t lastSortCut         =  24;
#ifdef BUTTONS3X3
#ifdef FIVEBUTTONS
static_assert(false, "The 3x3 Button board doesn't have 5 Buttons");
#endif
inline constexpr uint8_t buttonExtSC_begin   = 101;
inline constexpr uint8_t buttonExtSC_buttons =  18; // <= lastSortCut
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

#ifdef DFPlayerUsesHardwareSerial
inline constexpr HardwareSerial &dfPlayer_serial         = Serial1; // D0 RX, D1 TX (Every)
#else
inline constexpr uint8_t       dfPlayer_receivePin      = 2;
inline constexpr uint8_t       dfPlayer_transmitPin     = 3;
#endif

inline constexpr uint8_t       maxTracksInFolder        = 255;
inline constexpr uint8_t       dfPlayer_busyPin         = 4;
inline constexpr levelType     dfPlayer_busyPinType     = levelType::activeHigh;
inline constexpr unsigned long dfPlayer_timeUntilStarts = 1000;

// ####### tonuino #####################################

inline constexpr uint8_t       shutdownPin      = 7;
#ifdef USE_POLOLU_SHUTDOWN
inline constexpr levelType     shutdownPinType  = levelType::activeHigh;
#else
inline constexpr levelType     shutdownPinType  = levelType::activeLow;
#endif
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

#define SPKONOFF
#define HPJACKDETECT

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
inline constexpr HardwareSerial &dfPlayer_serial         = Serial3;

inline constexpr uint8_t        maxTracksInFolder        = 255;
inline constexpr uint8_t        dfPlayer_busyPin         = 13;
inline constexpr levelType      dfPlayer_busyPinType     = levelType::activeHigh;
inline constexpr unsigned long  dfPlayer_timeUntilStarts = 1000;
inline constexpr uint8_t        dfPlayer_noHeadphoneJackDetect     = 21;
inline constexpr levelType      dfPlayer_noHeadphoneJackDetectType = levelType::activeLow;

// ####### tonuino #####################################

inline constexpr uint8_t       shutdownPin           = 27;
inline constexpr levelType     shutdownPinType       = levelType::activeLow;
inline constexpr uint8_t       ampEnablePin          = 19;
inline constexpr levelType     ampEnablePinType      = levelType::activeLow;
inline constexpr uint8_t       usbAccessPin          = 20;
inline constexpr levelType     usbAccessPinType      = levelType::activeHigh;
inline constexpr uint8_t       voltageMeasurementPin = A6;
inline constexpr uint8_t       openAnalogPin         = A7;
inline constexpr unsigned long cycleTime             = 50;
#endif /* ALLinONE_Plus */

/***************************************************************************
 ** AiO ********************************************************************
 ***************************************************************************/

#ifdef ALLinONE
// ####### buttons #####################################

#if not defined(THREEBUTTONS) and not defined(BUTTONS3X3)
#define FIVEBUTTONS
#endif

#define SPKONOFF

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

inline constexpr uint8_t       dfPlayer_receivePin      = 2;
inline constexpr uint8_t       dfPlayer_transmitPin     = 3;

inline constexpr uint8_t       maxTracksInFolder        = 255;
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
