#include<ESP8266WiFi.h>
#include<PubSubClient.h>
#include<ArduinoJson.h>
char data[100];
#define CLIENT_ID "distance"
#define port 1883
const char *ssid="KJ.NATH";
const char *pswd="8500005875";
const char *mqtt_server="3.218.230.74";
WiFiClient espclient;
PubSubClient client(espclient);

const int trig = 2;  //D4
const int echo = 0;  //D3
void setup() {
  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);
  Serial.begin(9600);
  WiFi.begin(ssid,pswd);
  while(WiFi.status()!=WL_CONNECTED)
  {
   Serial.print("....");
   delay(200);
  }
Serial.println("\n WiFi CONNECTED");
Serial.println(ssid);
Serial.println(WiFi.localIP());
client.setServer(mqtt_server,1883);
}
void reconnect() {
  while(!client.connected())
  {
    Serial.print("Attempting mqtt connect");
  if(client.connect(CLIENT_ID))
  {
    Serial.println("connected");
    
  }
  // put your setup code here, to run once:
 
}
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(trig,LOW);
  delayMicroseconds(2);
  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);
  int duration=pulseIn(echo,HIGH);
  float distance=((0.034*duration)/2);
  
  if (distance>=20)
  {
    Serial.println("the bin is empty");
    
    
  }
  if (distance>10 && distance<20)
  {
    Serial.println("the bin is ready to fill");
    
    
  }
  if (distance<5)
  {
    Serial.println("the bin is filled");
    
  }
  Serial.print("distance: ");
  Serial.print(distance);
  Serial.println("cm");
  
  if(!client.connected())
  {
    reconnect();
  }

   StaticJsonDocument<300> Doc;
   Doc["distance"]=String(distance);
   serializeJson(Doc, data);
   if(client.connect(CLIENT_ID))
   {
      client.publish("distance",data);
   }
   delay(10000);

}
