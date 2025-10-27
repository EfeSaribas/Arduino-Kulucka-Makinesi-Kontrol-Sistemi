#ifndef LCD_H
#define LCD_H

#include <Arduino.h>

// ===========================================================================
// ======================== LCD Fonksiyonları ================================
// ===========================================================================

/**
 * @brief LCD ekranını başlatır ve özel karakterleri yükler
 */
void setupLCD();

/**
 * @brief Ana ekranı gösterir
 */
void lcdAnaEkran();

/**
 * @brief Ana menüyü gösterir
 */
void lcdMenu();

/**
 * @brief Saat/Tarih ayar ekranını gösterir
 */
void lcdSaatAyar();

/**
 * @brief Kuluçka ayar menüsünü gösterir (gün seçimi)
 */
void lcdKuluckaAyar();

/**
 * @brief Parametre ayar menüsünü gösterir
 */
void lcdAyarMenu();

/**
 * @brief Parametre düzenleme ekranını gösterir
 */
void lcdAyarMenu2();

/**
 * @brief Başlat/Durdur onay ekranını gösterir
 */
void lcdBaslatDurdur();

/**
 * @brief LCD cursor'ı yerleştirir
 * @param row Satır numarası
 */
void lcdCursor(uint8_t row);

/**
 * @brief Cursor sembolünü yazdırır
 * @param col Sütun numarası
 * @param row Satır numarası
 */
void printCursorSymbol(uint8_t col, uint8_t row);

/**
 * @brief Tarih formatını hazırlar
 * @param hedef Hedef karakter dizisi
 * @param gun Gün değeri
 * @param ay Ay ismi
 */
void tarihFormati(char* hedef, int gun, const char* ay);

/**
 * @brief Güvenli LCD yazma fonksiyonu (acil durumlar için)
 * @param col Sütun numarası
 * @param row Satır numarası
 * @param text Yazdırılacak metin
 */
void guvenlilcdYaz(int col, int row, String text);

/**
 * @brief Özel LCD karakterlerini oluşturur
 */
void createCustomChars();

#endif // LCD_H