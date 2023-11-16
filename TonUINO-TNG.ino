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
    TonUINO Version 3.1 - refactored by Boerge1

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
  LOG(init_log, s_error, F("TonUINO Version 3.1 - refactored by Boerge1\n"));
  LOG(init_log, s_error, F("created by Thorsten Voß and licensed under GNU/GPL."));
  LOG(init_log, s_error, F("Information and contribution at https://tonuino.de.\n"));
  LOG(init_log, s_error, F("V3.1.4 16.11.23\n"));

  Tonuino::getTonuino().setup();
}

void loop()
{
  Tonuino::getTonuino().loop();
}
