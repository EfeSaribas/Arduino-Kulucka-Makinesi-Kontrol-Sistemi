#include "Globals.h"
#include "Pins.h"

// ===========================================================================
// ======================== Global Değişken Tanımlamaları ====================
// ===========================================================================

// Zamanlama değişkenleri
unsigned long sonEkranGuncelleme = 0;
unsigned long sonGuvenlikKontrolu = 0;
unsigned long isiticiBaslangicZamani = 0;
unsigned long sonOkumaZamani = 0;
unsigned long sonDHTOkumaZamani = 0;
unsigned long sonEtkilesimZamani = 0;
unsigned long bildirimSuresi = 0;
unsigned long sonBasilma = 0;
unsigned long nemResetStart = 0;
unsigned long oncekiZaman = 0;

// Sensor değişkenleri
float santigrat = 0;
float nem = 0;
float hedefSicaklik[4] = {
  GUN_1_7_SICAKLIK,
  GUN_8_14_SICAKLIK,
  GUN_15_18_SICAKLIK,
  GUN_19_PLUS_SICAKLIK
};
float hedefNem[4] = {
  GUN_1_7_NEM,
  GUN_8_14_NEM,
  GUN_15_18_NEM,
  GUN_19_PLUS_NEM
};
float sicaklikTolerans[4] = {
  GUN_1_7_SIC_TOLERANS,
  GUN_8_14_SIC_TOLERANS,
  GUN_15_18_SIC_TOLERANS,
  GUN_19_PLUS_SIC_TOLERANS
};
float nemTolerans[4] = {
  GUN_1_7_NEM_TOLERANS,
  GUN_8_14_NEM_TOLERANS,
  GUN_15_18_NEM_TOLERANS,
  GUN_19_PLUS_NEM_TOLERANS
};

// Ayar değişkenleri
float ayar = 0;

// String değişkenleri
char gunVeAy[10];

// Sayaç değişkenleri
uint8_t sistemHataSayaci = 0;
uint8_t maxDeger = 0;
uint8_t deger = 0;
uint8_t secim = 0;
uint8_t ekranDurumu = 1;
uint8_t gunSayisi = 0;
uint8_t ayarSaat = 0;
uint8_t ayarDakika = 0;
uint8_t ayarSaniye = 0;
uint8_t ayarGun = 0;
uint8_t ayarAy = 0;
uint8_t ayarYil = 0;
uint8_t bildirim = 0;
uint8_t ayarlananGun = 0;
uint8_t secimAyar = 0;
uint8_t viyolSaat = 0;
uint8_t viyolDakika = 0;
uint8_t fanRPM = FAN_HIZLI_RPM;

// Boolean değişkenler
bool okumaHazir = false;
bool ekranTemizle = true;
bool etkilesim = false;
bool clkSonDurum = false;
bool ayarModu = false;
bool editModu = false;
bool kayitBtn = false;
bool zamanAsimiAktif = false;
bool makineAktif = false;
bool nemResetBekleme = false;
bool guvenlikModu = false;

// Bayrak değişkeni
byte flags = 0;

// ===========================================================================
// ======================== Global Objeler ===================================
// ===========================================================================

LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLS, LCD_ROWS);
OneWire oneWire(sicaklikSensor);
DallasTemperature DS18B20(&oneWire);
DeviceAddress DS18B20adres;
DHT dht(nemSensor, DHT22);
RTC_DS3231 saat;
DateTime now;