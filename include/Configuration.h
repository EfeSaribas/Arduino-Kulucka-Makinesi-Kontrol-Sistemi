#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <Arduino.h>

// ===========================================================================
// ======================== Pin Tanımlamaları ================================
// ===========================================================================

#define PIN_BTN                 A0
#define PIN_DT                  A1
#define PIN_CLK                 A2
#define PIN_KAPAK_SENSOR        A3
#define PIN_BUZZER              4
#define PIN_SIRKULASYON_FAN     5
#define PIN_VIYOL               6
#define PIN_NEM_RESET           7
#define PIN_EGZOZ_FAN           8
#define PIN_NEM_SENSOR          9
#define PIN_SICAKLIK_SENSOR     10
#define PIN_REZISTANS           11
#define PIN_NEMLENDIRICI        12

// ===========================================================================
// ======================== LCD Ayarları =====================================
// ===========================================================================

#ifndef LCD_ADDRESS
  #define LCD_ADDRESS 0x27
#endif

#ifndef LCD_COLS
  #define LCD_COLS 20
#endif

#ifndef LCD_ROWS
  #define LCD_ROWS 4
#endif
// ===========================================================================
// ======================== Buzzer Frekansı ==================================
// ===========================================================================

#define BUZZER_FREKANS 2400 // Hz

// ===========================================================================
// ======================== Zamanlama Sabitleri ==============================
// ===========================================================================

#define EKRAN_GUNCELLEME_SURESI     200     // ms
#define GUVENLIK_KONTROL_SURESI     10000   // ms
#define ZAMAN_ASIMI_SURESI          10000   // ms
#define BUTON_DEBOUNCE_SURESI       300     // ms
#define ENCODER_DEBOUNCE_SURESI     5       // ms
#define KAPAK_KONTROL_SURESI        500     // ms
#define DS18B20_OKUMA_SURESI        1000    // ms
#define DS18B20_BEKLEME_SURESI      750     // ms
#define DHT_OKUMA_SURESI            2000    // ms
#define SERIAL_MONITOR_SURESI       1000    // ms
#define EEPROM_GUNCELLEME_SURESI    300000  // ms (5 dakika)
#define BUZZER_ACIK_SURESI          150     // ms
#define BUZZER_KAPALI_SURESI        100     // ms
#define BUZZER_BEKLEME_SURESI       500     // ms

// ===========================================================================
// ======================== Sistem Güvenlik Ayarları =========================
// ===========================================================================

#define KRITIK_SICAKLIK         45.0    // °C
#define MIN_GECERLI_SICAKLIK    -20.0   // °C
#define MAX_GECERLI_SICAKLIK    60.0    // °C
#define SENSOR_HATA_LIMITI      3       // kez
#define ISITICI_MAX_SURE        120000  // ms (2 dakika)
#define KAPAK_OKUMA_DOGRULAMA   3       // kez

// ===========================================================================
// ======================== EEPROM Adres Haritası ============================
// ===========================================================================

#define EEPROM_GUN_SAYISI           0       // 1 byte
#define EEPROM_BASLANGIC_GUN        1       // 1 byte
#define EEPROM_BASLANGIC_AY         2       // 1 byte
#define EEPROM_HEDEF_SICAKLIK       3       // 16 bytes (4 float)
#define EEPROM_HEDEF_NEM            19      // 16 bytes (4 float)
#define EEPROM_SICAKLIK_TOLERANS    35      // 16 bytes (4 float)
#define EEPROM_NEM_TOLERANS         51      // 16 bytes (4 float)
#define EEPROM_BASLANGIC_SAAT       67      // 1 byte
#define EEPROM_BASLANGIC_DAKIKA     68      // 1 byte
#define EEPROM_BASLANGIC_SANIYE     69      // 1 byte
#define EEPROM_BASLANGIC_YIL        70      // 1 byte
#define EEPROM_VIYOL_SAAT           71      // 1 byte
#define EEPROM_VIYOL_DAKIKA         72      // 1 byte

// ===========================================================================
// ======================== Kuluçka Parametreleri ============================
// ===========================================================================

#define GUN_1_7_SICAKLIK        37.7
#define GUN_1_7_NEM             55
#define GUN_1_7_SIC_TOLERANS    0.3
#define GUN_1_7_NEM_TOLERANS    5

#define GUN_8_14_SICAKLIK       37.7
#define GUN_8_14_NEM            50
#define GUN_8_14_SIC_TOLERANS   0.3
#define GUN_8_14_NEM_TOLERANS   5

#define GUN_15_18_SICAKLIK      37.7
#define GUN_15_18_NEM           50
#define GUN_15_18_SIC_TOLERANS  0.3
#define GUN_15_18_NEM_TOLERANS  5

#define GUN_19_PLUS_SICAKLIK    37.0
#define GUN_19_PLUS_NEM         70
#define GUN_19_PLUS_SIC_TOLERANS 0.3
#define GUN_19_PLUS_NEM_TOLERANS 5

// ===========================================================================
// ======================== Viyol (Çevirme) Ayarları =========================
// ===========================================================================

#define VIYOL_PERIYOT           240     // dakika (4 saat)
#define VIYOL_CALISMA_SURESI    12000   // ms (12 saniye)
#define VIYOL_MIN_ZAMAN_FARKI   2       // dakika

// ===========================================================================
// ======================== Fan Ayarları =====================================
// ===========================================================================

#define FAN_HIZLI_RPM           255
#define FAN_YAVAS_RPM           200

// ===========================================================================
// ======================== Bayrak (Flag) Bit Tanımları ======================
// ===========================================================================

#define FLAG_ISITICI            0
#define FLAG_NEMLENDIRICI       1
#define FLAG_VIYOL              2
#define FLAG_SIRKULASYON_FAN    3
#define FLAG_EGZOZ_FAN          4
#define FLAG_KAPAK_DURUMU       5
#define FLAG_ALARM_DURUMU       6

// ===========================================================================
// ======================== Watchdog Ayarları ================================
// ===========================================================================

#ifndef WDT_TIMEOUT
  #define WDT_TIMEOUT WDTO_8S
#endif

// ===========================================================================
// ======================== Seri Port Ayarları ===============================
// ===========================================================================

#ifndef BAUD_RATE
  #define BAUD_RATE 115200
#endif

#endif // CONFIGURATION_H