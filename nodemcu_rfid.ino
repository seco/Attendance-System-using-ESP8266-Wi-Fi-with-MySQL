
#include<SoftwareSerial.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <SPI.h>
#include <MFRC522.h>

const char* ssid = "your SSID name";// 
const char* password = "your SSID password ";
//WiFiClient client;
char server[] = "YOUR SERVER IP";   //eg: 192.168.0.222
#define SS_PIN 2 //FOR RFID SS PIN BECASUSE WE ARE USING BOTH ETHERNET SHIELD AND RS-522
#define RST_PIN 15
#define No_Of_Card 3

 WiFiClient client;

//WiFiServer server(80);
SoftwareSerial mySerial(8,9);     
MFRC522 rfid(SS_PIN,RST_PIN);
MFRC522::MIFARE_Key key; 
byte id[No_Of_Card][4]={
  {44,153,22,219},             //RFID NO-1
  {112,224,72,84},             //RFID NO-2
  {151,94,80,84}              //RFID NO-3
};
byte id_temp[3][3];
byte i;
int j=0;
void setup()
{
 Serial.begin(115200);
  delay(10);
  mySerial.begin(115200);
  SPI.begin();
  rfid.PCD_Init();

    for(byte i=0;i<6;i++)
  {
    key.keyByte[i]=0xFF;
  }

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
//  server.begin();
  Serial.println("Server started");
  Serial.print(WiFi.localIP());
  delay(1000);
  Serial.println("connecting...");
 }
void loop()
{  // Check if a client has connected
  int m=0;
  if(!rfid.PICC_IsNewCardPresent())
  return;
  if(!rfid.PICC_ReadCardSerial())
  return;
  for(i=0;i<4;i++)
  {
   id_temp[0][i]=rfid.uid.uidByte[i]; 
             delay(50);
  }
  
   for(i=0;i<No_Of_Card;i++)
  {
          if(id[i][0]==id_temp[0][0])
          {
            if(id[i][1]==id_temp[0][1])
            {
              if(id[i][2]==id_temp[0][2])
              {
                if(id[i][3]==id_temp[0][3])
                {
                  Serial.print("your card no :");
                  for(int s=0;s<4;s++)
                  {
                    Serial.print(rfid.uid.uidByte[s]);
                    Serial.print(" ");
                   
                  }
                  Serial.println("\nVALID");
                  Sending_To_DB();
                  j=0;
                            
                            rfid.PICC_HaltA(); rfid.PCD_StopCrypto1();   return; 
                }
              }
            }
          }
   else
   {j++;
    if(j==No_Of_Card)
    {
      Serial.println("inVALID");
      Sending_To_DB();
      j=0;
    }
   }
  }
  
     // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
 }

 void Sending_To_DB()   //CONNECTING WITH MYSQL
 {
   if (client.connect(server, 80)) {
    Serial.println("connected");
    // Make a HTTP request:
    Serial.println("GET /rfid/rfid_read.php?allow=");     //YOUR URL /rfid/rfid_read.php?allow
    client.print("GET /rfid/nodemcu_rfid/rfid_read.php?allow=");     //YOUR URL /rfid/rfid_read.php?allow  /var/www/html/rfid/rfid_read.php
    if(j!=No_Of_Card)
    {
      Serial.println('1');
      client.print('1');
    }
    else
    {
      Serial.println('0');
      client.print('0');
    }
    Serial.println("&id=");
    client.print("&id=");
    for(int s=0;s<4;s++)
                  {
                    Serial.println(rfid.uid.uidByte[s]);
                    client.print(rfid.uid.uidByte[s]);
                                  
                  }
    client.print(" ");      //SPACE BEFORE HTTP/1.1
    client.print("HTTP/1.1");
    client.print("Host: ");
     client.println(server);
    client.println("Host: YOUR SERVER IP");//eg: 192.168.0.222
    client.println("Connection: close");
    client.println();
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
  client.stop();
 }

