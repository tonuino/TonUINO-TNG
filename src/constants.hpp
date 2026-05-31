#ifndef SRC_CONSTANTS_HPP_
#define SRC_CONSTANTS_HPP_

#include <Arduino.h>

// ######################################################################
// ####### default pins #################################################
// ######################################################################

/* #### Classic/Every/Esp32-Nano ###########################################################
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
 * Bluetooth ON/OFF        |   |   |   |   |   |   |   |   |   |   | x*|   |   | x |   |   |
 * Bluetooth Pairing       |   |   |   |   |   |   |   |   |   |   |   | x*|   |   |   | x |
 * Button LED Pins         |   |   |   |   |   | P |   |   |   |   |   |   | D | U |   |   |
 * #########################################################################################
 *
 * (*) Hardware Serial on Every/Esp32-Nano
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

/* #### Esp32-Wroom32 ##############################################################
 * GPIO                    | 33| 25| 26| 27| 44| 35| 12| 39| 16| 17|  2| 15| 13| 34|
 * Pin                     |D33|D25|D26|D27|D14|D35|D12| VN|RX2|TX2| D2|D15|D13|D34|
 * ------------------------+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
 * Com to DF Player        |   |   |   |   |   |   |   |   | RX| TX|   |   |   |   |
 * 3 Button                | P | U | D |   |   |   |   |   |   |   |   |   |   |   |
 * 5 Button                | P | V+| V-| U | D |   |   |   |   |   |   |   |   |   |
 * 3x3 Button Board        | P | U | D | A |   |   |   |   |   |   |   |   |   |   |
 * Open pin for random     |   |   |   |   |   |   |   | x |   |   |   |   |   |   |
 * Rotary encoder          |   |   |   |CLK| DT|   |   |   |   |   |   |   |   |   |
 * Poti                    |   |   |   | x |   |   |   |   |   |   |   |   |   |   |
 * Neo Ring/LED animat.    |   |   |   |   |   |   |   |   |   |   | x |   |   |   |
 * Speaker off             |   |   |   |   |   |   |   |   |   |   |   | x |   |   |
 * Shutdown                |   |   |   |   |   |   |   |   |   |   |   |   | x |   |
 * headphone jack detection|   |   |   |   |   |   |   |   |   |   |   |   |   | x |
 * special start shortcut  |   |   |   |   |   |   | x |   |   |   |   |   |   |   |
 * bat voltage measurement |   |   |   |   |   | x |   |   |   |   |   |   |   |   |
 * #################################################################################
 */

// ######################################################################
// ####### variant and feature configuration ############################
// ######################################################################

/* Select the right PCB by uncommenting one of the following lines
 * EN: Select the matching board by uncommenting one of the lines below.
 * ES: Selecciona la placa correcta descomentando una de las lineas siguientes
 */
//#define TROVALIBRE_CLASSIC
//#define TROVALIBRE_EVERY
//#define TROVALIBRE_EVERY_4808
//#define ALLinONE
//#define ALLinONE_Plus
//#define TROVALIBRE_ESP32 100 // Esp32 Nano
//#define TROVALIBRE_ESP32 200 // Esp32 Wroom 32

#include "gpioHelper.hpp"

#if defined(TROVALIBRE_CLASSIC)
DECL_PCB(1)
#endif
#if defined(TROVALIBRE_EVERY)
DECL_PCB(2)
#endif
#if defined(TROVALIBRE_EVERY_4808)
DECL_PCB(3)
#endif
#if defined(ALLinONE)
DECL_PCB(4)
#endif
#if defined(ALLinONE_Plus)
DECL_PCB(5)
#endif
#if defined(TROVALIBRE_ESP32)
DECL_PCB(6)
#endif
static_assert(SUM_PCB == 1 , "Please uncomment exactly one of the PCB lines (TROVALIBRE_CLASSIC, TROVALIBRE_EVERY, etc.). Please uncomment exactly one board definition line. / Descomenta exactamente una linea de definicion de placa (TROVALIBRE_CLASSIC, TROVALIBRE_EVERY, etc.).");

// ######################################################################

/* uncomment one of the below lines to enable special button support
 * EN: Configure buttons by uncommenting one of the lines below.
 * ES: Configura los botones descomentando una de las lineas siguientes
 * default: THREEBUTTONS for classic/every
 *          FIVEBUTTONS  for AiO and AiO+
 */
//#define THREEBUTTONS
//#define FIVEBUTTONS
//#define BUTTONS3X3

// ######################################################################

/* If using Nano Every with connected DfPlayer Rx/Tx to D0/D1 uncomment the following lines
 * EN: If Nano Every uses DfPlayer Rx/Tx on D0/D1, uncomment the next line.
 * ES: Si Nano Every usa Rx/Tx de DfPlayer en D0/D1, descomenta la linea siguiente
 */
//#define DFPlayerUsesHardwareSerial

// ######################################################################

/* uncomment the below line to enable serial input as additional command source
 * EN: Uncomment the next line to use Serial Monitor as an extra command source.
 * ES: Descomenta la linea siguiente para usar el monitor serie como fuente extra de comandos
 * -7:             -8: up         -9: upLong
 * -4: allLong     -5: pause      -6: pauseLong
 * -1: up/downLong -2: down       -3: downLong
 * number n > 0: EN: Jump to the nth voice-menu item and select it.
 * ES: Saltar al elemento n del menu de voz y seleccionarlo
 */
//#define SerialInputAsCommand

// ######################################################################

/* uncomment one of the below lines to support a special chip on the DfMiniMp3 player
 * EN: Uncomment one of the next lines to support specific DfMiniMp3 chips.
 * ES: Descomenta una de las lineas siguientes para soportar chips especificos de DfMiniMp3
 *
 * GD3200B:     bad behavior of getFolderTrackCount() - ignores the parameter folder
 *              bad behavior of callback OnPlayFinished - it is also called on advertise tracks
 * MH2024K16SS: no checksums
 * LISP3:       bad behavior of callback OnPlayFinished - it is also called on advertise tracks
 * MP3-TF-16P V3.0 with MH2024K24SS:
 *              very slow
 *              bad behavior of callback OnPlayFinished - it is also called on advertise tracks
 * LKP Player:  no ACK for requests (use Mp3ChipIncongruousNoAck for them)
 */
//#define DFMiniMp3_T_CHIP_GD3200B
//#define DFMiniMp3_T_CHIP_MH2024K16SS
//#define DFMiniMp3_T_CHIP_LISP3
//#define DFMiniMp3_T_CHIP_MH2024K24SS_MP3_TF_16P_V3_0
#define DFMiniMp3_T_CHIP_Mp3ChipIncongruousNoAck

// ######################################################################

/* uncomment the below line to disable shutdown via button (long press play/pause)
 * EN: Uncomment the next line to disable shutdown via button (long press Play/Pause).
 * ES: Descomenta la linea siguiente para desactivar el apagado por boton (pulsacion larga Play/Pause).
 */
//#define DISABLE_SHUTDOWN_VIA_BUTTON

// ######################################################################

/* uncomment the below line to enable the rotary encoder for volume setting
 * EN: Uncomment the next line to enable the rotary encoder.
 * ES: Descomenta la linea siguiente para activar el encoder rotatorio
 */
//#define ROTARY_ENCODER
/* uncomment the below line to enable the rotary encoder also for next and previous (only one click in one second)
 * EN: Uncomment the next line to use rotary encoder for previous/next track too.
 * ES: Descomenta la linea siguiente para usar tambien el encoder para anterior/siguiente
 */
//#define ROTARY_ENCODER_LONGPRESS

#ifdef ROTARY_ENCODER
#ifdef ALLinONE_Plus
// if using Rotary Encoder Buchse
//inline constexpr uint8_t   rotaryEncoderClkPin    = 31; // PE1
//inline constexpr uint8_t   rotaryEncoderDtPin     = 32; // PE2

// if using Opt Leiste (Male)
inline constexpr uint8_t   rotaryEncoderClkPin    = 36; // PF2
inline constexpr uint8_t   rotaryEncoderDtPin     = 37; // PF3

#elif TROVALIBRE_ESP32 == 200
inline constexpr uint8_t   rotaryEncoderClkPin    = 27; // (D27)
inline constexpr uint8_t   rotaryEncoderDtPin     = 14; // (D14)

#else
inline constexpr uint8_t   rotaryEncoderClkPin    = A3; // A3
inline constexpr uint8_t   rotaryEncoderDtPin     = A4; // A4
#endif // ALLinONE_Plus

inline constexpr uint8_t   rotaryEncoderDebounce  = 75; // ms
inline constexpr bool      rotaryEncoderPullUp    = false;
#endif // ROTARY_ENCODER

// ######################################################################

/* uncomment the below line to enable the poti for volume setting
 * EN: Uncomment the next line to enable the potentiometer.
 * ES: Descomenta la linea siguiente para activar el potenciometro.
 */
//#define POTI

#ifdef POTI
#ifdef ALLinONE_Plus
inline constexpr uint8_t   potiPin    = A14; // AiO+ PF4

#elif TROVALIBRE_ESP32 == 200
inline constexpr uint8_t   potiPin    = 27 ; // (D27)

#else
inline constexpr uint8_t   potiPin    = A3 ; // AiO/Classic A3
#endif // ALLinONE_Plus
#endif // POTI

// ######################################################################

/* uncomment the below line to enable the neo ring
 * To have more features (show volume setting) uncomment also NEO_RING_EXT
 * EN: Uncomment the next line to enable the Neo Ring.
 * ES: Descomenta la linea siguiente para activar el Neo Ring.
 * EN: Uncomment NEO_RING_EXT to enable extra ring features.
 * ES: Descomenta NEO_RING_EXT para activar funciones extra del anillo.
 */
//#define NEO_RING
//#define NEO_RING_EXT
//#define NEO_RING_2

#ifdef NEO_RING
#ifdef ALLinONE_Plus
inline constexpr uint8_t neoPixelRingPin = 10; // PB2 on AiOplus (EN: Expansion header (female). ES: Cabecera de expansion (hembra).)

#elif TROVALIBRE_ESP32 == 200
inline constexpr uint8_t neoPixelRingPin =  2; // GPIO02 (D2) on ESP32 Wroom 32

#else
inline constexpr uint8_t neoPixelRingPin = D5; // D5 on AiO/Classic
#endif // ALLinONE_Plus

inline constexpr uint8_t neoPixelNumber  = 24; // Total Number of Pixels

#ifdef NEO_RING_2
#ifdef ALLinONE_Plus
inline constexpr uint8_t neoPixelRingPin2= 14; // PC0 on AiOplus (EN: Expansion header (female). ES: Cabecera de expansion (hembra).)

#else
inline constexpr uint8_t neoPixelRingPin2= D2; // D2 on AiO/Classic (only Every)
#endif // ALLinONE_Plus
inline constexpr uint8_t neoPixelNumber2 = 24; // Total Number of Pixels
#endif // NEO_RING_2

#endif // NEO_RING

// ######################################################################

/* uncomment the below line to enable the Speaker on/off for Classic to suppress noise
 * on startup and shutdown (automatically enabled on AiO and AiOplus)
 * EN: Enable speaker on/off switch support for the Classic variant by uncommenting the next line.
 * ES: Activa el soporte de interruptor de altavoz para la variante Classic descomentando la linea siguiente.
 * EN: This helps suppress power-on and power-off noise.
 * ES: Esto ayuda a suprimir ruido al encender y apagar.
 * EN: Automatically enabled for AiO and AiOplus.
 * ES: Se activa automaticamente en AiO y AiOplus.
 */
//#define SPKONOFF

#ifdef SPKONOFF
#if not defined(ALLinONE_Plus) and not defined(ALLinONE) and not (TROVALIBRE_ESP32 == 200)
inline constexpr uint8_t       ampEnablePin     = D6;
inline constexpr levelType     ampEnablePinType = levelType::activeHigh;

#elif (TROVALIBRE_ESP32 == 200)
inline constexpr uint8_t       ampEnablePin     = 15; // (D15)
inline constexpr levelType     ampEnablePinType = levelType::activeLow;
#endif
#endif // SPKONOFF

// ######################################################################

/* uncomment the below line to enable the Headphone Jack detection (automatically enabled on AiOplus)
 * EN: Uncomment the next line to enable headphone detection.
 * ES: Descomenta la linea siguiente para activar la deteccion de auriculares.
 * EN: Automatically enabled for AiOplus.
 * ES: Se activa automaticamente en AiOplus.
 */
//#define HPJACKDETECT

#ifdef HPJACKDETECT
#if not defined(ALLinONE_Plus)  and not (TROVALIBRE_ESP32 == 200)
inline constexpr uint8_t       dfPlayer_noHeadphoneJackDetect     = D8;
inline constexpr levelType     dfPlayer_noHeadphoneJackDetectType = levelType::activeLow;

#elif (TROVALIBRE_ESP32 == 200)
inline constexpr uint8_t        dfPlayer_noHeadphoneJackDetect     = 34; // (D34)
inline constexpr levelType      dfPlayer_noHeadphoneJackDetectType = levelType::activeLow;
#endif
#endif // HPJACKDETECT

// ######################################################################

/* uncomment the below line to ignore the RFID if it is already playing
 * EN: Uncomment to ignore the same RFID while ready prompts are playing.
 * ES: Descomenta para ignorar la misma RFID mientras se reproducen mensajes de estado.
 * Zeile den Kommentar entfernen
 */
//#define DONT_ACCEPT_SAME_RFID_TWICE

/* uncomment the below line to resume playback if the RFID that started the
 * playback is detected again
 * EN: Uncomment to resume playback when the same RFID is detected and ready prompts are active.
 * ES: Descomenta para reanudar reproduccion cuando se detecta la misma RFID y hay mensajes de estado.
 * EN: Enable this option by uncommenting the next line.
 * ES: Activa esta opcion descomentando la linea siguiente.
 */
//#define RESUME_ON_SAME_RFID

/* uncomment the below line to replay the last card or short cut if pressed play/pause
 * in Idle state
 * EN: Uncomment to replay the last card or last shortcut when Play/Pause is pressed
 * EN: while in Idle state.
 * ES: en estado Idle.
 * ES: Activa esta opcion descomentando la linea siguiente.
 */
//#define REPLAY_ON_PLAY_BUTTON

// ######################################################################

/* uncomment the below line(s) to enable the quiz, memory teapot game
 * EN: Uncomment the next line(s) to enable quiz, memory, or teapot games.
 * ES: Descomenta las lineas siguientes para activar quiz, memoria o juego de teapot.
 */
//#define QUIZ_GAME
//#define MEMORY_GAME
//#define TEAPOT_GAME

// ######################################################################

/* uncomment the below line to store the last played card in EEPROM
 * EN: Uncomment to store the last card in EEPROM.
 * ES: Descomenta para guardar la ultima tarjeta en EEPROM.
 */
//#define STORE_LAST_CARD

// ######################################################################

/* uncomment the below line to enable special shortcut on startup via GPIO
 * EN: Uncomment to enable the special startup shortcut via GPIO.
 * ES: Descomenta para activar el acceso directo especial al inicio via GPIO.
 */
//#define SPECIAL_START_SHORTCUT

#ifdef SPECIAL_START_SHORTCUT
#ifdef ALLinONE_Plus
inline constexpr uint8_t   specialStartShortcutPin     = 33; // PE3

#elif (TROVALIBRE_ESP32 == 200)
inline constexpr uint8_t   specialStartShortcutPin     = 12; // D12

#else
inline constexpr uint8_t   specialStartShortcutPin     = A6; // A6 on AiO/Classic
#endif // ALLinONE_Plus

inline constexpr levelType specialStartShortcutPinType = levelType::activeHigh;
inline constexpr uint8_t   specialStartShortcutFolder  = 1;
inline constexpr uint8_t   specialStartShortcutTrack   = 1;
#endif // SPECIAL_START_SHORTCUT

// ######################################################################

/* uncomment the below line to enable support for BT module
 * EN: Uncomment to enable BT module support.
 * ES: Descomenta para activar el soporte del modulo BT.
 */
//#define BT_MODULE

#ifdef BT_MODULE
#ifdef DFPlayerUsesHardwareSerial
inline constexpr uint8_t   btModuleOnPin               =  D2; // D2
inline constexpr uint8_t   btModulePairingPin          =  D3; // D3

#else
inline constexpr uint8_t   btModuleOnPin               =  D6; // D6
inline constexpr uint8_t   btModulePairingPin          =  D8; // D8
#endif

inline constexpr levelType btModuleOnPinType           = levelType::activeHigh;
inline constexpr levelType btModulePairingPinType      = levelType::activeHigh;
inline constexpr unsigned long btModulePairingPulse    = 500;
#endif // BT_MODULE

// ######################################################################

/* uncomment the below line to enable battery voltage measurement (not for ALLinONE, not recommended for TROVALIBRE_CLASSIC)
 * EN: Uncomment to enable battery voltage measurement.
 * ES: Descomenta para activar la medicion de voltaje de bateria.
 * EN: Not for ALLinONE and not recommended for TROVALIBRE_CLASSIC.
 * ES: No para ALLinONE y no recomendado para TROVALIBRE_CLASSIC.
 */
//#define BAT_VOLTAGE_MEASUREMENT

#ifdef BAT_VOLTAGE_MEASUREMENT
#if not defined(ALLinONE_Plus) and not defined(ALLinONE) and not (TROVALIBRE_ESP32 == 200)
inline constexpr uint8_t voltageMeasurementPin         = A5;

#elif (TROVALIBRE_ESP32 == 200)
inline constexpr uint8_t voltageMeasurementPin         = 35; // (D35)
#endif // ALLinONE_Plus

#ifdef ALLinONE_Plus
inline constexpr float   voltageMeasurementCorrection  = 2.075; // EN: Voltage divider 100k/100k. ES: Divisor de tension 100k/100k.
#endif
#ifdef TROVALIBRE_CLASSIC
inline constexpr float   voltageMeasurementCorrection  = 1.960; // EN: Voltage divider 100k/100k. ES: Divisor de tension 100k/100k.
#endif
#ifdef TROVALIBRE_EVERY
inline constexpr float   voltageMeasurementCorrection  = 2.007; // EN: Voltage divider 100k/100k. ES: Divisor de tension 100k/100k.
#endif
#ifdef TROVALIBRE_EVERY_4808
inline constexpr float   voltageMeasurementCorrection  = 2.007; // EN: Voltage divider 100k/100k. ES: Divisor de tension 100k/100k.
#endif
#ifdef TROVALIBRE_ESP32
inline constexpr float   voltageMeasurementCorrection  = 0.710; // EN: Voltage divider 100k/100k. ES: Divisor de tension 100k/100k.
#endif

// for Li-Ion
inline constexpr float   batVoltageLow                 = 2.95;
inline constexpr float   batVoltageEmpty               = 2.90;
// for Li-PO
//inline constexpr float   batVoltageLow                 = 3.40;
//inline constexpr float   batVoltageEmpty               = 3.20;
#endif // BAT_VOLTAGE_MEASUREMENT

// ######################################################################

/* uncomment the below lines if you use Pololu Powerswitch or carrier board for shutdown
 * EN: Uncomment the next line when using Pololu powerswitch or carrier board for shutdown.
 * ES: Descomenta la linea siguiente si usas powerswitch Pololu o placa carrier para apagado.
 */
//#define USE_POLOLU_SHUTDOWN
//#define USE_TRAEGER_PLATINE_SHUTDOWN

// ######################################################################

/* uncomment one of the below lines if you want to change the antenna gain of the MFRC522 module
 * EN: Uncomment one of the next lines to adjust MFRC522 antenna gain.
 * ES: Descomenta una de las lineas siguientes para ajustar la ganancia de antena MFRC522.
 */
//#define MRFC522_RX_GAIN RxGain_18dB
//#define MRFC522_RX_GAIN RxGain_23dB
//#define MRFC522_RX_GAIN RxGain_33dB // default
//#define MRFC522_RX_GAIN RxGain_38dB
//#define MRFC522_RX_GAIN RxGain_43dB
//#define MRFC522_RX_GAIN RxGain_48dB
//#define MRFC522_RX_GAIN RxGain_min  // 18dB
//#define MRFC522_RX_GAIN RxGain_avg  // 33dB
//#define MRFC522_RX_GAIN RxGain_max  // 48dB

// ######################################################################

/* uncomment the below lines if you want to have the Jukebox modification card
 * EN: Uncomment the next line to enable Jukebox mode.
 * ES: Descomenta la linea siguiente para activar el modo Jukebox.
 */
//#define MODIFICATION_CARD_JUKEBOX

#ifdef MODIFICATION_CARD_JUKEBOX
inline constexpr uint8_t jukebox_max_cards  = 10;
#endif

// ######################################################################

/* uncomment the below lines if you want to have the Pause after Track modification card
 * EN: Uncomment the next line to enable the pause-after-each-track modifier card.
 * ES: Descomenta la linea siguiente para activar la tarjeta modificadora de pausa tras cada pista.
 */
//#define MODIFICATION_CARD_PAUSE_AFTER_TRACK

// ######################################################################

/* Enable this definition to activate animated LED button support via the LedManager.
 *
 * LED behavior:
 * - Startup:        All LEDs blink one after another.
 * - Awaiting input: All LEDs blink synchronously.
 * - Playing:        All LEDs stay on continuously.
 * - Paused:         Only the Play/Pause LED blinks.
 * - Shutdown:       All LEDs are turned off.
 * - On a valid button press, all LEDs blink once by toggling their current state.
 *
 * Use a series resistor (e.g. 1KΩ) per LED to limit current.
 *
 * ----------------------------------------------------------------------
 *
 * EN: Uncomment the line below to enable animated LED buttons via LedManager.
 * ES: Descomenta la linea inferior para activar botones LED animados con LedManager.
 *
 * LED-Verhalten:
 * - Beim Start:          Alle LEDs blinken nacheinander.
 * - Wartet auf Eingabe:  Alle LEDs blinken synchron.
 * EN: Playback active: all LEDs stay on.
 * ES: Reproduccion activa: todos los LED permanecen encendidos.
 * EN: Playback paused: only the Play/Pause LED blinks.
 * ES: Reproduccion en pausa: solo parpadea el LED de Play/Pause.
 * - Abschaltung:         Alle LEDs werden ausgeschalten.
 * EN: On a valid button press, all LEDs blink once by toggling their current state.
 * ES: En una pulsacion valida, todos los LED parpadean una vez invirtiendo su estado actual.
 *
 * Verwende einen Widerstand (z.B. 1KΩ) in Reihe zu jeder LED zur Strombegrenzung.
 */

//#define USE_LED_BUTTONS

#ifdef USE_LED_BUTTONS
inline constexpr uint8_t led_play_pin = A5;
inline constexpr uint8_t led_up_pin   = D6;
inline constexpr uint8_t led_down_pin = D5;

inline constexpr unsigned long led_update_interval = 800; // ms
inline constexpr unsigned long led_short_blink     = 400; // ms
#endif // USE_LED_BUTTONS

//#define POWER_HOLD_READY

#ifdef POWER_HOLD_READY
inline constexpr uint8_t power_hold_ready_pin        = D2;
inline constexpr levelType power_hold_ready_pin_type = levelType::activeHigh;
#endif

/* #################################################################################################
 * ##### normally, you don't have to edit lines below                   ############################
 * ##### EN: Normally the lines below do not need editing.
 * ##### ES: Normalmente no hace falta editar las lineas siguientes. ############################
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

#if defined(TROVALIBRE_CLASSIC) or defined(TROVALIBRE_EVERY) or defined(TROVALIBRE_EVERY_4808)
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
inline constexpr levelType     dfPlayer_busyPinType     = levelType::activeLow;
#if defined(DFMiniMp3_T_CHIP_MH2024K24SS_MP3_TF_16P_V3_0)
inline constexpr unsigned long dfPlayer_timeUntilStarts = 2500;
#elif defined(DFMiniMp3_T_CHIP_GD3200B)
inline constexpr unsigned long dfPlayer_timeUntilStarts = 2500;
#else
inline constexpr unsigned long dfPlayer_timeUntilStarts = 1200;
#endif

// ####### trovaLibre #####################################

inline constexpr uint8_t       shutdownPin      = 7;
#ifdef USE_TRAEGER_PLATINE_SHUTDOWN
inline constexpr levelType     shutdownPinType  = levelType::activeLow;
#else
inline constexpr levelType     shutdownPinType  = levelType::activeHigh;
#endif
inline constexpr uint8_t       openAnalogPin    = A7;
inline constexpr unsigned long cycleTime        = 50;
#endif /* TROVALIBRE_CLASSIC or TROVALIBRE_EVERY or TROVALIBRE_EVERY_4808 */

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
inline constexpr levelType      dfPlayer_busyPinType     = levelType::activeLow;
inline constexpr unsigned long  dfPlayer_timeUntilStarts = 1200;
inline constexpr uint8_t        dfPlayer_noHeadphoneJackDetect     = 21;
inline constexpr levelType      dfPlayer_noHeadphoneJackDetectType = levelType::activeLow;

// ####### trovaLibre #####################################

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
inline constexpr levelType     dfPlayer_busyPinType     = levelType::activeLow;
inline constexpr unsigned long dfPlayer_timeUntilStarts = 1200;

// ####### trovaLibre #####################################

inline constexpr uint8_t       shutdownPin      = 7;
inline constexpr levelType     shutdownPinType  = levelType::activeLow;
inline constexpr uint8_t       ampEnablePin     = 8;
inline constexpr levelType     ampEnablePinType = levelType::activeLow;
inline constexpr uint8_t       usbAccessPin     = A5;
inline constexpr levelType     usbAccessPinType = levelType::activeHigh;
inline constexpr uint8_t       openAnalogPin    = A7;
inline constexpr unsigned long cycleTime        = 50;
#endif /* ALLinONE */

/***************************************************************************
 ** Esp32 ******************************************************************
 ***************************************************************************/

#if defined(TROVALIBRE_ESP32)

/***************************************************************************
 ** Esp32 Nano *************************************************************
 ***************************************************************************/

#if TROVALIBRE_ESP32 == 100
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
inline constexpr byte     mfrc522_RSTPin  =  D9;
inline constexpr byte     mfrc522_SSPin   = D10;
inline constexpr uint8_t  cardRemoveDelay =   3;

// ####### mp3 #########################################

#ifdef DFPlayerUsesHardwareSerial
inline constexpr HardwareSerial &dfPlayer_serial         = Serial0; // D0 RX, D1 TX (Esp32)
#else
inline constexpr uint8_t       dfPlayer_receivePin      = D2;
inline constexpr uint8_t       dfPlayer_transmitPin     = D3;
#endif

inline constexpr uint8_t       maxTracksInFolder        = 255;
inline constexpr uint8_t       dfPlayer_busyPin         = D4;
inline constexpr levelType     dfPlayer_busyPinType     = levelType::activeLow;
#if defined(DFMiniMp3_T_CHIP_MH2024K24SS_MP3_TF_16P_V3_0)
inline constexpr unsigned long dfPlayer_timeUntilStarts = 2500;
#elif defined(DFMiniMp3_T_CHIP_GD3200B)
inline constexpr unsigned long dfPlayer_timeUntilStarts = 2500;
#else
inline constexpr unsigned long dfPlayer_timeUntilStarts = 1200;
#endif

// ####### trovaLibre #####################################

inline constexpr uint8_t       shutdownPin      = D7;
#ifdef USE_TRAEGER_PLATINE_SHUTDOWN
inline constexpr levelType     shutdownPinType  = levelType::activeLow;
#else
inline constexpr levelType     shutdownPinType  = levelType::activeHigh;
#endif
inline constexpr uint8_t       openAnalogPin    = A7;
inline constexpr unsigned long cycleTime        = 50;


/***************************************************************************
 ** Esp32-Core Development Board *******************************************
 ***************************************************************************/

#elif TROVALIBRE_ESP32 == 200
// ####### buttons #####################################

inline constexpr uint8_t   buttonPausePin  = 33; // (D33)

#if defined(BUTTONS3X3)
inline constexpr uint8_t   button3x3Pin    = 27; // ADC2_7(D27)
inline constexpr uint8_t   buttonUpPin     = 25; // (D25)
inline constexpr uint8_t   buttonDownPin   = 26; // (D26)
inline constexpr uint32_t  button3x3DbTime = 50; // Debounce time in milliseconds (default 50ms)
#elif defined(FIVEBUTTONS)
inline constexpr uint8_t   buttonUpPin     = 27; // (D27)
inline constexpr uint8_t   buttonDownPin   = 14; // (D14)
inline constexpr uint8_t   buttonFourPin   = 25; // (D25)
inline constexpr uint8_t   buttonFivePin   = 26; // (D26)
#else
inline constexpr uint8_t   buttonUpPin     = 25; // (D25)
inline constexpr uint8_t   buttonDownPin   = 26; // (D26)
#endif

inline constexpr levelType buttonPinType   = levelType::activeLow;
inline constexpr uint32_t  buttonDbTime    = 25; // Debounce time in milliseconds (default 25ms)

// ####### chip_card ###################################

inline constexpr uint32_t cardCookie      = 0x1337b347;
inline constexpr uint8_t  cardVersion     = 0x02;
inline constexpr byte     mfrc522_RSTPin  =  22; // (D22)
inline constexpr byte     mfrc522_SSPin   =   5; // (D5)
inline constexpr uint8_t  cardRemoveDelay =   3;

// ####### mp3 #########################################

#ifdef DFPlayerUsesHardwareSerial
inline constexpr HardwareSerial &dfPlayer_serial         = Serial2; // GPIO16 (6) RX, GPIO17 (7) TX (Esp32 Wroom)
#else
inline constexpr uint8_t       dfPlayer_receivePin      =  16; // (RX2)
inline constexpr uint8_t       dfPlayer_transmitPin     =  17; // (TX2)
#endif

inline constexpr uint8_t       maxTracksInFolder        = 255;
inline constexpr uint8_t       dfPlayer_busyPin         =   4; // (D4)
inline constexpr levelType     dfPlayer_busyPinType     = levelType::activeLow;
#if defined(DFMiniMp3_T_CHIP_MH2024K24SS_MP3_TF_16P_V3_0)
inline constexpr unsigned long dfPlayer_timeUntilStarts = 2500;
#elif defined(DFMiniMp3_T_CHIP_GD3200B)
inline constexpr unsigned long dfPlayer_timeUntilStarts = 2500;
#else
inline constexpr unsigned long dfPlayer_timeUntilStarts = 1200;
#endif

// ####### trovaLibre #####################################

inline constexpr uint8_t       shutdownPin      = 13; // (D13)
#ifdef USE_POLOLU_SHUTDOWN
inline constexpr levelType     shutdownPinType  = levelType::activeHigh;
#else
inline constexpr levelType     shutdownPinType  = levelType::activeLow;
#endif
inline constexpr uint8_t       openAnalogPin    = 39; // (VN)
inline constexpr unsigned long cycleTime        = 50;

#else //  TROVALIBRE_ESP32 == *
static_assert(false, "Not supported Esp32 HW type");
#endif  //  TROVALIBRE_ESP32 == *

#endif /* TROVALIBRE_ESP32 */


// ####### some helper fuctions #####################################

template <typename T> void PROGMEM_read(const T * sce, T& dest)
{
  memcpy_P (&dest, sce, sizeof (T));
}



#endif /* SRC_CONSTANTS_HPP_ */
