#include <Adafruit_ADS1X15.h>
#include <Adafruit_ADS1X15.h>
#include <Wire.h>
#include <SD.h>

Adafruit_ADS1X15 ads;  // ADC 16 bits
const int chipSelect = 10;  // Broche CS pour le module SD
const int soundThreshold = 500;  // Ajustez selon vos tests

void setup() {   
  Serial.begin(9600);
  
  // Initialisation ADC
    ads.begin();

   SD.begin();

  Serial.println("Système prêt.");
}

void loop() {
  int16_t soundLevel = ads.readADC_SingleEnded(0);  // Lecture du canal 0 (microphone)
  Serial.print("Niveau sonore : ");
  Serial.println(soundLevel);

  if (soundLevel >= soundThreshold) {
    Serial.println("Voix élevée détectée. Enregistrement...");

    // Enregistrement des données
    File dataFile = SD.open("audio_data.txt", FILE_WRITE);
    
    if (dataFile) {
      dataFile.println(soundLevel);
      dataFile.close();
      Serial.println("Données enregistrées sur la carte SD.");
    } else {
      Serial.println("Erreur d'écriture sur la carte SD.");
    }

    // Transmission vers Qt
    Serial.print("DATA:");
    Serial.println(soundLevel);
    delay(1000);  // Éviter la surcharge
  }else{
    Serial.println("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

    }
}
