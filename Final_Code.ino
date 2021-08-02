#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>
#include <SPI.h>      //rfid 
#include <MFRC522.h>  //rfid

#define FIREBASE_HOST "soulyaan.firebaseio.com"
#define FIREBASE_AUTH "C7kIhH0vbC3Z84kQZbyjZDaCmgnEoJhQ08IFaB58"
#define WIFI_SSID "AbcD"
#define WIFI_PASSWORD "ssrk2qccjb"
#define RST_PIN   D1           //5 9 //rfid
#define SS_PIN    D2           //4 10 //rfid

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

void setup() {
  Serial.begin(9600);
  
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  Serial.begin(9600);        //rfid // Initialize serial communications with the PC
  SPI.begin();               //rfid // Init SPI bus
  mfrc522.PCD_Init();        //rfid // Init MFRC522 card
}

int n = 0;

void loop() {
  int idd[20];
  long d=0;

  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

    for (byte i = 0; i < mfrc522.uid.size; i++) {
//    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
//    Serial.print(mfrc522.uid.uidByte[i], HEX);
//    Serial.print(mfrc522.uid.uidByte[i], DEC);
//    Serial.print(mfrc522.uid.uidByte[i]);
      idd[i]= mfrc522.uid.uidByte[i];     
  }
//  delay(5000);

  for(int j = 0; j <mfrc522.uid.size  ; j++)
  {
    //Serial.print(idd[j]);
    d=d*10+idd[j];
    
  }
//  Serial.print(d);
//  delay(1000);

    // set string value
  Firebase.setInt("detail/tag_id", d);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /message failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(500);

   // append a new value to /logs
//  String name = Firebase.pushInt("logs", n++);
//  // handle error
//  if (Firebase.failed()) {
//      Serial.print("pushing /logs failed:");
//      Serial.println(Firebase.error());  
//      return;
//  }
//  Serial.print("pushed: /logs/");
//  Serial.println(name);
//  delay(1000);
}
