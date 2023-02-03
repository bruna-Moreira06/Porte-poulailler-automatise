#include <Servo.h>
Servo monServo;
int sensorLight = A0;
int LED = 13;
int detection = 0;
int BUZZER = 2;
int duree = 0;
int melodie[] = { 262, 294, 330, 349, 392, 440, 494, 523 };
int dureeNote[] = { 8, 8, 8, 8, 8, 8, 8, 8 };
bool doorOpen = false;

// stepper
#include <Stepper.h>
const int stepsPerRevolution = 600;  // variable pour definir le nombre de pas a pas
// initialisation de la librairie stepper sur les pins allant de 8 a 11:
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);


void setup() {
  // initialisation des moteur & pin
  Serial.begin(9600);
  pinMode(sensorLight, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  // Fixe la vitesse de rotation en tours par minute du stepper
  myStepper.setSpeed(60);
  // initialisation buzzer
  int i;
  for (i = 0; i < 8; i++) {
    tone(BUZZER, melodie[i], 1000 / dureeNote[i]);
    delay(1300 / dureeNote[i]);
    noTone(BUZZER);
  }
}  // condition du coucher du soleil qui va capter la lumière exterieure
// Une fois que la variable sensorValue est en dessous d'un certain seuil
// En l'occurence "150"
void loop() {
  int sensorValue = analogRead(sensorLight);
  Serial.println(sensorValue);
  //condition du coucher du soleil
  if (sensorValue <= 100) {
    // condition pour lever la porte qu'une seule fois
    // grace a la variable bool doorOpen
    digitalWrite(LED, HIGH);
    if (!doorOpen) {
      delay(1000);
      upDoor(8);
      // La valeur devient true
      Serial.println(doorOpen);
      doorOpen = true;
      Serial.println(sensorValue);
      Serial.println("NUIT");
      delay(100);
    }
  } else {
    //condition du lever du soleil
    digitalWrite(LED, LOW);
    if (doorOpen) {
      delay(1000);
      downDoor(8);
      // ensuite deviens false afin de pouvoir lancer la fonction
      Serial.println(doorOpen);
      doorOpen = false;
      Serial.println(sensorValue);
      Serial.println("JOUR");
    }
  }
}

// fonction pour faire descendre la porte
void downDoor(int nbTour) {
  for (int i = 0; i < nbTour; i++) {
    myStepper.step(stepsPerRevolution);
  }
  Serial.println("clockwise");
  delay(500);
}

// fonction pour faire monter la porte
void upDoor(int nbTour) {
  for (int i = 0; i < nbTour; i++) {

    myStepper.step(-stepsPerRevolution);
  }
  Serial.println("counterclockwise");
  delay(500);
}

// fonction pour le buzzer en cas de blocage
void buzzer() {
  digitalWrite(BUZZER, HIGH);
  delay(1000),
    digitalWrite(BUZZER, LOW);
  delay(1000);
}

// fonction pour la detection de mouvement
// void movementCapteur() {
//   detection = digitalRead(capteur);
//   digitalWrite(LED, LOW);
//   if (detection == HIGH) {
//     digitalWrite(LED, HIGH);
//     Serial.println("Mouvement détecter!!");
//     delay(500);
//   } else {
//     Serial.println("RAS");
//     delay(100);
//   }
//   delay(1000);
// }
