#include "Connection.h"
#include "Sensors.h"
#include "Constants.h"

Sensors psens2__;

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
WiFiRM04Client client;

void Connection::begin(){ //init variables

  Serial.println("Start Wireless Config"); 
  statusConn = STATUS_OFFCONNECTION;
  statusServer = STATUS_OFFCONNECTION;
  //activeModeAT(); 
  /*Serial.begin(115200); 
  Serial1.begin(115200); 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }*/
  
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present"); 
    // don't continue:
    while(true);
  } 
  
  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
    status = WiFi.begin(ssid, pass);
  
    // wait 10 seconds for connection:
    delay(10000);
  } 
  Serial.println("Connected to wifi");
  statusConn = STATUS_ONCONNECTION;
  //printWifiStatus();
  
  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server, 8000)) {
    Serial.println("connected to server");
    statusServer = STATUS_ONCONNECTION;
  }
}

void Connection::printWifiStatus() {
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
}

void Connection::readSerials(){
 while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop();

    // do nothing forevermore:
    while(true);
  } 
}

void Connection::sendQuery(){
  //byte mac[6];
  //WiFi.macAddress(mac);
  if(statusServer && statusConn){
    String dataS = strucData();
    int lengthData = sizeData(dataS);
    for(int i=1; i<6; i++){
      client.println(HTTPPOST[i]);
      if(i==4){
        client.print(HTTPPOST[i]);
        client.print("00:00:00:00:00:00");
      }
      if(i==5){
        client.print(HTTPPOST[i]);
        client.println(lengthData);
      }
    }
    client.println(" ");
    sendData(dataS);
  }
}

void Connection::sendData(String dataS){
  if(statusServer && statusConn){
    client.println(dataS);
    client.flush();
  }
}

String Connection::strucData(){
  int i = 0;
  String data;
  
  data += JSONREQ[i];
  data += psens2__.convertF2C(psens2__._humidity);
  data += JSONREQ[i+1];
  data += psens2__.convertF2C(psens2__._valueLDR);
  data += JSONREQ[i+2];
  data += psens2__.convertF2C(psens2__._valueUV);
  data += JSONREQ[i+3];
  data += psens2__.convertF2C(psens2__._sound);
  data += JSONREQ[i+4];
  data += psens2__.convertF2C(psens2__._flowmeter);
  data += JSONREQ[i+5];
  data += psens2__.convertF2C(psens2__._volume);
  data += JSONREQ[i+6];
  data += psens2__.convertF2C(psens2__._CO2);
  data += JSONREQ[i+7];
  data += psens2__.convertF2C(psens2__._NO2);

  return data;
}

int Connection::sizeData(String dataS){
  int sized = dataS.length();
  return sized;
}
/*
void Connection::activeModeAT(){
  Serial.begin(115200);
  Serial1.begin(115200);
 
  pinMode(SCAPE_PIN, OUTPUT);
  digitalWrite(SCAPE_PIN, !SCAPE_PIN_ACTIVE);
  
  digitalWrite(SCAPE_PIN, SCAPE_PIN_ACTIVE);
  // according to user manual, delay should be > Tes time, so we plus 50ms here
  delay(TES_TIME_IN_MS+50);
  digitalWrite(SCAPE_PIN, !SCAPE_PIN_ACTIVE);
  
  modeAT = MODE_AT_ACTIVE;
  Serial.println("Modo AT actiado");
  if(modeAT) setConnection();
  else {
    Serial1.write("at+out_trans=0\r\n");
    modeAT = false;
  }

}

void Connection::setConnection(){
  if(modeAT){
    for(int i=0; i<13; i++){
      Serial1.write(commands_wifi_client[i]); 
    }
    Serial1.write("at+out_trans=0\r\n");
    statusConn = STATUS_ONCONNECTION;
   }
  else {
    modeAT = false;
    statusConn = STATUS_OFFCONNECTION;
    activeModeAT();
  }
}

boolean Connection::verifyConnection(){
  activeModeAT();
  if(modeAT){
    Serial1.write("at+wifi_ConState=?\r\n");
    Serial1.flush();
    Serial1.setTimeout(100);
    if(!Serial1.find("Connected\r\n")){
      Serial1.write("at+out_trans=0\r\n");
      return false;
    }
    else{
      Serial1.write("at+out_trans=0\r\n");
      return true;
    }
  }
}

char* Connection::getMAC(){
  char* mac;
  activeModeAT();
  if(modeAT){
    Serial1.write("at+Get_MAC=?\r\n");
    Serial1.flush();
    Serial1.setTimeout(100);
    if(Serial1.available()){
      mac = (char*)Serial1.read();
      Serial1.write("at+out_trans=0\r\n");
      return mac;
    }
    else{
      Serial1.write("at+out_trans=0\r\n");
      return mac;
    }
  }
}
*/
