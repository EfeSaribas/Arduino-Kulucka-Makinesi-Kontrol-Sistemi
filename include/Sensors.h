#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

// ===========================================================================
// ======================== Sensör Fonksiyonları =============================
// ===========================================================================

/**
 * @brief Tüm sensörleri başlatır
 */
void setupSensors();

/**
 * @brief Ortam sensörlerini okur (sıcaklık, nem, kapak durumu)
 */
void ortamSensorOku();

/**
 * @brief DS18B20 sıcaklık sensörünü okur
 */
void readTemperatureSensor();

/**
 * @brief DHT22 nem sensörünü okur
 */
void readHumiditySensor();

/**
 * @brief Kapak sensörünü okur ve durumu günceller
 */
void readDoorSensor();

/**
 * @brief Nem sensörü reset işlemi
 */
void resetHumiditySensor();

#endif // SENSORS_H