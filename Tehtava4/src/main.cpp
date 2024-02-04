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
  digitalWrite(pwrNTC, LOW);
  analogReadResolution(10);
  if ((!digitalRead(buttonB1)) && (a == 0))
  {
    Serial.println("aloitus");
    digitalWrite(pwrNTC, HIGH);
    alkuAika = micros();

    for (int i=0; i<50; i++)
    {
      mittaukset[i] = analogRead(sensoriNTC);
      delay(10);
    }
    loppuAika = micros();
    a = 1;

    for (int i=0; i<50; i++)
    {
      Serial.print(mittaukset[i]);
      Serial.print("; ");
    }
    Serial.println(" ");
    Serial.print("Kokonaisaika: ");
    Serial.println(loppuAika-alkuAika);
    Serial.print("Yhden mittauksen aika: ");
    Serial.println((loppuAika-alkuAika)/50.0);
    
  }
}

