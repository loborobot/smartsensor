//Arduino Code Lobo Robot Electronics
//Code by LoboRobotElectronics crew 2014
//Version 1.0

#include <Wire.h>
#include <RTClib.h>
#include <SD.h>
#include <WiFiRM04.h>
#include "Sensors.h"
#include "Connection.h"

Sensors psensors__;//objeto Sensores
Connection pconn3__;


char reqGET[]="GET /api/ HTTP/1.0\n"
"Host: 192.168.1.200\n"
"Accept: application/json\n"
"Connection: close";
    
char reqPOST[]="POST /api/update/ HTTP/1.0\n"
"Host: 192.168.1.200\n"
"Content-Type: application/json\n"
"Accept: application/json\n"
"Content-Length: 245\n"
"Connection: close\n";
    
char bodyPOST[]="{\"device\": \"166d77ac1b46a1ec38aa35ab7e628ab5\","
"\"pub_date\": \"2014-07-15T22:02:27.321Z\","
"\"temperature\": \"0\","
"\"humidity\": \"0\","
"\"light\": \"0\","
"\"ultra_violet\": \"0\","
"\"sound\": \"0\","
"\"flowmeter\": \"0\","
"\"volume\": \"0\","
"\"nitrogen_dioxide\": \"0\","
"\"carbon_monoxide\": \"0\"}";


char ssid[] = "WLAN_16D2"; //  your network SSID (name) 
char pass[] = "Z1460809D16D2";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
IPAddress server(192,168,1,200);  // numeric IP for Google (no DNS)
//char server[] = "192.168.1.200";    // name address for Google (using DNS)

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
//WiFiRM04Client client;

//==============GPRS using GSM.h==========
/*
// PIN number if necessary
#define PINNUMBER ""

// APN information obrained from your network provider
#define GPRS_APN       "ba.amx" // replace with your GPRS APN
#define GPRS_LOGIN     "amx"    // replace with your GPRS login
#define GPRS_PASSWORD  "amx" // replace with your GPRS password

char server[] = "190.42.34.239"; // the base URL
char path[] = "/api/devices/"; // the path
int port = 8000; // the port, 80 for HTTP
*/
//============== GPRS end using GSM.h===========
WiFiRM04Client client;
long previousMillis = 0;
long interval = 10000; 

void setup () {  
  Serial.begin(115200);//9600
  Serial1.begin(115200);//9600
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present"); 
    // don't continue:
    while(true);
  } 
  /*
  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
    status = WiFi.begin(ssid, pass);
  
    // wait 10 seconds for connection:
    delay(10000);
  } */
  Serial.println("Connected to wifi");
  //statusConn = STATUS_ONCONNECTION;
  printWifiStatus();
  
  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server, 8000)) {
    Serial.println("connected to server");
    client.println(reqGET);
    client.println();
    //statusServer = STATUS_ONCONNECTION;
  }
}

void loop () {
  //pconn3__.readSerials();
  //Serial.println("Loop");
  //psensors__.execute();
  ///delay(10000);//10 seconds delay
  
  unsigned long currentMillis = millis();
  
  while (interval > currentMillis){
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }
  }
  interval = interval + currentMillis;
  

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop();

    // do nothing forevermore:
    while(true);
  }   
  
  psensors__.execute();
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  
  psensors__.begin();
  Serial.println("END setup");
}
