#include "Pins.h"

// ===========================================================================
// ======================== Pin Tanımlamaları ================================
// ===========================================================================

// Giriş Pinleri
const uint8_t btn = PIN_BTN;
const uint8_t dt = PIN_DT;
const uint8_t clk = PIN_CLK;
const uint8_t kapakSensor = PIN_KAPAK_SENSOR;

// Çıkış Pinleri
const uint8_t buzzer = PIN_BUZZER;
const uint8_t sirkulasyonFan = PIN_SIRKULASYON_FAN;
const uint8_t viyol = PIN_VIYOL;
const uint8_t nemSensorYenidenBaslatma = PIN_NEM_RESET;
const uint8_t egzozFan = PIN_EGZOZ_FAN;
const uint8_t nemSensor = PIN_NEM_SENSOR;
const uint8_t sicaklikSensor = PIN_SICAKLIK_SENSOR;
const uint8_t rezistans = PIN_REZISTANS;
const uint8_t nemlendirici = PIN_NEMLENDIRICI;

// ===========================================================================
// ======================== Pin Başlatma =====================================
// ===========================================================================

void setupPins() {
  // Giriş pinleri
  pinMode(clk, INPUT_PULLUP);
  pinMode(dt, INPUT_PULLUP);
  pinMode(btn, INPUT_PULLUP);
  pinMode(kapakSensor, INPUT);
  
  // Çıkış pinleri
  pinMode(buzzer, OUTPUT);
  pinMode(egzozFan, OUTPUT);
  pinMode(viyol, OUTPUT);
  pinMode(rezistans, OUTPUT);
  pinMode(nemlendirici, OUTPUT);
  pinMode(sirkulasyonFan, OUTPUT);
  pinMode(nemSensorYenidenBaslatma, OUTPUT);
  
  // Başlangıç durumları
  analogWrite(sirkulasyonFan, LOW);
  digitalWrite(viyol, LOW);
  digitalWrite(buzzer, LOW);
  digitalWrite(egzozFan, LOW);
  digitalWrite(rezistans, LOW);
  digitalWrite(nemlendirici, HIGH);  // Aktif LOW
  digitalWrite(nemSensorYenidenBaslatma, HIGH);
}