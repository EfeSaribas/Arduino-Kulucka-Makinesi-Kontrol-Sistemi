#ifndef EEPROM_MANAGER_H
#define EEPROM_MANAGER_H

#include <Arduino.h>

// ===========================================================================
// ======================== EEPROM Fonksiyonları =============================
// ===========================================================================

/**
 * @brief EEPROM'dan ayarları yükler
 */
void loadEEPROMSettings();

/**
 * @brief EEPROM'a float dizisi yazar
 * @param adres Başlangıç adresi
 * @param dizi Float dizisi
 * @param uzunluk Dizi uzunluğu
 */
void eepromFloatDiziYaz(int adres, float* dizi, int uzunluk);

/**
 * @brief EEPROM'dan float dizisi okur
 * @param adres Başlangıç adresi
 * @param dizi Float dizisi
 * @param uzunluk Dizi uzunluğu
 */
void eepromFloatDiziOku(int adres, float* dizi, int uzunluk);

/**
 * @brief Geçen gün sayısını EEPROM'a yazar
 * @param gun Gün sayısı
 */
void gecenGunSayisi(uint8_t gun);

/**
 * @brief Başlangıç tarih/saatini EEPROM'a yazar
 * @param gun Gün
 * @param ay Ay
 * @param saat Saat
 * @param dakika Dakika
 * @param saniye Saniye
 * @param yil Yıl (son iki hane)
 */
void baslangicTarih(uint8_t gun, uint8_t ay, uint8_t saat, 
                    uint8_t dakika, uint8_t saniye, uint8_t yil);

/**
 * @brief EEPROM'u periyodik olarak günceller (gün sayısı)
 */
void eepromGuncelleAyri();

/**
 * @brief Viyol zamanını EEPROM'a yazar
 * @param saat Saat
 * @param dakika Dakika
 */
void viyolZamaniYaz(uint8_t saat, uint8_t dakika);

/**
 * @brief Viyol zamanını EEPROM'dan okur
 * @param saat Saat referansı
 * @param dakika Dakika referansı
 */
void viyolZamaniOku(uint8_t& saat, uint8_t& dakika);

#endif // EEPROM_MANAGER_H