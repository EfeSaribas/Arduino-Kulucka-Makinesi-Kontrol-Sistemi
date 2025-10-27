#ifndef SAFETY_H
#define SAFETY_H

#include <Arduino.h>

// ===========================================================================
// ======================== Güvenlik Fonksiyonları ===========================
// ===========================================================================

/**
 * @brief Sistem güvenlik kontrollerini yapar
 */
void guvenlikKontrolleri();

/**
 * @brief Acil durdurma prosedürü
 * @param sebep Durdurma sebebi
 */
void acilDurdurma(String sebep);

/**
 * @brief Aşırı sıcaklık kontrolü
 * @return true Sıcaklık kritik seviyede
 * @return false Sıcaklık normal
 */
bool asiriSicaklikKontrol();

/**
 * @brief Sensör hata kontrolü
 * @return true Sensör hatası var
 * @return false Sensör normal çalışıyor
 */
bool sensorHataKontrol();

/**
 * @brief Isıtıcı maksimum çalışma süresi kontrolü
 */
void isiticiSureKontrol();

/**
 * @brief Kapak sensörü doğrulama kontrolü
 */
void kapakSensorDogrulama();

/**
 * @brief Watchdog timer'ı resetler
 */
void resetWatchdog();

#endif // SAFETY_H