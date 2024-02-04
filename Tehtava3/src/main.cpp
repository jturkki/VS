/*
  Led-valon säätö. Nappi B1:n nopea painallus sytyttää led-valon. Saman napin pitkä painallus
  vuorotellen kirkastaa tai himmentää valoa. Uusi lyhyt painallus sammuttaa valon.
  Valon kirkkaus vaihtelee välillä 2 - 255. Kun sammutettu valo kytketään päälle,
  valon kirkkaudeksi asettuu 100. Kun sammutettu valo kytketään päälle, 
  ensimmäinen pitkä painallus kirkastaa valoa.

  Jyri Turkki 02/2024
*/

#include <Arduino.h>

int led = 13;                  // led-valon pin
volatile int kirkkaus = 0;     // led-valon kirkkaus
int fadeStep = 5;              // kuinka monta prosenttia led valo muuttuu;
int kirkkaudenMuutos = 0;      // muutosaskel ledin kirkkaudelle fadeStep prosenttia

int ButtonB1 = 4;

volatile boolean kirkastusPaalla = true; 
volatile boolean nappiPohjassa = false;
volatile boolean valoPaalla = false;

unsigned long aika = 0;             // ajanhetki josta mitataan rajaAika-mittainen aika kirkkauden muutokselle
unsigned long rajaAika = 50;        // aikaväli kuinka usein ledin kirkkautta muutetaan nappia painettaessa
unsigned long napinPainoAika = 0;   // miten kauan nappi on ollut pohjassa (lyhyt/pitkä painallus)
unsigned long pitkaPainoAika = 250; // painallus on pitkä, jos se kestää yli 250 ms.

void nappiMuutos();


void setup() 
{
  pinMode(led, OUTPUT);
  pinMode(ButtonB1, INPUT);
  

   // Serialmonitorin alustus Debuggausta varten
  Serial.begin(9600);
  while(!Serial);


  attachInterrupt(digitalPinToInterrupt(ButtonB1), nappiMuutos, CHANGE);  // keskeytys painonapille
 
  aika = millis();
  kirkkaudenMuutos = ceil((fadeStep/100.0) * 255);   // askel kirkkauden muutokselle fadeStep prosenttia

}

void loop() 
{
   Serial.println(kirkkaus);             // debuggaukseen käytetty kirkkauden tulostus monitoriin

  if (!valoPaalla)                      // jos valo kytketty pois päältä, sammutetaan led
    kirkkaus = 0;

  analogWrite(led, kirkkaus);

  if ((millis() - aika) > rajaAika) {       // rajaAjan välein muutetaan kirkkausta jos kirkastus
                                            // tai himmennys päällä

    if (nappiPohjassa)                    // pitkän painalluksen yhteydessä tehtävät muutokset valon kirkkauteen
    {
      if ((kirkastusPaalla) && (kirkkaus < 255) && napinPainoAika-millis() > pitkaPainoAika)     // kirkastus kun pitkä painallus
      {  
        kirkkaus = kirkkaus + kirkkaudenMuutos;
        if (kirkkaus > 255)
          kirkkaus = 255;                 // varmistetaan ettei kirkkaus mene yli maksimin
      }

      if ((!kirkastusPaalla) && (kirkkaus > 2) && napinPainoAika-millis() > pitkaPainoAika) // himmennys kun pitkä painallus
      {
          kirkkaus = kirkkaus - kirkkaudenMuutos;
          if (kirkkaus < 2)
            kirkkaus = 2;                 // varmistetaan ettei kirkkaus mene alle kahden (valoa ei saa sammuksiin himmennyksellä)
      }
    }
    
  aika = millis();        // aloitetaan uudestaan mittaamaan aikaa rajaAikaan saakka  
  }
  
}


void nappiMuutos()                   // kutsutaan kun nappi painuu alas ja kun nousee ylös
{
  nappiPohjassa = !nappiPohjassa;
  if (nappiPohjassa)                    // aina kun nappi painetaan pohjaan, otetaan talteen painallusaika
    napinPainoAika = millis();
  

  // kun nappi vapautetaan, tarkistetaan oliko pitkä vai lyhyt painallus. Lyhyt -> valo päälle tai pois, pitkä -> kirkastus päälle tai pois.
  if ((!nappiPohjassa) && (millis() - napinPainoAika < pitkaPainoAika))   
  {
    valoPaalla = !valoPaalla;
    if (valoPaalla)
      kirkkaus = 100;   
  } 

  if ((!nappiPohjassa) && (millis() - napinPainoAika >= pitkaPainoAika))
    kirkastusPaalla = !kirkastusPaalla;
}  


