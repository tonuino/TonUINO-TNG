#include "src/tonuino.hpp"

#include "src/settings.hpp"
#include "src/mp3.hpp"
#include "src/buttons.hpp"
#include "src/logger.hpp"
#include "src/constants.hpp"

// Select the right PCB by uncommenting one of the following lines
// Bitte die passende Platine durch entfernen der Kommentare in einer der folgenden Zeilen auswählen
//#define TonUINO_Classic
//#define ALLinONE
//#define ALLinONE_Plus

// uncomment the below line to enable five button support (already enabled for AiO and AiO+)
// um dedizierte Lauter-/Leiserknöpfe zu haben bitte die nächste Zeile auskommentieren (Standard bei AiO und AiO+)
//#define FIVEBUTTONS

/*
   _____         _____ _____ _____ _____
  |_   _|___ ___|  |  |     |   | |     |
    | | | . |   |  |  |-   -| | | |  |  |
    |_| |___|_|_|_____|_____|_|___|_____|
    TonUINO Version 3.0 - refactored by Boerge1

    created by Thorsten Voß and licensed under GNU/GPL.
    Information and contribution at https://tonuino.de.
*/

void setup()
{
  Serial.begin(115200);

  // Dieser Hinweis darf nicht entfernt werden
  LOG(init_log, s_error, F("\n _____         _____ _____ _____ _____"));
  LOG(init_log, s_error, F("|_   _|___ ___|  |  |     |   | |     |"));
  LOG(init_log, s_error, F("  | | | . |   |  |  |-   -| | | |  |  |"));
  LOG(init_log, s_error, F("  |_| |___|_|_|_____|_____|_|___|_____|"));
  LOG(init_log, s_error, F("TonUINO Version 3.0 - refactored by Boerge1\n"));
  LOG(init_log, s_error, F("created by Thorsten Voß and licensed under GNU/GPL."));
  LOG(init_log, s_error, F("Information and contribution at https://tonuino.de.\n"));

  Tonuino::getTonuino().setup();
}

void loop()
{
  Tonuino::getTonuino().loop();
}
