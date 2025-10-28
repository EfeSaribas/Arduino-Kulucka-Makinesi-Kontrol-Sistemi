#include <Arduino.h>
#include <avr/wdt.h>

#include "Configuration.h"
#include "Globals.h"
#include "Pins.h"
#include "LCD.h"
#include "Sensors.h"
#include "RTC.h"
#include "EEPROM_Manager.h"
#include "Encoder.h"
#include "Incubator.h"
#include "Safety.h"
#include "SerialDebug.h"
#include "Alarm.h"

void setup()
{
  // Watchdog başlat
  wdt_enable(WDT_TIMEOUT);

  // Seri port başlat
  setupSerial();

  // Pin'leri başlat
  setupPins();

  // LCD başlat
  setupLCD();
  lcd.setCursor(7, 1);
  lcd.print(F("Sistem"));
  lcd.setCursor(4, 2);
  lcd.print(F("Baslatiliyor"));

  // Sensörleri başlat
  setupSensors();

  // RTC başlat
  setupRTC();

  // EEPROM'dan ayarları yükle
  loadEEPROMSettings();

  delay(3000);
  lcd.clear();
}

void loop()
{
  // Watchdog'u resetle
  resetWatchdog();

  // Güvenlik kontrolleri
  guvenlikKontrolleri();

  // Ortam düzenleyicileri
  ortamDuzenliyicileri();

  // RTC güncelle
  now = saat.now();
  unsigned long currentMillis = millis();

  // Seri monitor (her saniye)
  if (currentMillis - oncekiZaman >= SERIAL_MONITOR_SURESI)
  {
    oncekiZaman = currentMillis;
    serialMonitor();
  }

  // Kuluçka süreci
  if (makineAktif)
  {
    kuluckaSureci();
    eepromGuncelleAyri();
  }

  // Encoder oku
  encoderOku();

  // Buton işle
  handleButton();

  // Zaman aşımı kontrolü
  checkTimeout();

  // Cursor göster
  if (ekranDurumu == 2 || ekranDurumu == 3 || ekranDurumu == 4 ||
      ekranDurumu == 5 || ekranDurumu == 6 || ekranDurumu == 7)
  {
    lcdCursor(secim);
  }

  // Ana ekranda sensör oku
  if (ekranDurumu == 1)
  {
    rtc();
  }

  ortamSensorOku();

  alarm();

  // Edit modu değer güncellemeleri
  if (editModu && ayarModu)
  {
    switch (secim)
    {
    case 0:
      ayarSaat = deger;
      break;
    case 1:
      ayarDakika = deger;
      break;
    case 2:
      ayarSaniye = deger;
      break;
    case 3:
      ayarGun = deger;
      break;
    case 4:
      ayarAy = deger;
      break;
    case 5:
      ayarYil = deger;
      break;
    }
  }

  // Ekran temizle
  if (ekranTemizle)
  {
    lcd.clear();
    ekranTemizle = false;
  }

  // Ekran güncelle
  if (currentMillis - sonEkranGuncelleme >= EKRAN_GUNCELLEME_SURESI)
  {
    switch (ekranDurumu)
    {
    case 1:
      lcdAnaEkran();
      break;
    case 2:
      lcdMenu();
      break;
    case 3:
      lcdSaatAyar();
      if (!ayarModu)
        saatAyarDegerleriYukle();
      break;
    case 4:
      lcdKuluckaAyar();
      break;
    case 5:
      lcdAyarMenu();
      break;
    case 6:
      lcdAyarMenu2();
      break;
    case 7:
      lcdBaslatDurdur();
      break;
    }
    sonEkranGuncelleme = currentMillis;
  }

  // Bildirim güncelleme
  if (makineAktif && bildirim != 2 && currentMillis - bildirimSuresi >= 3000)
  {
    if (bitRead(flags, FLAG_KAPAK_DURUMU))
      bildirim = 2;
    else if (!bitRead(flags, FLAG_KAPAK_DURUMU) && makineAktif)
      bildirim = 3;
  }
  else if (!makineAktif && bildirim != 0 && currentMillis - bildirimSuresi >= 3000)
  {
    bildirim = 0;
  }
}