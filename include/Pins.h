#ifndef PINS_H
#define PINS_H

#include "Configuration.h"

// ===========================================================================
// ======================== Pin İsimleri =====================================
// ===========================================================================

// Giriş Pinleri
extern const uint8_t btn;
extern const uint8_t dt;
extern const uint8_t clk;
extern const uint8_t kapakSensor;

// Çıkış Pinleri
extern const uint8_t sirkulasyonFan;
extern const uint8_t viyol;
extern const uint8_t nemSensorYenidenBaslatma;
extern const uint8_t egzozFan;
extern const uint8_t nemSensor;
extern const uint8_t sicaklikSensor;
extern const uint8_t rezistans;
extern const uint8_t nemlendirici;

// Pin başlatma fonksiyonu
void setupPins();

#endif // PINS_H