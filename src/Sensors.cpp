#include "Sensors.h"
#include "Globals.h"
#include "Pins.h"
#include "Configuration.h"

void setupSensors() {
  DS18B20.begin();
  DS18B20.getAddress(DS18B20adres, 0);
  DS18B20.setResolution(DS18B20adres, 12);
  dht.begin();
}

void ortamSensorOku() {
  readDoorSensor();
  readTemperatureSensor();
  readHumiditySensor();
}

void readTemperatureSensor() {
  unsigned long simdi = millis();
  
  // DS18B20 ölçüm isteği
  if (!okumaHazir && simdi - sonOkumaZamani >= DS18B20_OKUMA_SURESI) {
    DS18B20.requestTemperatures();
    sonOkumaZamani = simdi;
    okumaHazir = true;
  }
  
  // DS18B20 ölçüm sonucu
  if (okumaHazir && simdi - sonOkumaZamani >= DS18B20_BEKLEME_SURESI) {
    santigrat = DS18B20.getTempC(DS18B20adres);
    okumaHazir = false;
  }
}

void readHumiditySensor() {
  unsigned long simdi = millis();
  
  // DHT22 ölçümü (her 2 saniyede bir)
  if (simdi - sonDHTOkumaZamani >= DHT_OKUMA_SURESI) {
    nem = dht.readHumidity();
    
    // NaN tespit ederse yeniden dene
    if (isnan(nem)) {
      resetHumiditySensor();
    }
    
    sonDHTOkumaZamani = simdi;
  }
}

void readDoorSensor() {
  unsigned long simdi = millis();
  static unsigned long sonKapakKontrolu = 0;
  
  // Kapak sensörü kontrolünü seyrekleştir
  if (simdi - sonKapakKontrolu >= KAPAK_KONTROL_SURESI) {
    int sensorDeger = analogRead(kapakSensor);
    
    if (sensorDeger <= 200) {
      bitSet(flags, FLAG_KAPAK_DURUMU);
    } 
    else if (sensorDeger > 200 && makineAktif) {
      bitClear(flags, FLAG_KAPAK_DURUMU);
      
      // Bildirim güncellemesini de seyrekleştir
      static unsigned long sonBildirimZamani = 0;
      if (simdi - sonBildirimZamani >= 2000) {
        bildirim = 3;
        sonBildirimZamani = simdi;
      }
    }
    sonKapakKontrolu = simdi;
  }
}

void resetHumiditySensor() {
  if (!nemResetBekleme) {
    // Reset işlemi başlat
    digitalWrite(nemSensorYenidenBaslatma, LOW);
    nemResetStart = millis();
    nemResetBekleme = true;
  }
  
  // Bekleme süresi doldu mu?
  if (nemResetBekleme && millis() - nemResetStart >= 1000) {
    digitalWrite(nemSensorYenidenBaslatma, HIGH);
    nem = dht.readHumidity(); // Tekrar oku
    nemResetBekleme = false;
  }
}