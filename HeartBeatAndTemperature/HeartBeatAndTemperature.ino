#include <TimerOne.h>   // Inclusion de la bibliothèque TimerOne pour gérer les interruptions basées sur le timer
#include <LiquidCrystal.h> // Inclusion de la bibliothèque pour contrôler l'écran LCD

// Initialisation de l'écran LCD avec les broches correspondantes
LiquidCrystal lcd(13, 12, 11, 10, 9, 8); 

int val; // Variable pour stocker la valeur lue du capteur de température
int tempPin = A0; // Broche du capteur de température
int HBSensor = 7; // Broche du capteur de fréquence cardiaque
int HBCount = 0; // Compteur de battements cardiaques
int HBCheck = 0; // Variable pour suivre l'état du capteur de battements cardiaques
int TimeinSec = 0; // Variable pour stocker le temps écoulé en secondes
int HBperMin = 0; // Variable pour stocker le nombre de battements par minute
int HBStart = 2; // Bouton pour démarrer la détection de la fréquence cardiaque
int HBStartCheck = 0; // Variable pour vérifier si la détection de la fréquence cardiaque a commencé

void setup() 
{
  // Initialisation de l'écran LCD (20 colonnes, 4 lignes)
  lcd.begin(20, 4); 
  pinMode(HBSensor, INPUT); // Configuration de la broche du capteur de fréquence cardiaque en entrée
  pinMode(HBStart, INPUT_PULLUP); // Utilisation d'une résistance pull-up pour le bouton de démarrage
  Timer1.initialize(800000); // Initialisation du timer pour générer une interruption toutes les 0.8 secondes (800 ms)
  Timer1.attachInterrupt(timerIsr); // Attachement de la fonction d'interruption à appeler à chaque période du timer

  // Affichage initial sur l'écran LCD
  lcd.clear(); // Effacer l'écran
  lcd.setCursor(0,0); // Positionner le curseur
  lcd.print("HEALTH MONITORING"); // Afficher le texte "HEALTH MONITORING" sur la première ligne
  lcd.setCursor(0,1); 
  lcd.print("TIME IN SEC : "); // Afficher le texte "TIME IN SEC : " sur la deuxième ligne
  lcd.setCursor(0,3); 
  lcd.print("BODY TEMP   : "); // Afficher le texte "BODY TEMP : " sur la quatrième ligne
  lcd.setCursor(0,2); 
  lcd.print("HB PER MIN  : "); // Afficher le texte "HB PER MIN : " sur la troisième ligne
}

void loop() 
{
  // Lecture de la température à partir du capteur
  val = analogRead(tempPin); // Lire la valeur analogique du capteur
  float mv = (val / 1023.0) * 5000; // Conversion de la valeur en millivolts (valeur entre 0 et 5000 mV)
  int cel = mv / 10; // Conversion en degrés Celsius (approximation)
  lcd.setCursor(14,3); // Positionner le curseur sur la quatrième ligne pour afficher la température
  lcd.print(cel); // Afficher la température en Celsius
  lcd.print("   "); // Effacer les caractères restants en ajoutant des espaces
  delay(100); // Pause de 100 ms pour éviter les mises à jour trop rapides

  // Si le bouton de démarrage est pressé
  if(digitalRead(HBStart) == LOW)
  {
    HBStartCheck = 1; // Activer le suivi des battements cardiaques
  }

  // Si la détection des battements cardiaques est active
  if(HBStartCheck == 1)
  {
    // Si un battement est détecté (signal haut) et qu'il n'a pas encore été compté
    if((digitalRead(HBSensor) == HIGH) && (HBCheck == 0))
    {
      HBCount = HBCount + 1; // Incrémenter le compteur de battements cardiaques
      HBCheck = 1; // Indiquer qu'un battement a été détecté
    }

    // Si le signal du capteur est bas, réinitialiser l'état du capteur
    if((digitalRead(HBSensor) == LOW) && (HBCheck == 1))
    {
      HBCheck = 0; // Réinitialiser l'état pour détecter un nouveau battement
    }

    // Après 10 secondes de mesure
    if(TimeinSec == 10)
    {
      HBperMin = HBCount * 6; // Calculer le nombre de battements par minute (6 fois le nombre de battements en 10 secondes)
      HBStartCheck = 0; // Désactiver la détection
      lcd.setCursor(14,2); // Positionner le curseur pour afficher la fréquence cardiaque
      lcd.print(HBperMin); // Afficher la fréquence cardiaque
      lcd.print(" "); // Effacer les caractères restants
      HBCount = 0; // Réinitialiser le compteur de battements
      TimeinSec = 0; // Réinitialiser le compteur de temps
    }
  }
}

// Fonction appelée par l'interruption du timer toutes les 0.8 secondes
void timerIsr()
{
  if(HBStartCheck == 1) // Si la détection est active
  {
    TimeinSec = TimeinSec + 1; // Incrémenter le compteur de temps
    lcd.setCursor(14,1); // Positionner le curseur pour afficher le temps en secondes
    lcd.print(TimeinSec); // Afficher le temps écoulé
    lcd.print(" "); // Effacer les caractères restants
  }
}
