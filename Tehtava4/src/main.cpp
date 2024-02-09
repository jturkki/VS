/*
  Ohjelma testaa NTC-anturin käynnistyksen yhteydessä sensorin
  asettumisaikaa. Kun nappia B1 painaa, asetetaan ntc-sensrorille
  virrat päälle ja suoritetaan heti 50 mittausta lyhyin aikavälein.
  Lopuksi mittaustulokset tulostetaan monitoriin.
*/

#include <Arduino.h>

int buttonB1 = 4;
int pwrNTC = A1;
int sensoriNTC = A2;
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
      delayMicroseconds(100);
    }
    loppuAika = micros();             // loppuaika talteen
    a = 1;                            // a varmistaa että mittaukset tehdään vain kerran

    for (int i=0; i<50; i++)
    {
      Serial.println(mittaukset[i]);      // tulostetaan mittaukset
      
    }
    Serial.println(" ");
    Serial.print("Kokonaisaika: ");
    Serial.println(loppuAika-alkuAika);
    Serial.print("Yhden mittauksen aika: ");
    Serial.println((loppuAika-alkuAika)/50.0);
    
  }
}

