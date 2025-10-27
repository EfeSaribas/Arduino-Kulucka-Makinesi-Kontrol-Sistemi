#ifndef INCUBATOR_H
#define INCUBATOR_H

#include <Arduino.h>

// ===========================================================================
// ======================== Kuluçka Kontrol Fonksiyonları ====================
// ===========================================================================

/**
 * @brief Ana kuluçka sürecini yönetir
 */
void kuluckaSureci();

/**
 * @brief Geçen gün sayısını hesaplar
 */
void gunHesapla();

/**
 * @brief Ortam düzenleyicilerini (ısıtıcı, nem, fan) kontrol eder
 */
void ortamDuzenliyicileri();

/**
 * @brief Viyol (çevirme) sürecini yönetir
 */
void viyolKontrol();

/**
 * @brief Sıcaklık kontrolü yapar
 * @param gun Gün aralığı indeksi (0-3)
 */
void sicaklikKontrol(uint8_t gun);

/**
 * @brief Nem kontrolü yapar
 * @param gun Gün aralığı indeksi (0-3)
 */
void nemKontrol(uint8_t gun);

/**
 * @brief Kapak durumuna göre sistemleri yönetir
 */
void kapakDurumuKontrol();

#endif // INCUBATOR_H