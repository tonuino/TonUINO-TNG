#include "src/trovalibre.hpp"

#include "src/settings.hpp"
#include "src/mp3.hpp"
#include "src/buttons.hpp"
#include "src/logger.hpp"
#include "src/constants.hpp"
#include "src/version.hpp"

/*
   _____                    _     _ _
  |_   _| __ _____   ____ _| |   (_) |__  _ __ ___
    | || '__/ _ \ \ / / _` | |   | | '_ \| '__/ _ \
    | || | | (_) \ V / (_| | |___| | |_) | | |  __/
    |_||_|  \___/ \_/ \__,_|_____|_|_.__/|_|  \___|
    TrovaLibre Version 3.x

    TrovaLibre is based on TonUINO / TonUINO-TNG.
    Original created by Thorsten Voß and refactored by Boerge1, licensed under GNU/GPL.
    Information and contribution at https://tonuino.de.
*/

// EN: Select your TrovaLibre board (Classic, AiO or AiO+) in constants.hpp.
// ES: Selecciona tu placa TrovaLibre (Classic, AiO o AiO+) en constants.hpp.

void setup()
{
  Serial.begin(115200);
#ifdef TROVALIBRE_ESP32
  if (pin_is_active(buttonDownPin, buttonPinType))
    delay(5000);
#endif

  // EN: Keep this attribution notice to preserve project credits.
  // ES: Mantener este aviso de atribucion para preservar los creditos del proyecto.
  LOG(init_log, s_error, F(""));
  LOG(init_log, s_error, F(" _____         _____ _____ _____ _____ "));
  LOG(init_log, s_error, F("|_   _|___ ___|  |  |     |   | |     |"));
  LOG(init_log, s_error, F("  | | | . |   |  |  |-   -| | | |  |  |"));
  LOG(init_log, s_error, F("  |_| |___|_|_|_____|_____|_|___|_____|"));
  LOG(init_log, s_error, F("TrovaLibre Version 3.x"));
  LOG(init_log, s_error, F(""));
  LOG(init_log, s_error, F("TrovaLibre is based on TonUINO / TonUINO-TNG."));
  LOG(init_log, s_error, F("created by Thorsten Voß and refactored by Boerge1, licensed under GNU/GPL."));
  LOG(init_log, s_error, F("Information and contribution at https://tonuino.de."));
  LOG(init_log, s_error, F(""));
  LOG(init_log, s_error, TROVALIBRE_VERSION());

#ifdef TROVALIBRE_CLASSIC
  LOG(init_log, s_error, F("C "), lf_no);
#endif
#ifdef TROVALIBRE_EVERY
  LOG(init_log, s_error, F("E "), lf_no);
#endif
#ifdef TROVALIBRE_EVERY_4808
  LOG(init_log, s_error, F("EC "), lf_no);
#endif
#ifdef ALLinONE
  LOG(init_log, s_error, F("A "), lf_no);
#endif
#ifdef ALLinONE_Plus
  LOG(init_log, s_error, F("A+ "), lf_no);
#endif
#if TROVALIBRE_ESP32 == 100
  LOG(init_log, s_error, F("ESP32 Nano "), lf_no);
#endif
#if TROVALIBRE_ESP32 == 200
  LOG(init_log, s_error, F("ESP32 Wroom 32 "), lf_no);
#endif

#ifdef FIVEBUTTONS
  LOG(init_log, s_error, F("5"));
#elif  BUTTONS3X3
  LOG(init_log, s_error, F("3x3"));
#else
  LOG(init_log, s_error, F("3"));
#endif


  TrovaLibre::getInstance().setup();
}

void loop()
{
  TrovaLibre::getInstance().loop();
}
