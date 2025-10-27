#include "SerialDebug.h"
#include "Globals.h"
#include "Configuration.h"
#include <EEPROM.h>

void setupSerial() {
  Serial.begin(BAUD_RATE);
}

void serialMonitor() {
  Serial.print(F("Makine Durumu : "));
  Serial.print(makineAktif ? F("Aktif") : F("Pasif"));
  Serial.print(F(" | "));
  
  printSensorValues();
  printFlagStatus();
  printIncubatorInfo();
  printDateTime();
}

void printSensorValues() {
  Serial.print(F("Sıcaklık : "));
  Serial.print(santigrat);
  Serial.print(F("°C"));
  Serial.print(F(" | "));
  
  Serial.print(F("Nem : "));
  Serial.print(nem);
  Serial.print(F("%"));
  Serial.print(F(" | "));
  
  Serial.print(F("Gün : "));
  Serial.print(gunSayisi);
  Serial.print(F(" | "));
}

void printFlagStatus() {
  Serial.print(F("Rezistans : "));
  Serial.print(bitRead(flags, FLAG_ISITICI) ? F("Açık") : F("Kapalı"));
  Serial.print(F(" | "));
  
  Serial.print(F("Nemlendirici : "));
  Serial.print(bitRead(flags, FLAG_NEMLENDIRICI) ? F("Açık") : F("Kapalı"));
  Serial.print(F(" | "));
  
  Serial.print(F("Viyol : "));
  Serial.print(bitRead(flags, FLAG_VIYOL) ? F("Açık") : F("Kapalı"));
  Serial.print(F(" | "));
  
  Serial.print(F("S. Fanı : "));
  Serial.print(bitRead(flags, FLAG_SIRKULASYON_FAN) ? F("Açık") : F("Kapalı"));
  Serial.print(F(" | "));
  
  if (bitRead(flags, FLAG_SIRKULASYON_FAN)) {
    Serial.print(F("S. Fanı RPM : "));
    Serial.print(fanRPM);
    Serial.print(F(" | "));
  }
  
  Serial.print(F("Egzoz Fanı : "));
  Serial.print(bitRead(flags, FLAG_EGZOZ_FAN) ? F("Açık") : F("Kapalı"));
  Serial.print(F(" | "));
  
  Serial.print(F("Kapak : "));
  Serial.print(bitRead(flags, FLAG_KAPAK_DURUMU) ? F("Kapalı") : F("Açık"));
  Serial.print(F(" | "));
}

void printIncubatorInfo() {
  if (makineAktif) {
    Serial.print(F("Viyol Saati : "));
    Serial.print(viyolSaat);
    Serial.print(F(":"));
    Serial.print(viyolDakika);
    Serial.print(F(" | "));
    
    Serial.print(F("Başlangıç Tarihi : "));
    Serial.print(EEPROM.read(EEPROM_BASLANGIC_GUN));
    Serial.print(F("."));
    Serial.print(EEPROM.read(EEPROM_BASLANGIC_AY));
    Serial.print(F("."));
    Serial.print(EEPROM.read(EEPROM_BASLANGIC_YIL));
    Serial.print(F(" "));
    Serial.print(EEPROM.read(EEPROM_BASLANGIC_SAAT));
    Serial.print(F(":"));
    Serial.print(EEPROM.read(EEPROM_BASLANGIC_DAKIKA));
    Serial.print(F(":"));
    Serial.print(EEPROM.read(EEPROM_BASLANGIC_SANIYE));
    Serial.print(F(" | "));
  }
}

void printDateTime() {
  Serial.print(F("Saat / Tarih : "));
  Serial.print(now.day());
  Serial.print(F("."));
  Serial.print(now.month());
  Serial.print(F("."));
  Serial.print(now.year());
  Serial.print(F(" "));
  Serial.print(now.hour());
  Serial.print(F(":"));
  Serial.println(now.minute());
}