#include <nRF24L01.h>
#include <RF24.h>
#include <SPI.h>

#define unit_id "ID-03"
#define freq_channel 50

RF24 radio(7, 8);

const int buzpin =  2;    
unsigned long previousMillis = 0;
long interval = 100;            
unsigned long previousMillis2 = 0;
long interval2 = 1000;

int detection=0;

const byte address_tx[6] = "00001";
const byte address_rx[6] = "00001";

void setup() {
  //Set NRF for Receiver
  Serial.begin(9600);
  pinMode(2,OUTPUT);
  radio.begin();
  radio.openReadingPipe(0, address_rx);
  radio.setChannel(freq_channel);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  randomSeed(analogRead(0));
}

void loop() {
  if (radio.available()) {
    detection++;
    char text[32] = "";
    radio.read(&text, sizeof(text));
  }

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {

     //Set NRF for Transmiter
    radio.openWritingPipe(address_rx);
    radio.setChannel(freq_channel);
    radio.setPALevel(RF24_PA_MIN);
    radio.stopListening();
  
    previousMillis = currentMillis;
    interval = random(50, 60);

    char text[100];
    sprintf(text,unit_id);
  
    radio.write(&text, sizeof(text));
  
    radio.openReadingPipe(0, address_rx);
    radio.setChannel(freq_channel);    
    radio.setPALevel(RF24_PA_MIN);
    radio.startListening();
  }

  if(currentMillis - previousMillis2 >= interval2)
  {
    previousMillis2 = currentMillis;
    Serial.print("detection=");
    Serial.println(detection);
    if(detection>=15)
    {
      digitalWrite(buzpin,1);
      Serial.println("ON");
    }
    else
    {
      digitalWrite(buzpin,0);
    }
    detection=0;
  }
}
