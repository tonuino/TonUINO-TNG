#include "batVoltage.hpp"

#include "constants.hpp"

#ifdef BAT_VOLTAGE_MEASUREMENT
#include "logger.hpp"
#include "mp3.hpp"

namespace {

#ifndef TonUINO_Classic
inline constexpr uint16_t voltageMeasurementRefVoltage = 2500; // reference voltage mV
#endif
inline constexpr int16_t voltageMeasurementMaxLevel    = 1023;
inline constexpr int16_t voltageMeasurementLowLevel    = batVoltageLow  /voltageMeasurementCorrection*voltageMeasurementMaxLevel;
inline constexpr int16_t voltageMeasurementEmptyLevel  = batVoltageEmpty/voltageMeasurementCorrection*voltageMeasurementMaxLevel;
inline constexpr unsigned long batLowMessageIntervall  = 30*1000; // 30 seconds
inline constexpr unsigned long batEmptyTimer           = 10*1000; // 10 seconds

#ifdef TonUINO_Classic
long readVcc() {
  long result;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  result = 1125300L / result; // Back-calculate AVcc in mV
  return result;
}
#endif
}

BatVoltage::BatVoltage(Mp3& mp3)
: mp3(mp3)
{
  pinMode(voltageMeasurementPin, INPUT);
  logTimer.start(2000);
}

bool BatVoltage::check() {
#ifdef TonUINO_Classic
  const uint16_t voltageMeasurementRefVoltage = readVcc();
#endif
  const int16_t value = analogRead(voltageMeasurementPin)*static_cast<long>(voltageMeasurementRefVoltage)/1000;

  if (logTimer.isExpired()) {
    logTimer.start(2000);
    voltage = value*voltageMeasurementCorrection/voltageMeasurementMaxLevel;
    LOG(batvol_log, s_debug, F("BatVoltage: "), voltage);
  }

  if (value < voltageMeasurementEmptyLevel) {
    if (emptyTimer.isActive()) {
      if (emptyTimer.isExpired()) {
        LOG(batvol_log, s_error, F("BatVoltage empty"));
        return true;
      }
    }
    else {
      emptyTimer.start(batEmptyTimer);
    }
  }
  else {
    emptyTimer.stop();
  }

  if (value < voltageMeasurementLowLevel) {
    if (lowTimer.isActive()) {
      if (lowTimer.isExpired()) {
        low = true;
        lowMessage();
      }
    }
    else {
      lowTimer.start(batLowMessageIntervall);
    }
  }
  else {
    lowTimer.stop();
    low = false;
  }

  return false;
}

void BatVoltage::lowMessage() {
  mp3.playAdvertisement(advertTracks::t_262_pling, false /*olnyIfIsPlaying*/);
}

#endif // BAT_VOLTAGE_MEASUREMENT
