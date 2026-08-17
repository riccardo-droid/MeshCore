#include <Arduino.h>
#include <Wire.h>

#include "TechoBoard.h"

#ifdef LILYGO_TECHO

void TechoBoard::begin() {
  NRF52Board::begin();

  Wire.begin();

  pinMode(SX126X_POWER_EN, OUTPUT);
  digitalWrite(SX126X_POWER_EN, HIGH);
  delay(10);   // give sx1262 some time to power up

  // Inizializza la retroilluminazione (DISP_BACKLIGHT = pin 43)
  #ifdef DISP_BACKLIGHT
  pinMode(DISP_BACKLIGHT, OUTPUT);
  digitalWrite(DISP_BACKLIGHT, LOW); // Spenta di default per risparmiare energia
  #endif

  // Inizializza il pulsante utente (PIN_BUTTON1 = pin 42)
  #ifdef PIN_BUTTON1
  pinMode(PIN_BUTTON1, INPUT_PULLUP);
  #endif
}

void TechoBoard::update() {
  #if defined(DISP_BACKLIGHT) && defined(PIN_BUTTON1)
  bool btnState = digitalRead(PIN_BUTTON1);

  // Rileva la pressione del pulsante (transizione da HIGH a LOW)
  if (lastBtnState == HIGH && btnState == LOW) {
    digitalWrite(DISP_BACKLIGHT, HIGH);   // Accende la retroilluminazione (BL)
    backlightOffTime = millis() + 5000;   // Imposta lo spegnimento a 5 secondi
  }
  lastBtnState = btnState;

  // Spegnimento automatico allo scadere del timer
  if (backlightOffTime > 0 && millis() >= backlightOffTime) {
    digitalWrite(DISP_BACKLIGHT, LOW);    // Spegne la retroilluminazione
    backlightOffTime = 0;                 // Azzera il timer
  }
  #endif
}

uint16_t TechoBoard::getBattMilliVolts() {
  int adcvalue = 0;

  analogReference(AR_INTERNAL_3_0);
  analogReadResolution(12);
  delay(10);

  // ADC range is 0..3000mV and resolution is 12-bit (0..4095)
  adcvalue = analogRead(PIN_VBAT_READ);
  // Convert the raw value to compensated mv, taking the resistor-
  // divider into account (providing the actual LIPO voltage)
  return (uint16_t)((float)adcvalue * REAL_VBAT_MV_PER_LSB);
}
#endif