#ifndef SERIAL_DEBUG_H
#define SERIAL_DEBUG_H

#include <Arduino.h>

// ===========================================================================
// ======================== Serial Debug Fonksiyonları =======================
// ===========================================================================

/**
 * @brief Seri portu başlatır
 */
void setupSerial();

/**
 * @brief Sistem durumunu seri porta yazdırır
 */
void serialMonitor();

/**
 * @brief Sensor değerlerini yazdırır
 */
void printSensorValues();

/**
 * @brief Bayrak (flag) durumlarını yazdırır
 */
void printFlagStatus();

/**
 * @brief Tarih/saat bilgisini yazdırır
 */
void printDateTime();

/**
 * @brief Kuluçka bilgilerini yazdırır
 */
void printIncubatorInfo();

#endif // SERIAL_DEBUG_H