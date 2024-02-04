/*
  Led-valon säätö. Nappi B1 kirkastaa led-valoa 50ms välein annetun prosenttimäärän niin kauan kuin
  nappi pidetään pohjassa. Nappi B2 vastaavasti himmentää valoa.

  Jyri Turkki 02/2024
*/

#include <Arduino.h>

int led = 13;                  // led-valon pin
int kirkkaus = 0;              // led-valon kirkkaus
int fadeStep = 5;              // kuinka monta prosenttia led valo muuttuu;
int kirkkaudenMuutos = 0;      // muutosaskel ledin kirkkaudelle fadeStep prosenttia

int ButtonB1 = 4;
int ButtonB2 = 3;
volatile boolean kirkastusPaalla = false;   
volatile boolean himmennysPaalla = false;
unsigned long aika = 0;             // ajanhetki josta mitataan rajaAika-mittainen aika kirkkauden muutokselle
unsigned long rajaAika = 50;        // aikaväli kuinka usein ledin kirkkautta muutetaan nappia painettaessa

void kirkasta();
void himmenna();


void setup() 
{
  pinMode(led, OUTPUT);
  pinMode(ButtonB1, INPUT);
  pinMode(ButtonB2, INPUT);

 /*  Serialmonitorin alustus Debuggausta varten
  Serial.begin(9600);
  while(!Serial);
 */

  attachInterrupt(digitalPinToInterrupt(ButtonB1), kirkasta, CHANGE);  // keskeytykset painonapeille
  attachInterrupt(digitalPinToInterrupt(ButtonB2), himmenna, CHANGE);

  aika = millis();
  kirkkaudenMuutos = ceil((fadeStep/100.0) * 255);   // askel kirkkauden muutokselle fadeStep prosenttia

}

void loop() 
{
   // Serial.println(kirkkaus);             // debuggaukseen käytetty kirkkauden tulostus monitoriin

  analogWrite(led, kirkkaus);

  if ((millis() - aika) > rajaAika) {       // rajaAjan välein muutetaan kirkkausta jos kirkastus
                                            // tai himmennys päällä
    
    if ((kirkastusPaalla) && (kirkkaus < 255))
    {  
      kirkkaus = kirkkaus + kirkkaudenMuutos;
      if (kirkkaus > 255)
        kirkkaus = 255;                      // varmistetaan että kirkkaus ei mene yli maksimiarvon 
    }

    if ((himmennysPaalla) && (kirkkaus > 0))
    {
      kirkkaus = kirkkaus - kirkkaudenMuutos;
      if (kirkkaus < 0)
        kirkkaus = 0;                       // varmistetaan ettei kirkkaus mene alle nollan
    }

  aika = millis();  
  }
  
}


void kirkasta()                   // kutsutaan kun nappi painuu alas ja kun nousee ylös
{
    kirkastusPaalla = !kirkastusPaalla;   // muuutetaan kirkastus päälle tai pois
}

void himmenna()                   // kutsutaan kun nappi painuu alas ja kun nousee ylös
{
  himmennysPaalla = !himmennysPaalla;     // muuutetaan himmennys päälle tai pois
}

