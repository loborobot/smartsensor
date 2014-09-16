#include "Connection.h"
//#include "Resources.h"
//#include "Sensors.h"
#include "Constants.h"

char reqGET[]="GET /api/ HTTP/1.0\n"
"Host: 192.168.1.200\n"
"Accept: application/json\n"
"Connection: close";
    
char reqPOST[]="POST /api/update/ HTTP/1.0\n"
"Host: 192.168.1.200\n"
"Content-Type: application/json\n"
"Accept: application/json\n"
"Content-Length: 290\n"
"Connection: close\n";
    
char bodyPOST[]="{\"device\": \"166d77ac1b46a1ec38aa35ab7e628ab5\", \"pub_date\": \"2014-07-15T22:02:27.321Z\", \"temperature\": \"0\", \"humidity\": \"0\", \"light\": \"0\", \"ultra_violet\": \"0\", \"sound\": \"0\", \"flowmeter\": \"0\", \"volume\": \"0\", \"nitrogen_dioxide\": \"0\", \"carbon_monoxide\": \"0\"}";


//Sensors psens2__;
//Resources presc2__;
boolean connected = false;
uint32_t baud[7]={
  2400, 4800, 9600, 19200, 38400, 57600, 115200};

long previousMillis = 0;
long interval = 10000; 

char ssid[] = "WLAN_16D2"; //  your network SSID (name) 
char pass[] = "Z1460809D16D2";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

//****************************************************************START WIFI-HILINK
int status = WL_IDLE_STATUS;
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
IPAddress server(192,168,1,200);  // numeric IP for Google (no DNS)
//char server[] = "192.168.1.200";    // name address for Google (using DNS)

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
WiFiRM04Client client;
//*******************************************************************END WIFI-HILINK

void Connection::begin(){ //init variables

  Serial.println("Start Wireless Config"); 
  statusConn = STATUS_OFFCONNECTION;
  statusServer = STATUS_OFFCONNECTION;
  /*if(connect()){
    statusConn = STATUS_ONCONNECTION;
  }*/
  //*************************************************START WIFI-HILINK
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
  
  printWifiStatus();
  int tam = sizeof(bodyPOST);
  Serial.println(tam);
  Serial.println("\nStarting connection to server...");
  
  Serial.println(reqPOST);
  for(int j=0; j<14; j++){
    if(j == 1){
        Serial.print(bodyJSON[1]);
        Serial.print("2014-07-15T22:02:27.321Z");
      }
    if(j>=2){
      Serial.print(bodyJSON[j]);
      if(j<13) Serial.print(2.3);
    }
    if(j==0)Serial.print(bodyJSON[j]);
  }
  Serial.println();
  Serial.println();
  
  // if you get a connection, report back via serial:
  if (client.connect(server, 8000)) {
    Serial.println("connected to server");
    client.println(reqPOST);
    for (int i=0; i<14; i++){  
      if(i == 1){
        client.print(bodyJSON[1]);
        client.print("2014-07-15T22:02:27.321Z");
      }
      if(i >= 2){
        client.print(bodyJSON[i]);
        if(i<13) client.print(2.3);
      }
      if(i==0)client.print(bodyJSON[i]);
    }
    client.println();
    statusServer = STATUS_ONCONNECTION;
  }
  readSerials();
  
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
  
  Serial.println("END setup");
}


void Connection::readSerials(){
  
 unsigned long Starttime = millis();
 unsigned long Stoptime = Starttime + 30000;
 
  while (millis() < Stoptime){
    while (client.available()) {
      char c = client.read();
      Serial.write(c);
    }
  }
  Starttime = 0;
  Stoptime = 0;

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop();

    // do nothing forevermore:
    while(true);
  }   
}

void Connection::printData(String data){
  for(int k=0; k<6; k++){
    if(k==4){
      Serial.print(HTTPPOST[k]);
      Serial.println(data.length());
    }
    else Serial.println(HTTPPOST[k]);
  }
  Serial.println(data);
  Serial.println();
}

boolean Connection::sendQueryData(String data){
      statusServer = STATUS_ONCONNECTION;
      for(int l=0; l<6; l++){
        if(l==4){
          client.print(HTTPPOST[l]);
          client.println(data.length());
          //client.print(HTTPPOST[l]);
          //client.println(data.length());
        }
        else client.println(HTTPPOST[l]);
        //else client.println(HTTPPOST[l]);
      }
      client.println(data);
      client.println();
      return true;
      //client.println(data);
      //client.println();
      //readSerials();
}

//**************************************************************************END WIFI-HILINK 



//**************************************************************************START WIFI-RN171
/*
void Connection::printData(String data){
  for(int k=0; k<6; k++){
    if(k==4){
      Serial.print(HTTPPOST[k]);
      Serial.println(data.length());
    }
    else Serial.println(HTTPPOST[k]);
  }
  Serial.println(data);
  Serial.println();
}

boolean Connection::sendQueryData(String data){
  if(enterCommandMode()){
    if(open(SERVER[0], 8000)){
      statusServer = STATUS_ONCONNECTION;
      for(int l=0; l<6; l++){
        if(l==4){
          Serial1.print(HTTPPOST[l]);
          Serial1.println(data.length());
          //client.print(HTTPPOST[l]);
          //client.println(data.length());
        }
        else Serial1.println(HTTPPOST[l]);
        //else client.println(HTTPPOST[l]);
      }
      Serial1.println(data);
      Serial1.println();
      if(close()) return true;
      //client.println(data);
      //client.println();
      //readSerials();
    }
  }
  else{
    repair();
    return false;
  }
}




boolean Connection::findInResponse(const char *toMatch,
unsigned int timeOut = 1000) {
  int byteRead;

  unsigned long timeOutTarget; // in milliseconds

  for (unsigned int offset = 0; offset < strlen(toMatch); offset++) {
    timeOutTarget = millis() + timeOut; // Doesn't handle timer wrapping
    while (!Serial1.available()) {
      // Wait, with optional time out.
      if (timeOut > 0) {
        if (millis() > timeOutTarget) {
          return false;
        }
      }
      delay(1); // This seems to improve reliability slightly
    }
    byteRead = Serial1.read();
    //Serial.print((char)byteRead);
    delay(1); // Removing logging may affect timing slightly

    if (byteRead != toMatch[offset]) {
      offset = 0;
      // Ignore character read if it's not a match for the start of the string
      if (byteRead != toMatch[offset]) {
        offset = -1;
      }
      continue;
    }
  }

  return true;
}

boolean Connection::enterCommandMode() {
    delay(COMMAND_MODE_GUARD_TIME);
    Serial1.print(F("$$$"));
    delay(COMMAND_MODE_GUARD_TIME);
    Serial1.println();
    Serial1.println();
    if (findInResponse("\r\n<", 1000))
    {
      return true;
    }
  return false;
}

boolean Connection::connect(){
  Serial.println("Connecting..");
  if(enterCommandMode()){
    Serial.println("inicio config");
    Serial1.println("set wlan auth 4\r");delay(1000);
    Serial1.println("set ip dhcp 1\r");delay(1000);
    Serial1.println("set ip proto 10\r");delay(1000);
    Serial1.println("set wlan ssid WLAN_16D2\r");delay(1000);
    Serial1.println("set wlan phrase Z1460809D16D2\r");delay(1000);
    Serial1.println("set comm remote 0");delay(1000);
    Serial1.println("set wlan linkmon 5\r");delay(1000);
    Serial1.println("set wlan channel 0\r");delay(1000);
    Serial1.println("set wlan join 1\r");delay(1000);
    Serial1.println("save\r");delay(1000);
    Serial1.println("reboot\r");
    Serial.println("fin config");
    return true;
  }
  else{
    repair();
    return false;
  }
} 


boolean Connection::open(const char *addr, int port) {

  if (connected) {
    close();
  } 
  if (enterCommandMode())
  {
    Serial1.print("open ");
    Serial1.print(addr);
    Serial1.print(" ");
    Serial1.print(port);
    Serial1.println("\r");
    if (findInResponse("*OPEN*", 3000)) 
    {
      connected = true;
      return true;
    }
    else return false;
  }
  enterCommandMode();
  return false;
}

boolean Connection::close() {
  if (!connected) {
    return true;
  }
  Serial1.println("close\r");
  if (findInResponse("*CLOS*", 3000)) {
    connected = false;
    return true;
  }
  connected = false;
  return false;
}


boolean Connection::reset() {
  enterCommandMode();
  Serial1.println("factory R\r"); // Store settings
  Serial1.println("save\r"); // Store settings
  Serial1.println("reboot\r");
  
  return true;
}

void Connection::repair(){
  if(!enterCommandMode())
  {
    boolean repair = true;
    for (int i=6; ((i>=0)&&repair); i--)
    {
      Serial1.begin(baud[i]);
      if(enterCommandMode()) 
      {
        reset();
        repair = false;
      }
      Serial1.begin(9600);
    }
  }
}
*/
//***************************************************************END WIFI-RN171







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
