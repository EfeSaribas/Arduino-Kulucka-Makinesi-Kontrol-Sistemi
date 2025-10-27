#include "Encoder.h"
#include "Globals.h"
#include "Pins.h"
#include "LCD.h"
#include "RTC.h"
#include "EEPROM_Manager.h"
#include "Configuration.h"
#include <EEPROM.h>

void encoderOku() {
  static int encoderPos = 0;
  static uint8_t lastState = 0;
  static unsigned long sonEncoderZamani = 0;
  
  unsigned long simdi = millis();
  
  // Encoder debounce
  if (simdi - sonEncoderZamani < ENCODER_DEBOUNCE_SURESI) return;
  
  // Mevcut encoder durumu
  uint8_t MSB = digitalRead(clk);
  uint8_t LSB = digitalRead(dt);
  uint8_t currentState = (MSB << 1) | LSB;
  
  // İlk okuma için lastState'i başlat
  static bool ilkOkuma = true;
  if (ilkOkuma) {
    lastState = currentState;
    ilkOkuma = false;
    return;
  }
  
  // Her durumda değişim varsa
  if (currentState != lastState) {
    sonEncoderZamani = simdi;
    
    // Saat yönü (clockwise)
    if ((lastState == 0b00 && currentState == 0b01) ||
        (lastState == 0b01 && currentState == 0b11) ||
        (lastState == 0b11 && currentState == 0b10) ||
        (lastState == 0b10 && currentState == 0b00)) {
      encoderPos++;
    }
    // Saat yönü tersı (counter-clockwise)
    else if ((lastState == 0b00 && currentState == 0b10) ||
             (lastState == 0b10 && currentState == 0b11) ||
             (lastState == 0b11 && currentState == 0b01) ||
             (lastState == 0b01 && currentState == 0b00)) {
      encoderPos--;
    }
    
    // Tam 4 adımda bir sayaç güncellenir
    if (encoderPos >= 4) {
      secimAzalt();
      encoderPos = 0;
      if (etkilesim) {
        sonEtkilesimZamani = millis();
      }
    }
    else if (encoderPos <= -4) {
      secimArttir();
      encoderPos = 0;
      if (etkilesim) {
        sonEtkilesimZamani = millis();
      }
    }
    
    lastState = currentState;
  }
}

void secimArttir() {
  if (ekranDurumu == 2 || ekranDurumu == 4 || ekranDurumu == 5) {
    if (secim < 3) secim++;
    else secim = 0;
  }
  else if (ekranDurumu == 3 && !editModu && !kayitBtn) {
    if (secim < 6) secim++;
    else secim = 0;
  }
  else if (ekranDurumu == 3 && !editModu && kayitBtn) {
    if (secim < 7) secim++;
    else secim = 0;
  }
  else if (ekranDurumu == 3 && editModu) {
    if (secim >= 0 && secim <= 2) {
      if (deger < maxDeger) deger++;
      else deger = 0;
    }
    else if (secim >= 3 && secim <= 5) {
      if (deger < maxDeger) deger++;
      else deger = 1;
    }
  }
  else if ((ekranDurumu == 6 && !kayitBtn && !editModu) || ekranDurumu == 7) {
    if (secim < 1) secim++;
    else secim = 0;
  }
  else if (ekranDurumu == 6 && kayitBtn && !editModu) {
    if (secim < 2) secim++;
    else secim = 0;
  }
  else if (ekranDurumu == 6 && editModu) {
    ayar += 0.1;
  }
}

void secimAzalt() {
  if (ekranDurumu == 2 || ekranDurumu == 4 || ekranDurumu == 5) {
    if (secim > 0) secim--;
    else secim = 3;
  }
  else if (ekranDurumu == 3 && !editModu && !kayitBtn) {
    if (secim > 0) secim--;
    else secim = 6;
  }
  else if (ekranDurumu == 3 && !editModu && kayitBtn) {
    if (secim > 0) secim--;
    else secim = 7;
  }
  else if (ekranDurumu == 3 && editModu) {
    if (secim >= 0 && secim <= 2) {
      if (deger > 0) deger--;
      else deger = maxDeger;
    }
    else if (secim >= 3 && secim <= 5) {
      if (deger > 1) deger--;
      else deger = maxDeger;
    }
  }
  else if ((ekranDurumu == 6 && !kayitBtn && !editModu) || ekranDurumu == 7) {
    if (secim > 0) secim--;
    else secim = 1;
  }
  else if (ekranDurumu == 6 && kayitBtn && !editModu) {
    if (secim > 0) secim--;
    else secim = 2;
  }
  else if (ekranDurumu == 6 && editModu) {
    ayar -= 0.1;
  }
}

void checkTimeout() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - sonEtkilesimZamani >= ZAMAN_ASIMI_SURESI &&
      etkilesim &&
      !zamanAsimiAktif &&
      ekranDurumu != 1) {
    
    ekranTemizle = true;
    etkilesim = false;
    secim = 0;
    ekranDurumu = 1;
    ayarModu = false;
    editModu = false;
    kayitBtn = false;
    zamanAsimiAktif = false;
  }
}

void handleButton() {
  unsigned long currentMillis = millis();
  bool butonAnlikDurum = digitalRead(btn);
  
  if (butonAnlikDurum == LOW && currentMillis - sonBasilma >= BUTON_DEBOUNCE_SURESI) {
    sonBasilma = currentMillis;
    etkilesim = true;
    sonEtkilesimZamani = currentMillis;
    
    // Ana ekrandan menüye geçiş
    if (ekranDurumu == 1) {
      ekranDurumu = 2;
      ekranTemizle = true;
    }
    // Menülerden geri dönüş
    else if ((ekranDurumu == 2 && secim == 3) || 
             (ekranDurumu == 3 && secim == 6) || 
             (ekranDurumu == 6 && secim == 1) || 
             (ekranDurumu == 7 && secim == 0)) {
      ekranDurumu = 1;
      ekranTemizle = true;
      etkilesim = false;
      secim = 0;
      ayarModu = false;
      zamanAsimiAktif = false;
      lcdAnaEkran();
      kayitBtn = false;
    }
    // Saat/Tarih ayar menüsüne geçiş
    else if (ekranDurumu == 2 && secim == 2) {
      ekranDurumu = 3;
      ekranTemizle = true;
      etkilesim = true;
      sonEtkilesimZamani = currentMillis;
      secim = 0;
      lcdSaatAyar();
    }
    // Kuluçka ayar menüsüne geçiş
    else if (ekranDurumu == 2 && secim == 1) {
      ekranDurumu = 4;
      ekranTemizle = true;
      etkilesim = true;
      sonEtkilesimZamani = currentMillis;
      secim = 0;
    }
    // Saat/Tarih ayar edit modu
    else if (ekranDurumu == 3 && secim < 6 && !editModu) {
      ayarModu = true;
      editModu = true;
      zamanAsimiAktif = true;
      switch (secim) {
        case 0: maxDeger = 23; deger = ayarSaat; break;
        case 1: maxDeger = 59; deger = ayarDakika; break;
        case 2: maxDeger = 59; deger = ayarSaniye; break;
        case 3: maxDeger = ayGunSayisi(now.month(), now.year()); deger = ayarGun; break;
        case 4: maxDeger = 12; deger = ayarAy; break;
        case 5: maxDeger = 99; deger = ayarYil; break;
      }
    }
    // Saat/Tarih ayar edit modundan çıkış
    else if (ekranDurumu == 3 && ayarModu && editModu) {
      if (ayarSaat != now.hour() ||
          ayarDakika != now.minute() ||
          ayarSaniye != now.second() ||
          ayarGun != now.day() ||
          ayarAy != now.month() ||
          ayarYil != (now.year() % 100)) {
        kayitBtn = true;
        editModu = false;
      } else {
        ayarModu = false;
        editModu = false;
        kayitBtn = false;
      }
    }
    // Saat/Tarih kaydet
    else if (ekranDurumu == 3 && !editModu && secim == 7 && ayarModu) {
      rtcAyarla();
      bildirim = 1;
      bildirimSuresi = currentMillis;
      ayarModu = false;
      editModu = false;
      kayitBtn = false;
      ekranDurumu = 1;
      ekranTemizle = true;
      etkilesim = false;
      secim = 0;
      zamanAsimiAktif = false;
      lcdAnaEkran();
    }
    // Kuluçka parametresi seçimi
    else if (ekranDurumu == 4) {
      ekranDurumu = 5;
      ayarlananGun = secim;
      ayarModu = false;
      editModu = false;
      kayitBtn = false;
      ekranTemizle = true;
      sonEtkilesimZamani = currentMillis;
      secim = 0;
      lcdAyarMenu();
    }
    // Parametre tipi seçimi
    else if (ekranDurumu == 5) {
      secimAyar = secim;
      switch (secimAyar) {
        case 0: ayar = hedefSicaklik[ayarlananGun]; break;
        case 1: ayar = hedefNem[ayarlananGun]; break;
        case 2: ayar = sicaklikTolerans[ayarlananGun]; break;
        case 3: ayar = nemTolerans[ayarlananGun]; break;
      }
      ekranDurumu = 6;
      ayarModu = false;
      editModu = false;
      kayitBtn = false;
      ekranTemizle = true;
      sonEtkilesimZamani = currentMillis;
      secim = 0;
      lcdAyarMenu2();
    }
    // Parametre düzenleme
    else if (ekranDurumu == 6 && secim == 0) {
      if (editModu) {
        editModu = false;
        zamanAsimiAktif = true;
        switch (secimAyar) {
          case 0: if (ayar != hedefSicaklik[ayarlananGun]) kayitBtn = true; break;
          case 1: if (ayar != hedefNem[ayarlananGun]) kayitBtn = true; break;
          case 2: if (ayar != sicaklikTolerans[ayarlananGun]) kayitBtn = true; break;
          case 3: if (ayar != nemTolerans[ayarlananGun]) kayitBtn = true; break;
        }
      } else {
        editModu = true;
        zamanAsimiAktif = true;
      }
    }
    // Parametre kaydet
    else if (ekranDurumu == 6 && secim == 2) {
      kayitBtn = false;
      editModu = false;
      etkilesim = false;
      secim = 0;
      ekranTemizle = true;
      zamanAsimiAktif = false;
      ekranDurumu = 1;
      switch (secimAyar) {
        case 0: hedefSicaklik[ayarlananGun] = ayar; 
                eepromFloatDiziYaz(EEPROM_HEDEF_SICAKLIK, hedefSicaklik, 4); break;
        case 1: hedefNem[ayarlananGun] = ayar; 
                eepromFloatDiziYaz(EEPROM_HEDEF_NEM, hedefNem, 4); break;
        case 2: sicaklikTolerans[ayarlananGun] = ayar; 
                eepromFloatDiziYaz(EEPROM_SICAKLIK_TOLERANS, sicaklikTolerans, 4); break;
        case 3: nemTolerans[ayarlananGun] = ayar; 
                eepromFloatDiziYaz(EEPROM_NEM_TOLERANS, nemTolerans, 4); break;
      }
      bildirim = 1;
      bildirimSuresi = currentMillis;
      lcdAnaEkran();
    }
    // Başlat/Durdur menüsüne geçiş
    else if (ekranDurumu == 2 && secim == 0) {
      ekranTemizle = true;
      ekranDurumu = 7;
      secim = 0;
      sonEtkilesimZamani = currentMillis;
    }
    // Başlat/Durdur onayı
    else if (ekranDurumu == 7 && secim == 1) {
      if (!makineAktif) {
        makineAktif = true;
        gunSayisi = 1;
        gecenGunSayisi(gunSayisi);
        baslangicTarih(now.day(), now.month(), now.hour(), now.minute(), 
                      now.second(), (now.year() % 100));
        ekranTemizle = true;
        ekranDurumu = 1;
        secim = 0;
      } else {
        makineAktif = false;
        bitClear(flags, FLAG_ISITICI);
        bitClear(flags, FLAG_NEMLENDIRICI);
        bitClear(flags, FLAG_VIYOL);
        bitClear(flags, FLAG_SIRKULASYON_FAN);
        bitClear(flags, FLAG_EGZOZ_FAN);
        gunSayisi = 0;
        gecenGunSayisi(255);
        baslangicTarih(255, 255, 255, 255, 255, 255);
        EEPROM.update(EEPROM_VIYOL_SAAT, 255);
        EEPROM.update(EEPROM_VIYOL_DAKIKA, 255);
        ekranTemizle = true;
        ekranDurumu = 1;
        secim = 0;
      }
    }
  }
}