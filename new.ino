#include "MQ135.h"//Air sensor library
#include <ESP8266WiFi.h>
//#include <SoftwareSerial.h>//Serial com with bt


/*Objects*/
WiFiClient client;
MQ135 mqSensor(A0);//mqsensor
/*Start*/
// replace with your channel?s thingspeak API key and your SSID and password
String apiKey = "XXX";
const char* ssid = "XXX";
const char* password = "XXX";
const char* server = "api.thingspeak.com";


void setup() {
  Serial.begin(115200);
  delay(12000);
  WiFi.begin(ssid, password);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) 
  {
   delay(500);
   Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  mqSensor.getRZeroCO();
  mqSensor.getRZeroCO2();
  mqSensor.getRZeroEthanol();
  mqSensor.getRZeroNH4();
  mqSensor.getRZeroToluene();
  mqSensor.getRZeroAcetone();
  Serial.print("MQ135 RZERO Calibration Value : ");

}

void loop() {

     delay(240000);
     float resistance = mqSensor.getResistance();//resistance
     float CO      = mqSensor.getCO(resistance);//co ppm
     float CO2     = mqSensor.getCO2(resistance);//co2 ppm
     //float ehtanol = mqSensor.getEthanol(resistance);//ethanol ppm
     //float NH4     = mqSensor.getNH4(resistance); //NH4 ppm
     //float Toluene = mqSensor.getToluene(resistance); //toluene ppm
     //float Acetone = mqSensor.getAcetone(resistance); //acetone ppm*/
     //
     if (client.connect(server,80)) {
     String postStr = apiKey;
     postStr +="&field1=";
     postStr += String(CO);
     postStr +="&field2=";
     postStr += String(CO2);
     //postStr +="&field3=";
     //postStr += String(h);
     //postStr +="&field4=";
     //postStr += String(t);
     postStr += "\r\n\r\n";

     client.print("POST /update HTTP/1.1\n");
     client.print("Host: api.thingspeak.com\n");
     client.print("Connection: close\n");
     client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
     client.print("Content-Type: application/x-www-form-urlencoded\n");
     client.print("Content-Length: ");
     client.print(postStr.length());
     client.print("\n\n");
     client.print(postStr);
     Serial.print("CO ppm: ");
     Serial.print(CO);
     Serial.print("CO2 ppm: ");
     Serial.print(CO2); 
     //Serial.print("Temperature: ");
     //Serial.print(t);
     //Serial.print(" degrees Celsius Humidity: ");
     //Serial.print(h);
     //Serial.println("Sending data to Thingspeak"); 
    }
     client.stop();
     Serial.println("Waiting 20 secs");
     delay(12000);
}

