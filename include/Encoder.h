#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>

// ===========================================================================
// ======================== Encoder Fonksiyonları ============================
// ===========================================================================

/**
 * @brief Encoder'ı başlatır (interrupt modunda)
 */
void setupEncoder();

/**
 * @brief Encoder'ı okur ve seçimi günceller
 * @note Ana loop'ta sürekli çağrılır
 */
void encoderOku();

/**
 * @brief Encoder interrupt handler (ISR)
 * @note Hardware interrupt ile otomatik çağrılır
 */
void encoderISR();

/**
 * @brief Encoder pozisyonunu okur
 * @return int Mevcut encoder pozisyonu
 */
int encoderPozisyonOku();

/**
 * @brief Encoder pozisyonunu sıfırlar
 */
void encoderPozisyonSifirla();

/**
 * @brief Seçimi bir artırır (ekran durumuna göre)
 */
void secimArttir();

/**
 * @brief Seçimi bir azaltır (ekran durumuna göre)
 */
void secimAzalt();

/**
 * @brief Buton basımlarını işler
 * @note Ana loop'ta sürekli çağrılır
 */
void handleButton();

/**
 * @brief Buton interrupt handler (ISR)
 * @note Hardware interrupt ile otomatik çağrılır
 */
void butonISR();

/**
 * @brief Zaman aşımı kontrolü yapar
 */
void checkTimeout();

/**
 * @brief Encoder hızını hesaplar (RPM benzeri)
 * @return float Encoder dönüş hızı
 */
float encoderHiziniHesapla();

/**
 * @brief Hızlı dönüşte adım sayısını artırır (akselerasyon)
 * @param hiz Encoder dönüş hızı
 * @return uint8_t Uygulanacak adım sayısı
 */
uint8_t encoderAkselerasyonu(float hiz);

#endif // ENCODER_H