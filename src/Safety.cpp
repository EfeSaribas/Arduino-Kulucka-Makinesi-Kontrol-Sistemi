#include "Safety.h"
#include "Globals.h"
#include "Pins.h"
#include "LCD.h"
#include "Configuration.h"
#include <avr/wdt.h>

void guvenlikKontrolleri() {
  unsigned long simdi = millis();
  
  // Her 10 saniyede güvenlik kontrolü
  if (simdi - sonGuvenlikKontrolu >= GUVENLIK_KONTROL_SURESI) {
    
    // 1. Aşırı sıcaklık kontrolü
    if (asiriSicaklikKontrol()) {
      acilDurdurma(F("ASIRI SICAKLIK!"));
      return;
    }
    
    // 2. Sensör hata kontrolü
    if (sensorHataKontrol()) {
      acilDurdurma(F("SENSOR HATASI!"));
      return;
    }
    
    // 3. Isıtıcı maksimum çalışma süresi kontrolü
    isiticiSureKontrol();
    
    // 4. Kapak sensörü doğrulama
    kapakSensorDogrulama();
    
    sonGuvenlikKontrolu = simdi;
  }
}

bool asiriSicaklikKontrol() {
  if (santigrat > KRITIK_SICAKLIK) {
    return true;
  }
  return false;
}

bool sensorHataKontrol() {
  if (isnan(santigrat) || 
      santigrat < MIN_GECERLI_SICAKLIK || 
      santigrat > MAX_GECERLI_SICAKLIK) {
    sistemHataSayaci++;
    if (sistemHataSayaci >= SENSOR_HATA_LIMITI) {
      return true;
    }
  } else {
    sistemHataSayaci = 0;
  }
  return false;
}

void isiticiSureKontrol() {
  unsigned long simdi = millis();
  
  if (bitRead(flags, FLAG_ISITICI)) {
    if (isiticiBaslangicZamani == 0) {
      isiticiBaslangicZamani = simdi;
    }
    else if (simdi - isiticiBaslangicZamani >= ISITICI_MAX_SURE) {
      bitClear(flags, FLAG_ISITICI);  // Isıtıcıyı zorla kapat
      isiticiBaslangicZamani = 0;
    }
  } else {
    isiticiBaslangicZamani = 0;
  }
}

void kapakSensorDogrulama() {
  static uint8_t kapakOkumaSayaci = 0;
  int kapakDegeri = analogRead(kapakSensor);
  
  if (kapakDegeri < 200) {
    kapakOkumaSayaci++;
    if (kapakOkumaSayaci >= KAPAK_OKUMA_DOGRULAMA) {
      bitSet(flags, FLAG_KAPAK_DURUMU);
      kapakOkumaSayaci = 0;
    }
  } else {
    kapakOkumaSayaci = 0;
    if (makineAktif) {
      bitClear(flags, FLAG_KAPAK_DURUMU);
      bildirim = 3;
      // Güvenlik için sistemleri kapat
      bitClear(flags, FLAG_ISITICI);
      bitClear(flags, FLAG_NEMLENDIRICI);
    }
  }
}

void acilDurdurma(String sebep) {
  // Tüm sistemleri kapat
  bitClear(flags, FLAG_ISITICI);
  bitClear(flags, FLAG_NEMLENDIRICI);
  bitClear(flags, FLAG_VIYOL);
  bitClear(flags, FLAG_SIRKULASYON_FAN);
  bitClear(flags, FLAG_EGZOZ_FAN);
  
  digitalWrite(rezistans, LOW);
  digitalWrite(nemlendirici, HIGH);
  digitalWrite(viyol, LOW);
  digitalWrite(sirkulasyonFan, LOW);
  digitalWrite(egzozFan, LOW);
  
  makineAktif = false;
  guvenlikModu = true;
  
  lcd.clear();
  guvenlilcdYaz(7, 0, F("ACIL DURDURMA!"));
  guvenlilcdYaz(7, 1, F("Sistem Guvenli"));
  guvenlilcdYaz(0, 3, sebep);
  
  // 8 saniye sonra reset
  wdt_enable(WDTO_8S);
  while (1) {}  // WDT tetiklenecek ve Arduino reset atacak
}

void resetWatchdog() {
  wdt_reset();
}