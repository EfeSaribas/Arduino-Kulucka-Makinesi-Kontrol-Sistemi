# Kuluçka Makinesi Kontrol Sistemi

Arduino tabanlı, PlatformIO ile geliştirilmiş profesyonel (Şu anlık sadece "**Tavuk Yumurtaları**"  üzerinde çalışan) kuluçka makinesi kontrol sistemi.

## 🛠️ Donanım Listesi

### Ana Kontrol Birimi
- **Arduino UNO** (veya uyumlu klon)
- **20x4 I2C LCD Ekran** (0x27 adres)
- **Rotary Encoder** (buton ile)
- **DS3231 RTC Modülü** (saat/takvim)

### Sensörler
- **DS18B20** - Dijital sıcaklık sensörü (OneWire)
- **DHT22** - Nem sensörü (sadece nem ölçümü için)
- **QRD1114** - Kızılötesi optik sensör (kapak algılama)

### Güç Kaynağı ve Regülatörler
- **12V 10A Trafo** (220V → 12V dönüştürücü)
- **DC 0-50V 4A Voltaj Güç Filtresi** (parazit önleyici)
- **LM2596-ADJ Voltaj Regülatör x2**
  - **1. Regülatör**: Sistem ve sensörler için 5V
  - **2. Regülatör**: Röleler ve güç tüketen elemanlar için 5V

### Güç Kontrol Elemanları
- **4 Adet 5V Röle Modülü**
  - 2 Röle: Faz kontrol (220V)
  - 2 Röle: Nötr kontrol (220V)
- **BC547 NPN Transistör** (röle sürücü olarak)
- **L298N Motor Sürücü Modülü** (sirkülasyon fanı için PWM kontrol)

### Isıtma ve Havalandırma
- **220V Rezistans Kablo** (2 metre, iç ısıtma)
- **12V 12x12cm Fan** (iç hava sirkülasyonu, PWM kontrollü)
- **5V 4x4cm Egzoz Fanı** (nem tahliye, transistör kontrollü)
- **56'lık Viyol + 220V Motor** (3-4 RPM, yumurta çevirme)
  - 50Hz → 3 RPM
  - 60Hz → 4 RPM

### Nemlendirme
- **Piezo Nemlendirici** (ultrasonik buharlaştırıcı, transistör kontrollü)

---

## 📁 Proje Yapısı

```
incubator-controller/
├── platformio.ini         # PlatformIO yapılandırma dosyası
├── include/               # Header dosyaları
│   ├── Configuration.h    # Tüm yapılandırma sabitleri
│   ├── Globals.h          # Global değişkenler ve objeler
│   ├── Pins.h             # Pin tanımlamaları
│   ├── Sensors.h          # Sensör fonksiyonları
│   ├── LCD.h              # LCD ekran fonksiyonları
│   ├── Incubator.h        # Kuluçka kontrol fonksiyonları
│   ├── RTC.h              # RTC modül fonksiyonları
│   ├── EEPROM_Manager.h   # EEPROM yönetimi
│   ├── Encoder.h          # Rotary encoder ve buton
│   ├── Safety.h           # Güvenlik kontrolleri
│   └── SerialDebug.h      # Seri port debug
├── src/                   # Kaynak dosyaları
│   ├── main.cpp           # Ana program
│   ├── Globals.cpp        # Global değişken tanımlamaları
│   ├── Pins.cpp           # Pin başlatma
│   ├── Sensors.cpp        # Sensör okuma
│   ├── LCD.cpp            # LCD ekran yönetimi
│   ├── Incubator.cpp      # Kuluçka süreci
│   ├── RTC.cpp            # Saat/takvim
│   ├── EEPROM_Manager.cpp # EEPROM işlemleri
│   ├── Encoder.cpp        # Kullanıcı arayüzü
│   ├── Safety.cpp         # Güvenlik sistemleri
│   └── SerialDebug.cpp    # Debug çıktıları
├── LICENSE                # Özel lisans
├── CHANGES.md             # Değişiklik bildirimi şablonu
└── README.md              # Bu dosya
```

## 🎯 Modül Açıklamaları

### Configuration.h
- Tüm pin tanımlamaları
- Zamanlama sabitleri
- Güvenlik parametreleri
- EEPROM adres haritası
- Kuluçka parametreleri

### Sensors Modülü
- **DS18B20**: Sıcaklık sensörü
- **DHT22**: Nem sensörü
- **QRD1114**: Kapak sensörü (analog okuma)
- Otomatik sensör reset mekanizması

### LCD Modülü
- 20x4 I2C LCD kontrol
- Özel karakter tanımları
- Dinamik menü sistemi
- Cursor yönetimi

### Incubator Modülü
- Gün hesaplama
- Sıcaklık/nem kontrolü
- Viyol (çevirme) yönetimi
- Fan kontrol sistemleri

### Safety Modülü
- Aşırı sıcaklık koruması (>45°C)
- Sensör hata tespiti
- Isıtıcı maksimum süre limiti (2 dakika)
- Kapak açık algılama
- Acil durdurma mekanizması

### EEPROM Manager
- Parametrelerin kalıcı saklanması
- Float dizi okuma/yazma
- Gün sayısı takibi
- Başlangıç tarih/saat kaydı

## 🔌 Detaylı Pin Bağlantıları

### Arduino UNO Pin Kullanımı

| Arduino Pin | Bağlantı | Fonksiyon | Tip | Notlar |
|-------------|----------|-----------|-----|--------|
| **A0** | Encoder Butonu | Menü navigasyonu | Dijital Giriş | INPUT_PULLUP |
| **A1** | Encoder DT | Rotary encoder data | Dijital Giriş | INPUT_PULLUP |
| **A2** | Encoder CLK | Rotary encoder clock | Dijital Giriş | INPUT_PULLUP |
| **A3** | QRD1114 OUT | Kapak sensörü | Analog Giriş | 0-1023 aralık |
| **A4** | SDA (I2C) | LCD + RTC iletişim | I2C | Pull-up dirençli |
| **A5** | SCL (I2C) | LCD + RTC iletişim | I2C | Pull-up dirençli |
| **D5** | L298N IN1/IN2 | Sirkülasyon fanı PWM | PWM Çıkış | 12V fan kontrolü |
| **D6** | BC547 Base | Viyol motoru rölesi | Dijital Çıkış | 220V motor kontrolü (Aynı Anda 2 Röle için)|
| **D7** | DHT22 VCC | DHT22 reset pini | Dijital Çıkış | Sensör reset için |
| **D8** | BC547 Base | Egzoz fanı transistör | Dijital Çıkış | 5V fan kontrolü |
| **D9** | DHT22 DATA | Nem sensörü data | Dijital I/O | 10kΩ pull-up |
| **D10** | DS18B20 DATA | Sıcaklık sensörü | OneWire | 4.7kΩ pull-up |
| **D11** | BC547 Base | Rezistans rölesi | Dijital Çıkış | 220V ısıtıcı (Aynı Anda 2 Röle için)|
| **D12** | BC547 Base | Piezo nemlendirici | Dijital Çıkış | Aktif LOW |

### Röle Bağlantı Şeması

```
220V FUŞ (Rezistans) → RÖLEan 1 (Faz)  ← D11 (Arduino)
220V NÖTR (Rezistans) → RÖLE 2 (Nötr) ← BC547 ← GND

220V FAZ (Viyol)      → RÖLE 3 (Faz)  ← D6 (Arduino)
220V NÖTR (Viyol)     → RÖLE 4 (Nötr) ← BC547 ← GND
```

### Güç Kaynağı Şeması

```
220V AC
  ↓
12V 10A Trafo
  ↓
Parazit Filtresi
  ↓
  ├→ LM2596 Regülatör #1 → 5V → Arduino + Sensörler + LCD + RTC
  │
  └→ LM2596 Regülatör #2 → 5V → Röleler + Piezo + Egzoz Fanı
  
  12V → L298N → Sirkülasyon Fanı (PWM kontrollü)
```

### Transistör Sürücü Şemaları

#### BC547 Sürücü (Röleler için)
```
Arduino Pin → 1kΩ → BC547 Base
                     ↓ Collector → Röle Bobini (+)
                     ↓ Emitter → GND
                     
Röle Bobini (-) → 5V
1N4007 Diyot → Röle bobinine paralel (flyback)
```

#### L298N Bağlantısı (Sirkülasyon Fanı)
```
Arduino D5 (PWM) → L298N ENA
L298N IN1 → 5V
L298N IN2 → GND
L298N OUT1 → Fan (+)
L298N OUT2 → Fan (-)
L298N VCC → 12V
L298N GND → Arduino GND (ortak toprak)
```

### I2C Cihaz Adresleri

| Cihaz | I2C Adresi | SDA | SCL |
|-------|-----------|-----|-----|
| LCD 20x4 | 0x27 | A4 | A5 |
| DS3231 RTC | 0x68 | A4 | A5 |

### OneWire Cihazlar

| Cihaz | Pin | Pull-up |
|-------|-----|---------|
| DS18B20 | D10 | 4.7kΩ → 5V |

## ⚡ Güç Tüketimi

| Bileşen | Voltaj | Akım | Güç |
|---------|--------|------|-----|
| Arduino UNO | 5V | ~50mA | 0.25W |
| LCD 20x4 | 5V | ~20mA | 0.1W |
| DS3231 | 5V | ~0.1mA | 0.0005W |
| DS18B20 | 5V | ~1mA | 0.005W |
| DHT22 | 5V | ~2.5mA | 0.0125W |
| QRD1114 | 5V | ~20mA | 0.1W |
| Röleler (4x) | 5V | ~280mA | 1.4W |
| Sirkülasyon Fanı | 12V | ~0.5A | 6W |
| Egzoz Fanı | 5V | ~0.2A | 1W |
| Piezo Nemlendirici | 5V | ~0.3A | 1.5W |
| **Toplam (5V)** | - | **~0.6A** | **3W** |
| **Toplam (12V)** | - | **~0.5A** | **6W** |
| **220V (Rezistans)** | 220V | ~0.45A | **100W** |
| **220V (Viyol)** | 220V | ~0.1A | **22W** |

**Önerilen Güç Kaynakları:**
- LM2596 #1: 5V 3A (sistem için)
- LM2596 #2: 5V 3A (güç elemanları için)
- 12V Trafo: 10A (yeterli)
- **NOT:** ⚠️Güçlü bir parazit önleyici ile tek bir LM2596 kullanarak sistem teoride çalıştırılabilir ama denenmedi⚠️

## 📊 Kuluçka Parametreleri

| Gün Aralığı | Sıcaklık | Nem | Çevirme |
|-------------|----------|-----|---------|
| 1-7 | 37.7°C ±0.3 | 55% ±5 | Her 4 saatte |
| 8-14 | 37.7°C ±0.3 | 50% ±5 | Her 4 saatte |
| 15-18 | 37.7°C ±0.3 | 50% ±5 | Her 4 saatte |
| 19+ | 37.0°C ±0.3 | 70% ±5 | Yok |

## 🛠️ Kurulum

### Gereksinimler
- PlatformIO CLI veya IDE
- Arduino UNO (veya uyumlu)
- Gerekli kütüphaneler (otomatik indirilir):
  - DHT sensor library
  - RTClib
  - DallasTemperature
  - OneWire
  - LiquidCrystal_I2C

### Donanım Kurulumu

1. **Güç Kaynağını Bağlayın**
   - 220V → 12V trafo
   - Parazit filtresinden geçirin
   - İki LM2596 regülatörü ayarlayın (5V çıkış)

2. **Arduino Bağlantıları**
   - Pin tablosuna göre tüm sensörleri bağlayın
   - I2C cihazları paralel bağlayın (LCD + RTC)
   - Ortak toprak (GND) bağlantısını unutmayın

3. **Röle Bağlantıları**
   - BC547 transistörler üzerinden kontrol edin
   - Flyback diyotlarını (1N4007) ekleyin
   - 220V bağlantıları güvenli şekilde yapın

4. **Güvenlik Önlemleri**
   - 220V bağlantıları için izolasyon yapın
   - Parazitonleyiciyi mutlaka kullanın
   - Topraklama bağlantısı yapın

### Yazılım Kurulumu

```bash
# PlatformIO CLI ile
pio run -t upload

# Veya PlatformIO IDE'de
# Upload butonuna basın
```

### İlk Çalıştırma

1. **RTC Ayarı**: Saat/Tarih menüsünden zamanı ayarlayın
2. **Parametre Kontrolü**: Kuluçka ayarlarını kontrol edin
3. **Test Modu**: Boş çalıştırarak sistemleri test edin
4. **Başlat**: Yumurtaları yerleştirin ve işlemi başlatın

## 🔒 Güvenlik Özellikleri

1. **Watchdog Timer**: 8 saniye timeout
2. **Aşırı Sıcaklık**: >45°C'de acil durdurma
3. **Sensör Doğrulama**: 3 hatalı okumada durdurma
4. **Isıtıcı Limiti**: Maksimum 10 dakika sürekli çalışma
5. **Kapak Güvenliği**: Kapak açıkken tüm sistemler devre dışı
6. **Parazit Filtresi**: Elektriksel gürültü koruması
7. **İzolasyon**: 220V ile 5V arasında optik izolasyon (röleler)

## 📝 EEPROM Kullanımı

| Adres | Veri | Boyut |
|-------|------|-------|
| 0 | Gün sayısı | 1 byte |
| 1-2 | Başlangıç gün/ay | 2 byte |
| 3-18 | Hedef sıcaklıklar | 16 byte |
| 19-34 | Hedef nemler | 16 byte |
| 35-50 | Sıcaklık toleransları | 16 byte |
| 51-66 | Nem toleransları | 16 byte |
| 67-70 | Başlangıç saat/dakika/saniye/yıl | 4 byte |
| 71-72 | Viyol zamanı | 2 byte |

## 🎮 Kullanım

### Menü Sistemi
1. **Ana Ekran**: Sıcaklık, nem, tarih, gün sayısı
2. **Ana Menü**: 
   - Başlat/Durdur
   - Kuluçka Ayarları
   - Saat/Tarih
3. **Kuluçka Ayarları**: 4 farklı gün aralığı için parametreler
4. **Saat/Tarih**: RTC ayarlama

### Encoder Kullanımı
- **Döndür**: Menüde gezinme / Değer değiştirme
- **Bas**: Seçim yapma / Edit moduna girme

## 🐛 Hata Ayıklama

Serial monitörde her saniye sistem durumu yazdırılır:
- Makine durumu
- Sensör değerleri
- Bayrak durumları (ısıtıcı, fan, vb.)
- Tarih/saat bilgisi
- Kuluçka bilgileri

## ⚠️ Önemli Uyarılar

1. **220V Tehlikeli**: Elektrik işlerini bilgili kişilere yaptırın
2. **Topraklama**: Mutlaka topraklama yapın
3. **İzolasyon**: Rölelerin 220V tarafını izole edin
4. **Nem + Elektrik**: Su ile elektrik temasını önleyin
5. **Ventilasyon**: Elektronik kutuyu havalandırın
6. **Yedek Güç**: Kesinti anında UPS kullanın

## 📄 Lisans

Bu proje **özel bir lisans** altında dağıtılmaktadır. Detaylar için [LICENSE](LICENSE) dosyasına bakın.

### Özet:
- ✅ **Kişisel kullanım**: Tamamen ücretsiz
- ✅ **Eğitim/Araştırma**: Serbestçe kullanılabilir
- ✅ **Açık kaynak projeler**: Kullanılabilir
- ⚠️ **Ticari kullanım**: Özel izin gerektirir (küçük işletmeler için uygun fiyat)
- ⚠️ **Değişiklik bildirim zorunluluğu**: Değişikliklerinizi bildirmelisiniz

### Değişiklik Yapmak İstiyorsanız:
1. Değişikliklerinizi yapın
2. `CHANGES.md` dosyası oluşturun ([şablon](CHANGES.md))
3. Pull request gönderin VEYA e-posta ile bildirin
4. Aynı lisansı koruyun

### Ticari Kullanım:
Ticari kullanım için lütfen iletişime geçin:
- **E-posta**: efesaribas909@gmail.com
- **Konu**: Ticari Lisans Talebi
- Küçük işletmeler için uygun fiyatlandırma mevcuttur

### İletişim:
- **GitHub**: https://github.com/EfeSaribas
- **E-posta**: efesaribas909@gmail.com

---

**Geliştirici**: Efe Sarıbaş
**Son Güncelleme**: Ocak 2025
**Versiyon**: 2.0 (Modüler PlatformIO)
**Lisans**: Özel - İzinli Ticari Kullanım

## 👨‍💻 Geliştirici Notları

### Kod Standartları
- Türkçe değişken/fonksiyon isimlendirme (mevcut kod ile uyumluluk için)
- Modüler yapı (her modül kendi başlık ve kaynak dosyasında)
- PROGMEM kullanımı (özel karakterler için)
- Debounce mekanizmaları (buton ve encoder)
- Non-blocking kod yapısı (millis() bazlı)

### Performans Optimizasyonları
- EEPROM yazımı minimize edilmiş (5 dakikada bir + inaktif durum kontrolü)
- Sensör okumaları seyrekleştirilmiş
- LCD güncellemeleri 200ms aralıklarla
- Encoder okuma 5ms debounce

### Gelecek Geliştirmeler
- [ ] WiFi modülü entegrasyonu
- [ ] SD kart veri loglama
- [ ] Mobil uygulama desteği
- [ ] Grafiksel LCD desteği
- [ ] Çoklu dil desteği
- [ ] PID kontrol algoritması

## 🔧 Yapılandırma

Tüm yapılandırma `include/Configuration.h` dosyasından yapılabilir:

```cpp
// Pin değiştirme örneği
#define PIN_VIYOL 6  // Farklı bir pin kullanmak için

// Zamanlama değiştirme
#define EKRAN_GUNCELLEME_SURESI 200  // ms

// Güvenlik parametreleri
#define KRITIK_SICAKLIK 45.0  // °C

// Kuluçka parametreleri
#define GUN_1_7_SICAKLIK 37.5
#define GUN_1_7_NEM 55
```

## 📊 Bayrak (Flag) Sistemi

Sistem durumları tek bir byte değişkende bit bazlı saklanır:

| Bit | Fonksiyon |
|-----|-----------|
| 0 | Isıtıcı durumu |
| 1 | Nemlendirici durumu |
| 2 | Viyol motoru durumu |
| 3 | Sirkülasyon fanı durumu |
| 4 | Egzoz fanı durumu |
| 5 | Kapak durumu (1=Kapalı, 0=Açık) |

Kullanım:
```cpp
bitSet(flags, FLAG_ISITICI);    // Isıtıcıyı aç
bitClear(flags, FLAG_ISITICI);  // Isıtıcıyı kapat
bitRead(flags, FLAG_ISITICI);   // Isıtıcı durumunu oku
```

## 🌡️ Sensör Özellikleri

### DS18B20 (Sıcaklık)
- Çözünürlük: 12-bit (0.0625°C)
- Okuma süresi: 750ms
- Aralık: -55°C ile +125°C
- Doğruluk: ±0.5°C

### DHT22 (Nem)
- Nem aralığı: 0-100%
- Nem doğruluğu: ±2%
- Okuma süresi: 2 saniye
- Otomatik reset özelliği

## 🔄 Viyol (Çevirme) Sistemi

- **Periyot**: 4 saat (240 dakika)
- **Süre**: 12 saniye
- **Aktif günler**: 1-18. günler
- **Otomatik yeniden planlama**: Güç kesintisi sonrası

Algoritma:
1. EEPROM'dan son çevirme zamanı okunur
2. Geçmiş zamansa, bir sonraki zaman hesaplanır
3. Planlanan zamana gelince motor çalışır
4. 12 saniye sonra motor durur
5. Sonraki çevirme zamanı kaydedilir

## 🛡️ Güvenlik Mekanizmaları

### Çok Katmanlı Koruma

1. **Watchdog Timer**
   - 8 saniyede sistem yanıt vermezse reset
   - Sürekli beslenmeli (wdt_reset)

2. **Sıcaklık Limitleri**
   - Kritik: >45°C → Acil durdurma
   - Doğrulama: -20°C ile 60°C arası
   - Güvenlik kesimi: 44°C'de ısıtıcı zorla kapatılır

3. **Zaman Limitleri**
   - Isıtıcı maksimum 10 dakika sürekli çalışabilir
   - Nemlendirici maksimum 20 saniye sürekli çalışabilir

4. **Kapak Güvenliği**
   - 3 ardışık okuma ile doğrulama
   - Kapak açıkken tüm sistemler devre dışı
   - Otomatik bildirim

## 📈 Sistem Akış Diyagramı

```
Setup
  ↓
Watchdog Enable
  ↓
Pin Başlatma
  ↓
LCD Başlatma
  ↓
Sensör Başlatma
  ↓
RTC Başlatma
  ↓
EEPROM Yükleme
  ↓
Loop ←─────────────┐
  ↓                │
Watchdog Reset     │
  ↓                │
Güvenlik Kontrol   │
  ↓                │
Sensör Okuma       │
  ↓                │
Kuluçka Kontrolü   │
  ↓                │
Ortam Düzenleyici  │
  ↓                │
Kullanıcı Arayüzü  │
  ↓                │
LCD Güncelleme     │
  ↓                │
Serial Debug       │
  └────────────────┘
```

## 🎛️ Menü Yapısı

```
Ana Ekran
  ├── Encoder Bas → Ana Menü
  │   ├── Başlat/Durdur
  │   │   └── Onay Ekranı
  │   ├── Kuluçka Ayar
  │   │   ├── Gün 1-7
  │   │   ├── Gün 8-14
  │   │   ├── Gün 15-17
  │   │   └── Gün 18-21+
  │   │       ├── Hedef Sıcaklık
  │   │       ├── Hedef Nem
  │   │       ├── Sıcaklık Tolerans
  │   │       └── Nem Tolerans
  │   ├── Saat/Tarih
  │   │   ├── Saat
  │   │   ├── Dakika
  │   │   ├── Saniye
  │   │   ├── Gün
  │   │   ├── Ay
  │   │   ├── Yıl
  │   │   └── Kaydet
  │   └── Geri Dön
  └── Zaman Aşımı (10s) → Ana Ekran
```

## 💾 Bellek Kullanımı

- **Program**: ~25KB (Arduino Mega için ideal)
- **RAM**: ~860B global değişkenler
- **EEPROM**: 73 byte kullanılıyor

## 🚀 Hızlı Başlangıç

1. **Donanımı bağlayın** (Pin tablosuna göre)
2. **PlatformIO yükleyin**
3. **Projeyi açın**
4. **Kartı seçin** (Mega 2560 / Uno)
5. **Upload** butonuna basın
6. **İlk açılış**: Tarih/saat ayarlayın
7. **Parametre kontrolü**: Kuluçka ayarlarını kontrol edin
8. **Başlat**: Ana menüden işlemi başlatın

## 📞 Destek

Sorun bildirimi için lütfen detaylı açıklama ve serial monitor çıktısı ekleyin.

---

**Son Güncelleme**: 2025
**Versiyon**: 2.0 (Modüler PlatformIO)
