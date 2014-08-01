//Arduino Code Lobo Robot Electronics
//Code by LoboRobotElectronics crew 2014
//Version 1.0

#include <Wire.h>
#include <RTClib.h>
#include <EEPROM.h>
#include "Sensors.h"
//#include "Constants.h"
//#include <GSM.h>// import the GSM library



//========Star GPRS Sim900========================
//Sim900 sim900gprs(S900Rx_PIN, S900Tx_PIN);//sim900
//=========End GPRS Sim900=========================

//const int _led[]={3,5,6,9,10,11,13};//leds

//int _Value[5];//values analog write
//int _val[5];//values
//char _into[5];//values  
//int _dato1,_i=0;

RTC_DS1307 RTC; //objeto RTC

Sensors p_sensors;//objeto Sensores


//==============GPRS using GSM.h==========
/*
// PIN number if necessary
#define PINNUMBER ""

// APN information obrained from your network provider
#define GPRS_APN       "ba.amx" // replace with your GPRS APN
#define GPRS_LOGIN     "amx"    // replace with your GPRS login
#define GPRS_PASSWORD  "amx" // replace with your GPRS password

// initialize the library instances
GSMClient client;
GPRS gprs;
GSM gsmAccess;

// This example downloads the URL "http://arduino.cc/latest.txt"

char server[] = "190.42.34.239"; // the base URL
char path[] = "/api/devices/"; // the path
int port = 8000; // the port, 80 for HTTP
*/

//reqHTTP pHTTP;


//============== GPRS end using GSM.h===========

byte valor;

void setup () {  
  Serial.begin(9600);
  p_sensors.begin();
  //sim900gprs.setupSim900();

  


  Wire.begin();
  //startVal();
  startRTC();
  //Serial.print("Read sensor: "); //DHT11
  //pinMode( ledPin, OUTPUT ); // ledPin how output   
  
  for (int i = 0; i < 256; i++){
    EEPROM.write(i, i);
  }
  
  for (int j = 0; j < 256; j++){
    valor = EEPROM.read(j);
    Serial.print(j);
    Serial.print("t");
    Serial.print(valor, DEC);
    Serial.println();
  }


  /*startSensorDHT11();
  startSensorLDR();
  startSensorUV();
  startSensorSound();
  startFlowMeter();

  String jData = pHTTP.dataJson(humidity,temperature,light,ultra_violet,sound,flowmeter,volume,nitro_dioxide,carbon_monoxide);
  //Serial.println(jData);
  String rGet = pHTTP.get("/api/devices/");
  Serial.println(rGet);
  Serial.println(" ");
  String rPost = pHTTP.post("/api/devices/");
  Serial.println(rPost);
  Serial.println(" ");
  String rDataJson = pHTTP.getDataJson();
  Serial.println(rDataJson);

  //generate the MD5 hash for our string
  char* Password = "123";
  unsigned char* hash=MD5::make_hash(Password);
  Serial.println(Password);
  //generate the digest (hex encoding) of our hash
  char *md5str = MD5::make_digest(hash, 16);
  //print it on our serial monitor
  Serial.println(" ");
  Serial.print("Encriptado MD5: ");
  Serial.println(md5str);*/


//============GPRS USING GSM.h=========
  // initialize serial communications
  //Serial.begin(9600);
  
  /*
  Serial.println("Starting Arduino web client");
  // connection state
  boolean notConnected = true;

  // Start GSM shield
  // pass the PIN of your SIM as a parameter of gsmAccess.begin()
  while(notConnected)
  {
    if((gsmAccess.begin(PINNUMBER)==GSM_READY) &
        (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD)==GPRS_READY))
      notConnected = false;
    else
    {
      Serial.println("Not connected");
      delay(1000);
    }
  }

  Serial.println("connecting..");
  
  //Get IP.
  IPAddress LocalIP = gprs.getIPAddress();
  Serial.println("Server IP address=");
  Serial.println(LocalIP);

  // if you get a connection, report back via serial:
  if (client.connect(server, port))
  {
    Serial.println("connected");
    // Make a HTTP request:
    client.println(pHTTP.post("/api/devices/"));
    client.println(pHTTP.getDataJson());
    */
    
    //request POST
    /*client.println("POST /api/devices/ HTTP/1.0");
    client.println("Content-Type: application/json");
    client.println("Accept: application/json");
    client.println("Authorization: Basic ZmluY3l0OmZpbmN5dA==");
    client.println("Content-Length: 13");
    client.println();
    client.println("{\"gsm\": 3000}");
    client.println();*/
    
    
    //request
    /*client.println("GET /api/devices/ HTTP/1.0");
    client.println("Accept: application/json");
    client.println("Authorization: Basic ZmluY3l0OmZpbmN5dA==");
    client.println();*/
  /*
  } 
  else
  {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
  */
  //============END GPRS USING GSM.h=========

  
}



void loop () {
  //Serial.println(" ");
  //resultDisplay();
  //startSim900();


  /*startSensorDHT11();
  startSensorLDR();
  startSensorUV();
  startSensorSound();
  startFlowMeter();*/
  
  p_sensors.execute();
  displayRTC(); 
      
      
  
  
  //============GPRS USING GSM.h========= 
  /// if there are incoming bytes available 
  // from the server, read them and print them:
  
  /*
  if (client.available())
  {
    char c = client.read();
    Serial.print(c);
  }

  // if the server's disconnected, stop the client:
  if (!client.available() && !client.connected())
  {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();

    // do nothing forevermore:
    for(;;)
      ;
  }
  */
  //============END GPRS USING GSM.h=========
}
