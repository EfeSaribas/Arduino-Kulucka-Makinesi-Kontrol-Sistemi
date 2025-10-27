#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <DHT.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>
#include <DallasTemperature.h>
#include "Configuration.h"

// ===========================================================================
// ======================== Global Değişkenler ===============================
// ===========================================================================

// Zamanlama değişkenleri
extern unsigned long sonEkranGuncelleme;
extern unsigned long sonGuvenlikKontrolu;
extern unsigned long isiticiBaslangicZamani;
extern unsigned long sonOkumaZamani;
extern unsigned long sonDHTOkumaZamani;
extern unsigned long sonEtkilesimZamani;
extern unsigned long bildirimSuresi;
extern unsigned long sonBasilma;
extern unsigned long nemResetStart;
extern unsigned long oncekiZaman;

// Sensor değişkenleri
extern float santigrat;
extern float nem;
extern float hedefSicaklik[4];
extern float hedefNem[4];
extern float sicaklikTolerans[4];
extern float nemTolerans[4];

// Ayar değişkenleri
extern float ayar;

// String değişkenleri
extern char gunVeAy[10];

// Sayaç değişkenleri
extern uint8_t sistemHataSayaci;
extern uint8_t maxDeger;
extern uint8_t deger;
extern uint8_t secim;
extern uint8_t ekranDurumu;
extern uint8_t gunSayisi;
extern uint8_t ayarSaat;
extern uint8_t ayarDakika;
extern uint8_t ayarSaniye;
extern uint8_t ayarGun;
extern uint8_t ayarAy;
extern uint8_t ayarYil;
extern uint8_t bildirim;
extern uint8_t ayarlananGun;
extern uint8_t secimAyar;
extern uint8_t viyolSaat;
extern uint8_t viyolDakika;
extern uint8_t fanRPM;

// Boolean değişkenler
extern bool okumaHazir;
extern bool ekranTemizle;
extern bool etkilesim;
extern bool clkSonDurum;
extern bool ayarModu;
extern bool editModu;
extern bool kayitBtn;
extern bool zamanAsimiAktif;
extern bool makineAktif;
extern bool nemResetBekleme;
extern bool guvenlikModu;

// Bayrak değişkeni (flags)
extern byte flags;

// ===========================================================================
// ======================== Global Objeler ===================================
// ===========================================================================

extern LiquidCrystal_I2C lcd;
extern OneWire oneWire;
extern DallasTemperature DS18B20;
extern DeviceAddress DS18B20adres;
extern DHT dht;
extern RTC_DS3231 saat;
extern DateTime now;

#endif // GLOBALS_H