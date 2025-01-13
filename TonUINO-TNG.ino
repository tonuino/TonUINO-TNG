#include "src/tonuino.hpp"

#include "src/settings.hpp"
#include "src/mp3.hpp"
#include "src/buttons.hpp"
#include "src/logger.hpp"
#include "src/constants.hpp"

/*
   _____         _____ _____ _____ _____
  |_   _|___ ___|  |  |     |   | |     |
    | | | . |   |  |  |-   -| | | |  |  |
    |_| |___|_|_|_____|_____|_|___|_____|
    TonUINO Version 3.x - refactored by Boerge1

    created by Thorsten Voß and licensed under GNU/GPL.
    Information and contribution at https://tonuino.de.
*/

/*
Please select your TonUINO PCB (Classic, AiO or AiO+) in file constants.hpp

Bitte wählt eure TonUINO-Platine (Classic, AiO oder AiO+) in der Datei constants.hpp
*/

void setup()
{
  Serial.begin(115200);

  // Dieser Hinweis darf nicht entfernt werden
  LOG(init_log, s_error, F("\n _____         _____ _____ _____ _____"));
  LOG(init_log, s_error, F("|_   _|___ ___|  |  |     |   | |     |"));
  LOG(init_log, s_error, F("  | | | . |   |  |  |-   -| | | |  |  |"));
  LOG(init_log, s_error, F("  |_| |___|_|_|_____|_____|_|___|_____|"));
  LOG(init_log, s_error, F("TonUINO Version 3.x - refactored by Boerge1\n"));
  LOG(init_log, s_error, F("created by Thorsten Voß and licensed under GNU/GPL."));
  LOG(init_log, s_error, F("Information and contribution at https://tonuino.de.\n"));
  LOG(init_log, s_error, F("V3.2.1 13.01.25\n"));

#ifdef TonUINO_Classic
  LOG(init_log, s_error, F("C "), lf_no);
#endif
#ifdef TonUINO_Every
  LOG(init_log, s_error, F("E "), lf_no);
#endif
#ifdef TonUINO_Every_4808
  LOG(init_log, s_error, F("EC "), lf_no);
#endif
#ifdef ALLinONE
  LOG(init_log, s_error, F("A "), lf_no);
#endif
#ifdef ALLinONE_Plus
  LOG(init_log, s_error, F("A+ "), lf_no);
#endif

#ifdef FIVEBUTTONS
  LOG(init_log, s_error, F("5"));
#elif  BUTTONS3X3
  LOG(init_log, s_error, F("3x3"));
#else
  LOG(init_log, s_error, F("3"));
#endif


  Tonuino::getTonuino().setup();
}

void loop()
{
  Tonuino::getTonuino().loop();
}
