#include <Arduino.h>

int buttonB1 = 4;
int pwrNTC = A2;
int sensoriNTC = A0;
unsigned long int alkuAika = 0;
unsigned long int loppuAika = 0;
int mittaukset[50];
int a = 0;

void setup() 
{
  Serial.begin(9600);
  while(!Serial);

  pinMode(buttonB1, INPUT_PULLUP);
  pinMode(pwrNTC, OUTPUT);
  pinMode(sensoriNTC, INPUT);
}

void loop() 
{
  digitalWrite(pwrNTC, LOW);        // varmistetaan että sensorissa ei ole virrat päällä
  analogReadResolution(10);
  if ((!digitalRead(buttonB1)) && (a == 0))   // kun nappia painetaan, a varmistaa että tämä tehdään vain kerran
  {
    Serial.println("aloitus");        // debuggausta varten 
    digitalWrite(pwrNTC, HIGH);       // virrat päälle sensoriin
    alkuAika = micros();              // otetaan aloitusaika talteen

    for (int i=0; i<50; i++)
    {
      mittaukset[i] = analogRead(sensoriNTC);       // luetaan 50 mittausta taulukkoon
      delay(10);
    }
    loppuAika = micros();             // loppuaika talteen
    a = 1;                            // a varmistaa että mittaukset tehdään vain kerran

    for (int i=0; i<50; i++)
    {
      Serial.print(mittaukset[i]);      // tulostetaan mittaukset
      Serial.print("; ");
    }
    Serial.println(" ");
    Serial.print("Kokonaisaika: ");
    Serial.println(loppuAika-alkuAika);
    Serial.print("Yhden mittauksen aika: ");
    Serial.println((loppuAika-alkuAika)/50.0);
    
  }
}

