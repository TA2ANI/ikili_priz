#define BLYNK_PRINT Serial
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
BlynkTimer timer;




//NODEMCU PİNLERİ
int kled = 5;       // D1
int mled = 0;       // D3
int yled = 4;       // D2
int role1 = 16;     // D0
int role2 = 2;      // D4
int button1 = 14;   // D5
int button2 = 12;   // D6
int prizled1 = 13;  // D7
int prizled2 = 15;  // D8
bool sondur = false;


int butondurum1 = 0;
int butondurum2 = 0;
int sayac1 ;
int sayac2 ;
bool dongudurum = false;

int sanalbuton1;                  // V0 -
int sanalbuton2;                  // V1 -
int sanalbuton3;                  // V2
bool role1durum = false;
bool role2durum = false;


char auth[] = "14d3fb26feb040e6b2d52e7c452f5467";


/*
  // WiFi Büro
  char ssid[] = "TA2ANI_connection";
  char pass[] = "AeF14772380";
*/




// WiFi EV
char ssid[] = "ELAN";
char pass[] = "1234AB567C";




BLYNK_WRITE(V0)
{
  sanalbuton1 = param.asInt();
  if (sanalbuton1 == 1)
  {
    sayac1 = 1;
    //    digitalWrite (prizled1, 1);
    //    digitalWrite (role1, 1);
  }

  else
  {
    sayac1 = 2;
    //    digitalWrite (prizled1, 0);
    //    digitalWrite (role1, 0);
    delay(50);
    Blynk.virtualWrite(V0, 0);
  }
}


BLYNK_WRITE(V1)
{
  sanalbuton2 = param.asInt();
  if (sanalbuton2 == 1)
  {
    sayac2 = 1;
    //    digitalWrite (prizled2, 1);
    //    digitalWrite (role2, 1);
  }

  else
  {
    sayac2 = 2;
    //    digitalWrite (prizled2, 0);
    //    digitalWrite (role2, 0);
    delay(50);
    Blynk.virtualWrite(V1, 0);
  }
}



BLYNK_WRITE(V2)
{
  sanalbuton3 = param.asInt();
  if (sanalbuton3 == 1)
  {
    sondur = true;
  }

  else
  {
    sondur = false;
  }
}


BLYNK_CONNECTED()
{
  Blynk.syncVirtual(V0);
  Blynk.syncVirtual(V1);
  //  Blynk.syncVirtual(V2);
}



void setup()
{
  Serial.begin(9600);
  pinMode (kled, OUTPUT);
  pinMode (yled, OUTPUT);
  pinMode (mled, OUTPUT);
  pinMode (prizled1, OUTPUT);
  pinMode (prizled2, OUTPUT);
  pinMode (role1, OUTPUT);
  pinMode (role2, OUTPUT);
  pinMode (button1, INPUT);
  pinMode (button2, INPUT);
  Blynk.begin(auth, ssid, pass);
  analogWrite (kled, 1023);
  analogWrite (yled, 1023);
  analogWrite (mled, 1023);
  digitalWrite (role1, 0);
  digitalWrite (role2, 0);
  timer.setInterval(10000L, dongu);
  timer.setInterval(10000L, kontrol);
}




void dongu ()
{
  dongudurum = true;
  if (dongudurum)
  {
    butondurum1 = digitalRead (button1);
    butondurum2 = digitalRead (button2);

    if (!sondur)
    {
      analogWrite (yled, 0);
      analogWrite (kled, 1023);
      analogWrite (mled, 1023);
    }
    else
    {
      analogWrite (yled, 1023);
      analogWrite (kled, 1023);
      analogWrite (mled, 1023);
    }

    butondurum1 = digitalRead (button1);
    butondurum2 = digitalRead (button2);

    if (butondurum1 == HIGH)
    {
      sayac1++;
      while (butondurum1 == HIGH)
      {
        butondurum1 = digitalRead (button1);
      }
    }
    if (sayac1 == 1)
    {
      digitalWrite (prizled1, 1);
      digitalWrite (role1, 1);
      delay(100);
      Blynk.virtualWrite(V0, 1);
    }
    if (sayac1 == 2)
    {
      digitalWrite (prizled1, 0);
      digitalWrite (role1, 0);
      delay(100);
      Blynk.virtualWrite(V0, 0);
    }
    if (sayac1 >= 2)
    {
      sayac1 = 0;
    }


    if (butondurum2 == HIGH)
    {
      sayac2++;
      while (butondurum2 == HIGH)
      {
        butondurum2 = digitalRead (button2);
      }
    }
    if (sayac2 == 1)
    {
      digitalWrite (prizled2, 1);
      digitalWrite (role2, 1);
      delay(100);
      Blynk.virtualWrite(V1, 1);
    }
    if (sayac2 == 2)
    {
      digitalWrite (prizled2, 0);
      digitalWrite (role2, 0);
      delay(100);
      Blynk.virtualWrite(V1, 0);
    }
    if (sayac2 >= 2)
    {
      sayac2 = 0;
    }
    Serial.print ("sayac 1 = ");
    Serial.println (sayac1);
    Serial.print ("sayac 2 = ");
    Serial.println (sayac2);
  }
}


void kontrol()
{
  Serial.println ("BAGLANTI KOPTU");
  while (!dongudurum)
  {
    analogWrite (yled, 1023);
    analogWrite (kled, 0);
    analogWrite (mled, 1023);


    if (WiFi.status() != WL_CONNECTED)
    {
      Serial.println ("WIFI DENENIYOR");
      //      digitalWrite (prizled1, 1);
      //      delay (250);
      //      digitalWrite (prizled1, 0);
      //      digitalWrite (prizled2, 1);
      //      delay (250);
      //      digitalWrite (prizled2, 0);
      WiFi.begin (ssid, pass);
      delay (1000);
      Blynk.begin(auth, ssid, pass);
    }

    if (WiFi.status() == WL_CONNECTED)
    {
      dongudurum = true;
    }
  }
}





void loop()
{
  if (Blynk.connected())
  {
    Blynk.run();
    timer.run();
    dongu ();
  }

  else
  {
    dongudurum = false;
    kontrol ();
  }
}

