#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>

// ===========================================================================
// ======================== Encoder Fonksiyonları ============================
// ===========================================================================

/**
 * @brief Encoder'ı okur ve seçimi günceller
 */
void encoderOku();

/**
 * @brief Seçimi bir artırır
 */
void secimArttir();

/**
 * @brief Seçimi bir azaltır
 */
void secimAzalt();

/**
 * @brief Buton basımlarını işler
 */
void handleButton();

/**
 * @brief Zaman aşımı kontrolü yapar
 */
void checkTimeout();

#endif // ENCODER_H