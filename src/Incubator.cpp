#include "Incubator.h"
#include "Globals.h"
#include "Pins.h"
#include "Configuration.h"
#include "EEPROM_Manager.h"
#include <EEPROM.h>

void kuluckaSureci() {
  gunHesapla();
  viyolKontrol();
  
  uint8_t gun;
  
  // Gün aralığına göre parametreleri belirle
  if (gunSayisi >= 1 && gunSayisi <= 7) {
    gun = 0;
  } else if (gunSayisi >= 8 && gunSayisi <= 14) {
    gun = 1;
  } else if (gunSayisi >= 15 && gunSayisi <= 18) {
    gun = 2;
  } else if (gunSayisi >= 19) {
    gun = 3;
  } else {
    gun = 0;
  }
  
  // Fan hızını ayarla
  if (gunSayisi >= 1 && gunSayisi <= 18) {
    fanRPM = FAN_HIZLI_RPM;
  } else {
    fanRPM = FAN_YAVAS_RPM;
  }
  
  kapakDurumuKontrol();
  
  if (bitRead(flags, FLAG_KAPAK_DURUMU)) {  // Kapak kapalı
    bitSet(flags, FLAG_SIRKULASYON_FAN);
    sicaklikKontrol(gun);
    nemKontrol(gun);
  } else {  // Kapak açık - GÜVENLİK
    // Tüm ısıtma/soğutma sistemlerini kapat
    bitClear(flags, FLAG_ISITICI);
    bitClear(flags, FLAG_NEMLENDIRICI);
    bitClear(flags, FLAG_SIRKULASYON_FAN);
    bitClear(flags, FLAG_EGZOZ_FAN);
  }
}

void gunHesapla() {
  static unsigned long sonGunKontrolZamani = 0;
  static uint8_t oncekiGunSayisi = 255;
  
  unsigned long simdi = millis();
  if (simdi - sonGunKontrolZamani < 30000) return;
  
  sonGunKontrolZamani = simdi;
  
  // EEPROM'dan başlangıç tarih ve saati oku
  uint8_t startGun    = EEPROM.read(EEPROM_BASLANGIC_GUN);
  uint8_t startAy     = EEPROM.read(EEPROM_BASLANGIC_AY);
  uint8_t startYil    = EEPROM.read(EEPROM_BASLANGIC_YIL);
  uint8_t startSaat   = EEPROM.read(EEPROM_BASLANGIC_SAAT);
  uint8_t startDakika = EEPROM.read(EEPROM_BASLANGIC_DAKIKA);
  uint8_t startSaniye = EEPROM.read(EEPROM_BASLANGIC_SANIYE);
  
  // Başlangıç tarihini oluştur
  DateTime startDate(2000 + startYil, startAy, startGun, startSaat, startDakika, startSaniye);
  
  // Gün farkını hesapla
  float daysPassed = (now.unixtime() - startDate.unixtime()) / 86400.0;
  uint8_t dayInt = (uint8_t)daysPassed + 1;
  
  if (dayInt != oncekiGunSayisi) {
    gunSayisi = dayInt;
    oncekiGunSayisi = dayInt;
    
    // EEPROM yazımını seyrekleştir
    static unsigned long sonEEPROMYazimi = 0;
    if (simdi - sonEEPROMYazimi >= 60000) {
      if (ekranDurumu == 1 || !etkilesim) {
        EEPROM.update(EEPROM_GUN_SAYISI, dayInt);
        sonEEPROMYazimi = simdi;
      }
    }
  }
}

void viyolKontrol() {
  static unsigned long viyolSuresi = 0;
  static bool viyolCevirildi = false;
  static bool ilkKontrol = true;
  
  if (gunSayisi >= 1 && gunSayisi <= 18) {
    // EEPROM'dan viyol zamanını oku veya ayarla
    if (EEPROM.read(EEPROM_VIYOL_SAAT) == 255 || EEPROM.read(EEPROM_VIYOL_DAKIKA) == 255) {
      viyolSaat   = now.hour() + 4;
      viyolDakika = now.minute();
      if (viyolSaat > 23) viyolSaat -= 24;
      viyolZamaniYaz(viyolSaat, viyolDakika);
      ilkKontrol = false;
    } else {
      viyolZamaniOku(viyolSaat, viyolDakika);
      
      // İlk kontrolde geçmiş zamanları yakala
      if (ilkKontrol) {
        int nowMinutes = now.hour() * 60 + now.minute();
        int scheduledMinutes = viyolSaat * 60 + viyolDakika;
        
        if (nowMinutes > scheduledMinutes) {
          int fark = nowMinutes - scheduledMinutes;
          int kacCevrimGecti = (fark / VIYOL_PERIYOT) + 1;
          int yeniMinutes = scheduledMinutes + (kacCevrimGecti * VIYOL_PERIYOT);
          
          while (yeniMinutes >= 1440) {
            yeniMinutes -= 1440;
          }
          
          viyolSaat = yeniMinutes / 60;
          viyolDakika = yeniMinutes % 60;
          viyolZamaniYaz(viyolSaat, viyolDakika);
        }
        ilkKontrol = false;
      }
    }
    
    // 4 saatte bir çevirme
    if (!viyolCevirildi) {
      int nowMinutes = now.hour() * 60 + now.minute();
      int scheduledMinutes = viyolSaat * 60 + viyolDakika;
      
      if (nowMinutes >= scheduledMinutes && nowMinutes < scheduledMinutes + VIYOL_MIN_ZAMAN_FARKI) {
        bitSet(flags, FLAG_VIYOL);
        viyolSuresi = millis();
        viyolCevirildi = true;
        
        // Sonraki çevirme zamanı
        viyolSaat = now.hour() + 4;
        viyolDakika = now.minute();
        if (viyolSaat > 23) viyolSaat -= 24;
        viyolZamaniYaz(viyolSaat, viyolDakika);
      }
    }
    
    // 12 saniye sonra motoru kapat
    if (viyolCevirildi && millis() - viyolSuresi >= VIYOL_CALISMA_SURESI) {
      bitClear(flags, FLAG_VIYOL);
      viyolCevirildi = false;
    }
  } else {
    ilkKontrol = true;
  }
}

void sicaklikKontrol(uint8_t gun) {
  static unsigned long isiticiSuresi = 0;
  
  // İç sıcaklığı toleransla dengeleme
  if (santigrat < hedefSicaklik[gun] - sicaklikTolerans[gun] + 0.05 &&
      !bitRead(flags, FLAG_ISITICI) &&
      millis() - isiticiSuresi >= 20000) {
    bitSet(flags, FLAG_ISITICI);
    isiticiSuresi = millis();
  }
  
  if (bitRead(flags, FLAG_ISITICI) &&
      ((santigrat >= hedefSicaklik[gun]) ||
       (millis() - isiticiSuresi >= 30000))) {
    bitClear(flags, FLAG_ISITICI);
    isiticiSuresi = millis();
  }
}

void nemKontrol(uint8_t gun) {
  static unsigned long nemlendiriciSuresi = 0;
  
  // İç nemi toleransla dengeleme
  if (nem < hedefNem[gun] &&
      !bitRead(flags, FLAG_NEMLENDIRICI) &&
      millis() - nemlendiriciSuresi >= 10000) {
    bitSet(flags, FLAG_NEMLENDIRICI);
    nemlendiriciSuresi = millis();
  }
  
  if (bitRead(flags, FLAG_NEMLENDIRICI) &&
      ((nem >= hedefNem[gun]) ||
       (millis() - nemlendiriciSuresi >= 20000))) {
    bitClear(flags, FLAG_NEMLENDIRICI);
    nemlendiriciSuresi = millis();
  }
  
  // Egzoz fanı kontrolü
  if (nem >= hedefNem[gun] + nemTolerans[gun] && !bitRead(flags, FLAG_EGZOZ_FAN)) {
    bitSet(flags, FLAG_EGZOZ_FAN);
  }
  if (nem < hedefNem[gun] + nemTolerans[gun] && bitRead(flags, FLAG_EGZOZ_FAN)) {
    bitClear(flags, FLAG_EGZOZ_FAN);
  }
}

void kapakDurumuKontrol() {
  if (!bitRead(flags, FLAG_KAPAK_DURUMU) && makineAktif) {
    // Kapak açık - Tüm sistemleri kapat
    bitClear(flags, FLAG_ISITICI);
    bitClear(flags, FLAG_NEMLENDIRICI);
    bitClear(flags, FLAG_SIRKULASYON_FAN);
    bitClear(flags, FLAG_EGZOZ_FAN);
  }
}

void ortamDuzenliyicileri() {
  if (guvenlikModu) {
    digitalWrite(rezistans, LOW);
    digitalWrite(nemlendirici, HIGH);
    digitalWrite(viyol, LOW);
    analogWrite(sirkulasyonFan, 0);
    digitalWrite(egzozFan, LOW);
    return;
  }
  
  // Normal çalışma - ekstra güvenlik kontrolleri ile
  digitalWrite(rezistans, (bitRead(flags, FLAG_ISITICI) && santigrat < 44.0) ? HIGH : LOW);
  digitalWrite(nemlendirici, bitRead(flags, FLAG_NEMLENDIRICI) ? LOW : HIGH);
  digitalWrite(viyol, bitRead(flags, FLAG_VIYOL) ? HIGH : LOW);
  analogWrite(sirkulasyonFan, bitRead(flags, FLAG_SIRKULASYON_FAN) ? fanRPM : 0);
  digitalWrite(egzozFan, bitRead(flags, FLAG_EGZOZ_FAN) ? HIGH : LOW);
}