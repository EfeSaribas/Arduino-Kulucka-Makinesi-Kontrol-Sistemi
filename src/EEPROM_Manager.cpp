#include "EEPROM_Manager.h"
#include "Globals.h"
#include "Configuration.h"
#include <EEPROM.h>

void loadEEPROMSettings() {
  // EEPROM'a default gün kaydediliyor
  if (EEPROM.read(EEPROM_GUN_SAYISI) == 255) {
    gunSayisi = 0;
    makineAktif = false;
  } else {
    gunSayisi = EEPROM.read(EEPROM_GUN_SAYISI);
    bildirim = 2;
    makineAktif = true;
  }
  
  // Hedef Sıcaklık
  if (EEPROM.read(EEPROM_HEDEF_SICAKLIK) == 255 || 
      EEPROM.read(EEPROM_HEDEF_SICAKLIK + 1) == 255) {
    eepromFloatDiziYaz(EEPROM_HEDEF_SICAKLIK, hedefSicaklik, 4);
  } else {
    eepromFloatDiziOku(EEPROM_HEDEF_SICAKLIK, hedefSicaklik, 4);
  }
  
  // Hedef Nem
  if (EEPROM.read(EEPROM_HEDEF_NEM) == 255 || 
      EEPROM.read(EEPROM_HEDEF_NEM + 1) == 255) {
    eepromFloatDiziYaz(EEPROM_HEDEF_NEM, hedefNem, 4);
  } else {
    eepromFloatDiziOku(EEPROM_HEDEF_NEM, hedefNem, 4);
  }
  
  // Sıcaklık Tolerans
  if (EEPROM.read(EEPROM_SICAKLIK_TOLERANS) == 255 || 
      EEPROM.read(EEPROM_SICAKLIK_TOLERANS + 1) == 255) {
    eepromFloatDiziYaz(EEPROM_SICAKLIK_TOLERANS, sicaklikTolerans, 4);
  } else {
    eepromFloatDiziOku(EEPROM_SICAKLIK_TOLERANS, sicaklikTolerans, 4);
  }
  
  // Nem Tolerans
  if (EEPROM.read(EEPROM_NEM_TOLERANS) == 255 || 
      EEPROM.read(EEPROM_NEM_TOLERANS + 1) == 255) {
    eepromFloatDiziYaz(EEPROM_NEM_TOLERANS, nemTolerans, 4);
  } else {
    eepromFloatDiziOku(EEPROM_NEM_TOLERANS, nemTolerans, 4);
  }
}

void eepromFloatDiziYaz(int adres, float* dizi, int uzunluk) {
  byte* ptr = (byte*)dizi;
  for (int i = 0; i < uzunluk * sizeof(float); i++) {
    EEPROM.write(adres + i, ptr[i]);
  }
}

void eepromFloatDiziOku(int adres, float* dizi, int uzunluk) {
  byte* ptr = (byte*)dizi;
  for (int i = 0; i < uzunluk * sizeof(float); i++) {
    ptr[i] = EEPROM.read(adres + i);
  }
}

void gecenGunSayisi(uint8_t gun) {
  EEPROM.update(EEPROM_GUN_SAYISI, gun);
}

void baslangicTarih(uint8_t gun, uint8_t ay, uint8_t saat, 
                    uint8_t dakika, uint8_t saniye, uint8_t yil) {
  EEPROM.update(EEPROM_BASLANGIC_GUN, gun);
  EEPROM.update(EEPROM_BASLANGIC_AY, ay);
  EEPROM.update(EEPROM_BASLANGIC_SAAT, saat);
  EEPROM.update(EEPROM_BASLANGIC_DAKIKA, dakika);
  EEPROM.update(EEPROM_BASLANGIC_SANIYE, saniye);
  EEPROM.update(EEPROM_BASLANGIC_YIL, yil);
}

void eepromGuncelleAyri() {
  static unsigned long sonEEPROMGuncelleme = 0;
  
  // Her 5 dakikada bir ve sadece encoder aktif değilken
  if (millis() - sonEEPROMGuncelleme >= EEPROM_GUNCELLEME_SURESI &&
      (ekranDurumu == 1 || !etkilesim)) {
    
    if (EEPROM.read(EEPROM_GUN_SAYISI) != gunSayisi) {
      EEPROM.update(EEPROM_GUN_SAYISI, gunSayisi);
      sonEEPROMGuncelleme = millis();
    }
  }
}

void viyolZamaniYaz(uint8_t saat, uint8_t dakika) {
  EEPROM.update(EEPROM_VIYOL_SAAT, saat);
  EEPROM.update(EEPROM_VIYOL_DAKIKA, dakika);
}

void viyolZamaniOku(uint8_t& saat, uint8_t& dakika) {
  saat = EEPROM.read(EEPROM_VIYOL_SAAT);
  dakika = EEPROM.read(EEPROM_VIYOL_DAKIKA);
}