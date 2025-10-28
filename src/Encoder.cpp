#include "Encoder.h"
#include "Globals.h"
#include "Pins.h"
#include "LCD.h"
#include "RTC.h"
#include "EEPROM_Manager.h"
#include "Configuration.h"
#include <EEPROM.h>

// ===========================================================================
// ======================== Encoder Değişkenleri =============================
// ===========================================================================

// Encoder durumu için state machine
volatile int8_t encoderDelta = 0;  // ISR tarafından güncellenir
volatile uint8_t encoderSonDurum = 0;
volatile unsigned long sonEncoderZamani = 0;
volatile unsigned long encoderHareketSayisi = 0;

// Encoder lookup table (daha hızlı ve doğru okuma)
const int8_t ENCODER_TABLE[] PROGMEM = {
  0, -1,  1,  0,
  1,  0,  0, -1,
  -1,  0,  0,  1,
  0,  1, -1,  0
};

// Akselerasyon için
unsigned long sonHizHesaplama = 0;
float encoderHizi = 0;

// Buton için
volatile bool butonBasildi = false;
volatile unsigned long butonBasilmaZamani = 0;

// ===========================================================================
// ======================== Encoder Başlatma =================================
// ===========================================================================

void setupEncoder() {
  // Interrupt pinleri için Arduino UNO'da:
  // INT0 = D2 (CLK için)
  // INT1 = D3 (Opsiyonel, DT için)
  // Ancak kodunuzda A2 kullanıyorsunuz, o yüzden polling yöntemi kullanacağız
  // Eğer D2/D3 kullanırsanız aşağıdaki aktif hale getirin:
  
  // attachInterrupt(digitalPinToInterrupt(2), encoderISR, CHANGE);
  // attachInterrupt(digitalPinToInterrupt(3), butonISR, FALLING);
  
  // Mevcut pin yapılandırmanız için başlangıç değerleri
  uint8_t MSB = digitalRead(clk);
  uint8_t LSB = digitalRead(dt);
  encoderSonDurum = (MSB << 1) | LSB;
}

// ===========================================================================
// ======================== ISR (Interrupt Service Routine) ==================
// ===========================================================================

// D2, D3 pinleri kullanılırsa bu fonksiyonlar aktif olur
void encoderISR() {
  unsigned long simdi = micros();
  
  // Debounce (çok hızlı okuma engelle)
  if (simdi - sonEncoderZamani < 1000) return; // 1ms
  
  sonEncoderZamani = simdi;
  
  // Yeni durum oku
  uint8_t MSB = digitalRead(clk);
  uint8_t LSB = digitalRead(dt);
  uint8_t yeniDurum = (MSB << 1) | LSB;
  
  // Lookup table ile yön belirleme
  uint8_t index = (encoderSonDurum << 2) | yeniDurum;
  int8_t delta = pgm_read_byte(&ENCODER_TABLE[index]);
  
  encoderDelta += delta;
  encoderSonDurum = yeniDurum;
  
  if (delta != 0) {
    encoderHareketSayisi++;
  }
}

void butonISR() {
  unsigned long simdi = millis();
  
  // Debounce
  if (simdi - butonBasilmaZamani < 50) return;
  
  butonBasilmaZamani = simdi;
  butonBasildi = true;
}

// ===========================================================================
// ======================== Polling Yöntemi (Mevcut Pinler için) =============
// ===========================================================================

void encoderOku() {
  static int encoderPos = 0;
  unsigned long simdi = millis();
  
  // Encoder durumu oku
  uint8_t MSB = digitalRead(clk);
  uint8_t LSB = digitalRead(dt);
  uint8_t yeniDurum = (MSB << 1) | LSB;
  
  // Durum değiştiyse
  if (yeniDurum != encoderSonDurum) {
    // Debounce kontrolü
    if (simdi - sonEncoderZamani < 2) {
      return;
    }
    
    sonEncoderZamani = simdi;
    
    // Lookup table kullanarak yön belirleme
    uint8_t index = (encoderSonDurum << 2) | yeniDurum;
    int8_t delta = pgm_read_byte(&ENCODER_TABLE[index]);
    
    encoderPos += delta;
    encoderSonDurum = yeniDurum;
    
    if (delta != 0) {
      encoderHareketSayisi++;
    }
    
    // Encoder hızını hesapla
    float hiz = encoderHiziniHesapla();
    
    // Akselerasyon uygula (hızlı döndürünce daha fazla adım atlar)
    uint8_t adimSayisi = encoderAkselerasyonu(hiz);
    
    // Belirlenen adım sayısına ulaşıldığında işlem yap
    int esikDeger = 4 / adimSayisi;
    
    if (encoderPos >= esikDeger) {
      for (uint8_t i = 0; i < adimSayisi; i++) {
        secimArttir();
      }
      encoderPos = 0;
      
      if (etkilesim) {
        sonEtkilesimZamani = millis();
      }
    }
    else if (encoderPos <= -esikDeger) {
      for (uint8_t i = 0; i < adimSayisi; i++) {
        secimAzalt();
      }
      encoderPos = 0;
      
      if (etkilesim) {
        sonEtkilesimZamani = millis();
      }
    }
  }
}

// ===========================================================================
// ======================== Yardımcı Fonksiyonlar ============================
// ===========================================================================

int encoderPozisyonOku() {
  return encoderDelta;
}

void encoderPozisyonSifirla() {
  encoderDelta = 0;
}

float encoderHiziniHesapla() {
  unsigned long simdi = millis();
  static unsigned long oncekiHareketSayisi = 0;
  
  // Her 100ms'de bir hız hesapla
  if (simdi - sonHizHesaplama >= 100) {
    unsigned long hareketFarki = encoderHareketSayisi - oncekiHareketSayisi;
    encoderHizi = (float)hareketFarki / 0.1; // Hareket/saniye
    
    oncekiHareketSayisi = encoderHareketSayisi;
    sonHizHesaplama = simdi;
  }
  
  return encoderHizi;
}

uint8_t encoderAkselerasyonu(float hiz) {
  // Hıza göre adım sayısı belirleme
  if (hiz < 5) {
    return 1;  // Yavaş: Normal
  } else if (hiz < 15) {
    return 2;  // Orta: 2x hızlı
  } else if (hiz < 30) {
    return 4;  // Hızlı: 4x hızlı
  } else {
    return 8;  // Çok hızlı: 8x hızlı
  }
}

// ===========================================================================
// ======================== Seçim Fonksiyonları ==============================
// ===========================================================================

void secimArttir() {
  if (ekranDurumu == 2 || ekranDurumu == 4 || ekranDurumu == 5) {
    if (secim < 3) secim++;
    else secim = 0;
  }
  else if (ekranDurumu == 3 && !editModu && !kayitBtn) {
    if (secim < 6) secim++;
    else secim = 0;
  }
  else if (ekranDurumu == 3 && !editModu && kayitBtn) {
    if (secim < 7) secim++;
    else secim = 0;
  }
  else if (ekranDurumu == 3 && editModu) {
    if (secim >= 0 && secim <= 2) {
      if (deger < maxDeger) deger++;
      else deger = 0;
    }
    else if (secim >= 3 && secim <= 5) {
      if (deger < maxDeger) deger++;
      else deger = 1;
    }
  }
  else if ((ekranDurumu == 6 && !kayitBtn && !editModu) || ekranDurumu == 7) {
    if (secim < 1) secim++;
    else secim = 0;
  }
  else if (ekranDurumu == 6 && kayitBtn && !editModu) {
    if (secim < 2) secim++;
    else secim = 0;
  }
  else if (ekranDurumu == 6 && editModu) {
    // Edit modunda hız kontrolü
    float hiz = encoderHiziniHesapla();
    if (hiz < 10) {
      ayar += 0.1;  // Yavaş: 0.1 artır
    } else if (hiz < 20) {
      ayar += 0.5;  // Orta: 0.5 artır
    } else {
      ayar += 1.0;  // Hızlı: 1.0 artır
    }
    
    // Sınır kontrolü
    if (ayar > 100) ayar = 100;
  }
}

void secimAzalt() {
  if (ekranDurumu == 2 || ekranDurumu == 4 || ekranDurumu == 5) {
    if (secim > 0) secim--;
    else secim = 3;
  }
  else if (ekranDurumu == 3 && !editModu && !kayitBtn) {
    if (secim > 0) secim--;
    else secim = 6;
  }
  else if (ekranDurumu == 3 && !editModu && kayitBtn) {
    if (secim > 0) secim--;
    else secim = 7;
  }
  else if (ekranDurumu == 3 && editModu) {
    if (secim >= 0 && secim <= 2) {
      if (deger > 0) deger--;
      else deger = maxDeger;
    }
    else if (secim >= 3 && secim <= 5) {
      if (deger > 1) deger--;
      else deger = maxDeger;
    }
  }
  else if ((ekranDurumu == 6 && !kayitBtn && !editModu) || ekranDurumu == 7) {
    if (secim > 0) secim--;
    else secim = 1;
  }
  else if (ekranDurumu == 6 && kayitBtn && !editModu) {
    if (secim > 0) secim--;
    else secim = 2;
  }
  else if (ekranDurumu == 6 && editModu) {
    // Edit modunda hız kontrolü
    float hiz = encoderHiziniHesapla();
    if (hiz < 10) {
      ayar -= 0.1;  // Yavaş: 0.1 azalt
    } else if (hiz < 20) {
      ayar -= 0.5;  // Orta: 0.5 azalt
    } else {
      ayar -= 1.0;  // Hızlı: 1.0 azalt
    }
    
    // Sınır kontrolü
    if (ayar < 0) ayar = 0;
  }
}

// ===========================================================================
// ======================== Zaman Aşımı ======================================
// ===========================================================================

void checkTimeout() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - sonEtkilesimZamani >= ZAMAN_ASIMI_SURESI &&
      etkilesim &&
      !zamanAsimiAktif &&
      ekranDurumu != 1) {
    
    ekranTemizle = true;
    etkilesim = false;
    secim = 0;
    ekranDurumu = 1;
    ayarModu = false;
    editModu = false;
    kayitBtn = false;
    zamanAsimiAktif = false;
  }
}

// ===========================================================================
// ======================== Buton İşleyici ===================================
// ===========================================================================

void handleButton() {
  unsigned long currentMillis = millis();
  
  // ISR modunda çalışıyorsa
  if (butonBasildi) {
    butonBasildi = false;
    // Buton işlemleri...
    etkilesim = true;
    sonEtkilesimZamani = currentMillis;
  }
  
  // Polling modunda çalışıyorsa (mevcut)
  bool butonAnlikDurum = digitalRead(btn);
  
  if (butonAnlikDurum == LOW && currentMillis - sonBasilma >= BUTON_DEBOUNCE_SURESI) {
    sonBasilma = currentMillis;
    etkilesim = true;
    sonEtkilesimZamani = currentMillis;
    
    // [Önceki handleButton kodunun devamı buraya gelecek]
    // Kod çok uzun olduğu için kısaltıldı, orijinal kodunuzu kullanın
    
    // Ana ekrandan menüye geçiş
    if (ekranDurumu == 1) {
      ekranDurumu = 2;
      ekranTemizle = true;
    }
    // ... (geri kalan buton işlemleri)
  }
}

// NOT: handleButton fonksiyonunun tam hali için mevcut kodunuzu kullanın
// Yukarıda sadece iyileştirmeleri gösterdim