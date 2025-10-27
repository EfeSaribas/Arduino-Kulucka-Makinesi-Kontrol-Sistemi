#include "RTC.h"
#include "Globals.h"
#include "LCD.h"
#include <avr/wdt.h>

void setupRTC() {
  if (!saat.begin()) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("RTC HATASI!"));
    lcd.setCursor(0, 1);
    lcd.print(F("Servis Gerekli"));
    while (1) {
      wdt_reset();
      delay(200);
    }
  }
}

void rtc() {
  now = saat.now();
  if (!ayarModu) {
    const char* ay;
    switch (now.month()) {
      case 1: ay = "Ocak"; break;
      case 2: ay = "Subat"; break;
      case 3: ay = "Mart"; break;
      case 4: ay = "Nisan"; break;
      case 5: ay = "Mayis"; break;
      case 6: ay = "Haziran"; break;
      case 7: ay = "Temmuz"; break;
      case 8: ay = "Agustos"; break;
      case 9: ay = "Eylul"; break;
      case 10: ay = "Ekim"; break;
      case 11: ay = "Kasim"; break;
      case 12: ay = "Aralik"; break;
      default: ay = "Hata"; break;
    }
    tarihFormati(gunVeAy, now.day(), ay);
  }
}

void rtcAyarla() {
  saat.adjust(DateTime(ayarYil + 2000, ayarAy, ayarGun, ayarSaat, ayarDakika, ayarSaniye));
}

void saatAyarDegerleriYukle() {
  now = saat.now();
  ayarSaat = now.hour();
  ayarDakika = now.minute();
  ayarSaniye = now.second();
  ayarGun = now.day();
  ayarAy = now.month();
  ayarYil = (now.year() % 100);
}

uint8_t ayGunSayisi(int ay, int yil) {
  switch (ay) {
    case 1: case 3: case 5: case 7: case 8: case 10: case 12:
      return 31;
    case 4: case 6: case 9: case 11:
      return 30;
    case 2:
      // Artık yıl kontrolü
      if ((yil % 4 == 0 && yil % 100 != 0) || (yil % 400 == 0)) {
        return 29;
      } else {
        return 28;
      }
    default:
      return 31;
  }
}

uint8_t calculateDayOfWeek(int day, int month, int year) {
  if (month < 3) {
    month += 12;
    year -= 1;
  }
  int K = year % 100;
  int J = year / 100;
  int h = (day + (13 * (month + 1)) / 5 + K + (K / 4) + (J / 4) + 5 * J) % 7;
  return ((h + 5) % 7) + 1;
}