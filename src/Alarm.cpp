#include "Globals.h"
#include "Pins.h"
#include "Configuration.h"

void alarm()
{
    if(!bitRead(flags, FLAG_ALARM_DURUMU)) return;
    
    static unsigned long oncekiZaman = 0;
    static int durum = 0;
    static int sayac = 0;

    unsigned long simdi = millis();

    switch (durum)
    {
    case 0: // buzzer aç
        tone(buzzer, BUZZER_FREKANS);
        oncekiZaman = simdi;
        durum = 1;
        break;

    case 1: // açık bekle
        if (simdi - oncekiZaman >= BUZZER_ACIK_SURESI)
        {
            noTone(buzzer);
            oncekiZaman = simdi;
            durum = 2;
        }
        break;

    case 2: // sessiz bekle
        if (simdi - oncekiZaman >= BUZZER_KAPALI_SURESI)
        {
            sayac++;
            if (sayac < 3)
            {
                durum = 0; // tekrar ötsün
            }
            else
            {
                sayac = 0;
                oncekiZaman = simdi;
                durum = 3; // 3 kere bitti → bekle
            }
        }
        break;

    case 3: // bekleme
        if (simdi - oncekiZaman >= BUZZER_BEKLEME_SURESI)
        {
            durum = 0; // tekrar başa dön
        }
        break;
    }
}
