/*
  Ajan arviointi -peli Adafruit Metro M4 alustalle. Peli pyytää painamaan
  nappia B1 viisi kertaa, niin että ensimmäisen ja viidennen painalluksen väli on 
  20 sekuntia. Tämän jälkeen peli antaa tulokset ja alkaa alusta uudestaan.

  Jyri Turkki 01/2024
*/

#include <Arduino.h>

int Button = 4;               // nappi B1 on 4 Metro M4 alustalla
volatile int maxLkm = 5;      // painallusten maksimi lukumäärä
volatile int lkm = -1;        // laskuri napin painalluksille, aloitus -1 ohjeiden antoa varten
volatile double ajat[5];      // taulukko napin painamisajoille, huom sekunnin sadasosina


volatile unsigned long aloitusMillit = 0;  // ajanmittauksen aloitushetki
volatile boolean aloitettu = false;        // onko peli aloitettu

void NapinPainallus();
void tulokset();

void setup() {

  Serial.begin(9600);       // avataan sarjamonitori
  while(!Serial);
  
  pinMode(Button, INPUT_PULLUP);    // alustetaan painonappi

// kytketään painonappiin keskeytys
  attachInterrupt(digitalPinToInterrupt(Button), NapinPainallus, FALLING);

}

void NapinPainallus()
  {
    lkm++;                  // lisätään painallusten lukumäärää ja tulostetaan se
    Serial.println(lkm);    // sarjamonitoriin
    
   if (!aloitettu) {                // pelin aloitus, eli napin ensimmäinen painallus
        aloitusMillit = millis();   // otetaan talteen aloitusaika 
        aloitettu = true;                
    }

    // jokaisen napinpainannan aika taulukkoon sekunnin sadasosina
    ajat[lkm-1] = (millis() - aloitusMillit)/10.0;  
   
    if (lkm == maxLkm) {            // kun viisi painallusta tehty, näytetään tulokset
      tulokset();                   // ja alustetaan peli  
      lkm = -1;
      aloitettu = false;
      Serial.println();
    }
   
  }

  void tulokset() {         // tulokset sarjamonitoriin
      Serial.println();
      Serial.print("Tavoiteaika oli 20 s. Sait tulokseksi ");
      Serial.print(ajat[4]/100.0);                // muutetaan sadasosat sekunneiksi
      Serial.print(" sekuntia, eli virheesi oli ");
      Serial.print(fabs(20-(ajat[4]/100.0)));     // tulostetaan virheen määrä sekunteina
      Serial.println(" sekuntia.");
      Serial.println();

      double keskiarvo = ajat[4]/400.0;       // painallusten keskiarvo sekunteina

      Serial.print("Painallusten keskiarvo oli ");
      Serial.print(keskiarvo);
      Serial.println(" sekuntia.");
      Serial.println();

      double hajonta = 0;
          // lasketaan keskihajonta sekunteina (ajat muutetaan ensin sadasosista sekunneiksi)
      for (int i=0; i<4; i++) {
        hajonta += (((ajat[i+1]-ajat[i])/100.0-keskiarvo)*((ajat[i+1]-ajat[i])/100.0-keskiarvo));
      }

      hajonta = hajonta/4.0;   
      hajonta = sqrt(hajonta);

      Serial.print("Painallusten keskihajonta oli ");
      Serial.print(hajonta);
      Serial.print(" sekuntia.");
      
  }

void loop() {

  if (lkm < 0) {    // pelin ohjeet ja laskurin nosto -1:stä nollaan
    lkm++;
    Serial.println();
    Serial.println("Paina nappia B1 niin että ensimmäisen ja viimeisen");
    Serial.println("painalluksen välinen aika on 20 sekuntia.");
  }
}
