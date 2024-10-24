// Inclusion des bibliothèques nécessaires pour le fonctionnement du module SIM808 et des capteurs
#include <SoftwareSerial.h>  // Pour la communication série avec le module SIM808
#define TEMP_PIN A0           // Broche analogique pour le capteur de température
#define AD8232_PIN D2         // Broche digitale pour le capteur de rythme cardiaque AD8232

SoftwareSerial sim808(7, 8);  // Connexion du module SIM808 aux broches 7 et 8 du WeMos D1 Mini

int temperature;              // Variable pour stocker la température
int heartRate;                // Variable pour stocker les données du capteur cardiaque

void setup() {
  Serial.begin(9600);         // Démarrage de la communication série pour le débogage
  sim808.begin(9600);         // Démarrage de la communication série avec le module SIM808
  
  pinMode(AD8232_PIN, INPUT); // Déclaration du capteur AD8232 comme entrée
}

void loop() {
  // Lecture des données de température depuis le capteur LM35
  temperature = analogRead(TEMP_PIN); 
  float voltage = temperature * (5.0 / 1023.0);  // Conversion de la lecture en tension (0 - 5V)
  float tempC = voltage * 100;  // Conversion de la tension en degré Celsius

  // Lecture du signal du capteur de rythme cardiaque AD8232
  heartRate = digitalRead(AD8232_PIN);

  // Affichage des valeurs lues sur le moniteur série pour débogage
  Serial.print("Température: ");
  Serial.print(tempC);
  Serial.println(" °C");

  Serial.print("Rythme cardiaque détecté: ");
  if (heartRate == 1) {
    Serial.println("BPM détecté");
  } else {
    Serial.println("Pas de signal");
  }

  // Transmission des données par SMS via le module SIM808
  sendDataViaSIM808(tempC, heartRate);

  delay(2000);  // Pause de 2 secondes avant la prochaine lecture
}

// Fonction pour envoyer les données via le module SIM808
void sendDataViaSIM808(float temperature, int heartRate) {
  sim808.println("AT+CMGF=1");    // Commande pour passer en mode SMS
  delay(100);
  
  sim808.println("AT+CMGS=\"+212XXXXXXXXX\""); // Numéro de téléphone où envoyer les données
  delay(100);

  // Message à envoyer par SMS
  sim808.print("Température: ");
  sim808.print(temperature);
  sim808.println(" °C");

  sim808.print("Rythme cardiaque: ");
  if (heartRate == 1) {
    sim808.println("Signal cardiaque détecté.");
  } else {
    sim808.println("Pas de signal cardiaque.");
  }

  delay(100);
  sim808.write(26);  // Envoi du SMS
}
