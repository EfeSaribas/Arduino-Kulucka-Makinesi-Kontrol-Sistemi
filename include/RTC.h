#ifndef RTC_H
#define RTC_H

#include <Arduino.h>

// ===========================================================================
// ======================== RTC Fonksiyonları ================================
// ===========================================================================

/**
 * @brief RTC modülünü başlatır
 */
void setupRTC();

/**
 * @brief RTC'den tarih/saat bilgisini okur
 */
void rtc();

/**
 * @brief RTC'ye yeni tarih/saat ayarlar
 */
void rtcAyarla();

/**
 * @brief Saat ayar değerlerini mevcut zamandan yükler
 */
void saatAyarDegerleriYukle();

/**
 * @brief Ay içindeki gün sayısını hesaplar
 * @param ay Ay numarası (1-12)
 * @param yil Yıl
 * @return uint8_t Gün sayısı
 */
uint8_t ayGunSayisi(int ay, int yil);

/**
 * @brief Haftanın gününü hesaplar (Zeller algoritması)
 * @param day Gün
 * @param month Ay
 * @param year Yıl
 * @return uint8_t Haftanın günü (1=Pazartesi, 7=Pazar)
 */
uint8_t calculateDayOfWeek(int day, int month, int year);

#endif // RTC_H