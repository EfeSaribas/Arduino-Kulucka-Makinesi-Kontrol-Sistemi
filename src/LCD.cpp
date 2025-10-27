#include "LCD.h"
#include "Globals.h"
#include "RTC.h"
#include "Configuration.h"
#include <avr/pgmspace.h>

// Özel Karakterler
const byte nemSembol[8] PROGMEM = {
  B00100, B00100, B01110, B01110, B11111, B11111, B11111, B01110
};

const byte sicaklikSembol[8] PROGMEM = {
  B00100, B01010, B01010, B01010, B01110, B01110, B01110, B00100
};

const byte artmakSembol[8] PROGMEM = {
  B00100, B01110, B11111, B00100, B00100, B00100, B00100, B00100
};

const byte dusmekSembol[8] PROGMEM = {
  B00100, B00100, B00100, B00100, B00100, B11111, B01110, B00100
};

const byte altCizgi[8] PROGMEM = {
  B11111, B11111, B00000, B00000, B00000, B00000, B00000, B00000
};

void setupLCD() {
  lcd.init();
  lcd.backlight();
  createCustomChars();
}

void createCustomChars() {
  lcd.createChar(0, (uint8_t*)sicaklikSembol);
  lcd.createChar(1, (uint8_t*)nemSembol);
  lcd.createChar(2, (uint8_t*)artmakSembol);
  lcd.createChar(3, (uint8_t*)dusmekSembol);
  lcd.createChar(5, (uint8_t*)altCizgi);
}

void lcdAnaEkran() {
  rtc();
  now = saat.now();
  
  // Sıcaklık Göstergesi
  lcd.setCursor(0, 0);
  lcd.write(byte(0));
  lcd.print(santigrat, 1);
  lcd.write(223);
  lcd.print("C");
  if (bitRead(flags, FLAG_ISITICI)) {
    lcd.write(byte(2));
    lcd.print(" ");
  } else {
    lcd.print(F("  "));
  }
  
  // Nem Göstergesi
  lcd.setCursor(0, 1);
  lcd.write(byte(1));
  lcd.print(nem, 1);
  lcd.print(F("%"));
  if (bitRead(flags, FLAG_NEMLENDIRICI)) lcd.write(byte(2));
  if (bitRead(flags, FLAG_EGZOZ_FAN)) {
    lcd.write(byte(3));
    lcd.print(F(" "));
  } else {
    lcd.print(F("  "));
  }
  
  // Saat
  lcd.setCursor(15, 0);
  if (now.hour() < 10) lcd.print(F("0"));
  lcd.print(now.hour());
  lcd.print(F(":"));
  if (now.minute() < 10) lcd.print(F("0"));
  lcd.print(now.minute());
  
  // Ay ve Gün
  lcd.setCursor(10, 1);
  lcd.print(gunVeAy);
  
  // Haftanın Günü
  lcd.setCursor(10, 2);
  switch (calculateDayOfWeek(now.day(), now.month(), now.year())) {
    case 1: lcd.print(F(" Pazartesi")); break;
    case 2: lcd.print(F("      Sali")); break;
    case 3: lcd.print(F("  Carsamba")); break;
    case 4: lcd.print(F("  Persembe")); break;
    case 5: lcd.print(F("      Cuma")); break;
    case 6: lcd.print(F(" Cumartesi")); break;
    case 7: lcd.print(F("     Pazar")); break;
    default: lcd.print(F("Bilinmiyor")); break;
  }
  
  // Geçen Gün Sayısı
  lcd.setCursor(0, 2);
  lcd.print(F("Gun: "));
  if (gunSayisi == 0) {
    lcd.print(F("--"));
  } else if (gunSayisi < 10) {
    lcd.print(gunSayisi);
    lcd.print(F(" "));
  } else {
    lcd.print(gunSayisi);
  }
  
  // Sistem Durumu
  lcd.setCursor(0, 3);
  lcd.print(F("Durum: "));
  switch (bildirim) {
    case 0: lcd.print(F("Beklemede... ")); break;
    case 1: lcd.print(F("Kaydedildi!  ")); break;
    case 2: lcd.print(F("Kulucka Aktif")); break;
    case 3: lcd.print(F("Kapak Acik!  ")); break;
  }
}

void lcdMenu() {
  lcd.setCursor(7, 0);
  lcd.print(makineAktif ? F("Durdur") : F("Baslat"));
  lcd.setCursor(4, 1);
  lcd.print(F("Kulucka Ayar"));
  lcd.setCursor(5, 2);
  lcd.print(F("Saat/Tarih"));
  lcd.setCursor(6, 3);
  lcd.print(F("Geri Don"));
}

void lcdSaatAyar() {
  // Saat Bilgileri
  lcd.setCursor(0, 0);
  if (ayarSaat < 10) lcd.print(F("0"));
  lcd.print(ayarSaat);
  lcd.print(":");
  if (ayarDakika < 10) lcd.print(F("0"));
  lcd.print(ayarDakika);
  lcd.print(":");
  if (ayarSaniye < 10) lcd.print(F("0"));
  lcd.print(ayarSaniye);
  
  // Tarih Bilgileri
  lcd.setCursor(12, 0);
  if (ayarGun < 10) lcd.print(F("0"));
  lcd.print(ayarGun);
  lcd.print("/");
  if (ayarAy < 10) lcd.print(F("0"));
  lcd.print(ayarAy);
  lcd.print("/");
  if (ayarYil < 10) lcd.print(F("0"));
  lcd.print(ayarYil);
  
  // Butonlar
  lcd.setCursor(6, 3);
  lcd.print(F("Geri Don"));
  if (kayitBtn) {
    lcd.setCursor(7, 2);
    lcd.print(F("Kaydet"));
  } else {
    lcd.setCursor(7, 2);
    lcd.print(F("      "));
  }
}

void lcdKuluckaAyar() {
  lcd.setCursor(6, 0);
  lcd.print(F("Gun 1-7"));
  lcd.setCursor(6, 1);
  lcd.print(F("Gun 8-14"));
  lcd.setCursor(5, 2);
  lcd.print(F("Gun 15-17"));
  lcd.setCursor(5, 3);
  lcd.print(F("Gun 18-21+"));
}

void lcdAyarMenu() {
  lcd.setCursor(6, 0);
  lcd.print(F("Hedef S."));
  lcd.setCursor(6, 1);
  lcd.print(F("Hedef N."));
  lcd.setCursor(4, 2);
  lcd.print(F("S. Tolerans"));
  lcd.setCursor(4, 3);
  lcd.print(F("N. Tolerans"));
}

void lcdAyarMenu2() {
  switch (secimAyar) {
    case 0: lcd.setCursor(0, 0); lcd.print(F("Hedef S.   : ")); break;
    case 1: lcd.setCursor(0, 0); lcd.print(F("Hedef Nem  : ")); break;
    case 2: lcd.setCursor(0, 0); lcd.print(F("S. Tolerans: ")); break;
    case 3: lcd.setCursor(0, 0); lcd.print(F("N. Tolerans: ")); break;
  }
  lcd.print(ayar, 1);
  
  if (kayitBtn) {
    lcd.setCursor(7, 2);
    lcd.print(F("Kaydet"));
  } else {
    lcd.setCursor(7, 2);
    lcd.print(F("      "));
  }
  
  lcd.setCursor(6, 3);
  lcd.print(F("Geri Don"));
}

void lcdBaslatDurdur() {
  if (!makineAktif) {
    lcd.setCursor(1, 0);
    lcd.print(F("Islemi Baslatmayi"));
    lcd.setCursor(2, 1);
    lcd.print(F("Onayliyor musun?"));
  } else {
    lcd.setCursor(1, 0);
    lcd.print(F("Islemi Durdurmayi"));
    lcd.setCursor(2, 1);
    lcd.print(F("Onayliyor musun?"));
  }
  lcd.setCursor(3, 3);
  lcd.print(F("Hayir"));
  lcd.setCursor(13, 3);
  lcd.print(F("Evet"));
}

// LCD.cpp devamı - lcdCursor ve yardımcı fonksiyonlar

void lcdCursor(uint8_t row) {
  static uint8_t oncekiRow = 255;
  
  // Menu Ekranı için Cursor
  if (ekranDurumu == 2 || ekranDurumu == 4 || ekranDurumu == 5) {
    if (row != oncekiRow) {
      for (int i = 0; i < 4; i++) {
        lcd.setCursor(1, i);
        lcd.print(F("  "));
        lcd.setCursor(17, i);
        lcd.print(F("  "));
      }
    }
    lcd.setCursor(1, row);
    lcd.print(F("=>"));
    lcd.setCursor(17, row);
    lcd.print(F("<="));
  }
  // Saat/Tarih Ekranı için Cursor
  else if (ekranDurumu == 3) {
    if (row != oncekiRow) {
      if (oncekiRow == 6) {
        lcd.setCursor(3, 3);
        lcd.print(F("   "));
      } else if (oncekiRow == 7) {
        lcd.setCursor(3, 2);
        lcd.print(F("   "));
      } else {
        for (int i = 0; i < 20; i++) {
          lcd.setCursor(i, 1);
          lcd.print(F(" "));
        }
      }
    }
    
    switch (row) {
      case 0: printCursorSymbol(0,  1); break;
      case 1: printCursorSymbol(3,  1); break;
      case 2: printCursorSymbol(6,  1); break;
      case 3: printCursorSymbol(12, 1); break;
      case 4: printCursorSymbol(15, 1); break;
      case 5: printCursorSymbol(18, 1); break;
      case 6: lcd.setCursor(3, 3); lcd.print(F("=>")); break;
      case 7: lcd.setCursor(3, 2); lcd.print(F("=>")); break;
    }
  }
  else if (ekranDurumu == 6) {
    if (row != oncekiRow) {
      if (oncekiRow == 1) {
        lcd.setCursor(3, 3);
        lcd.print(F("   "));
      } else if (oncekiRow == 2) {
        lcd.setCursor(3, 2);
        lcd.print(F("   "));
      } else {
        for (int i = 0; i < 20; i++) {
          lcd.setCursor(i, 1);
          lcd.print(F(" "));
        }
      }
    }
    switch (row) {
      case 0: printCursorSymbol(13, 1); break;
      case 1: lcd.setCursor(3, 3); lcd.print(F("=>")); break;
      case 2: lcd.setCursor(3, 2); lcd.print(F("=>")); break;
    }
  }
  else if (ekranDurumu == 7) {
    if (row != oncekiRow) {
      if (oncekiRow == 0) {
        lcd.setCursor(2, 3);
        lcd.print(F(" "));
      } else if (oncekiRow == 1) {
        lcd.setCursor(12, 3);
        lcd.print(F(" "));
      }
    }
    switch (row) {
      case 0: lcd.setCursor(2, 3); lcd.print(F(">")); break;
      case 1: lcd.setCursor(12, 3); lcd.print(F(">")); break;
    }
  }
  
  oncekiRow = row;
}

void printCursorSymbol(uint8_t col, uint8_t row) {
  lcd.setCursor(col, row);
  if (!editModu) {
    lcd.write(byte(2));
    lcd.write(byte(2));
  } else {
    lcd.write(byte(5));
    lcd.write(byte(5));
  }
}

void tarihFormati(char* hedef, int gun, const char* ay) {
  char temp[20];
  snprintf(temp, 20, "%d %s", gun, ay);
  
  // 10 karakterlik alan için sağa yasla
  int len = strlen(temp);
  int spaces = 10 - len;
  
  // Sağa yaslama - soldan boşluk ekle
  if (spaces > 0) {
    memset(hedef, ' ', spaces);
    strcpy(hedef + spaces, temp);
  } else {
    strcpy(hedef, temp);
  }
}

void guvenlilcdYaz(int col, int row, String text) {
  static unsigned long sonLcdZamani = 0;
  unsigned long simdi = millis();
  
  if (simdi - sonLcdZamani < 50) return;  // LCD'ye çok sık yazma engelle
  
  lcd.setCursor(col, row);
  lcd.print(text);
  sonLcdZamani = simdi;
}