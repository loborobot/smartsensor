#include "Connection.h"
#include "Constants.h"
#include "Sensors.h"
#include <stdio.h>

#define MAX_TEMP_BUF_SIZE  64

uint32_t baud[7]={
  2400, 4800, 9600, 19200, 38400, 57600, 115200};
int post_interval = 1000;

//char ssid[] = "WLAN_16D2"; //  your network SSID (name) 
//char pass[] = "Z1460809D16D2";    // your network password (use for WPA, or use as key for WEP)

int keyIndex = 0;            

#define ENTRY 10

static char *cmds_rn[10] = {
"set ip dhcp 1", 
strcat("set wlan ssid ", ssid),
strcat("set wlan phrase ", pass),
"set wlan auth 4",
"set wlan join 1",
"set sys autoconn 1",
"save",
"reboot"
}; 

// commans AT for set HLK-RM04 (not use because we use WiFiRM04 library)
/*static char *cmds_hlk[13]={
  "at+netmode=2",
  "at+wifi_conf=WLAN_16D2,wpa2_aes,Z1460809D16D2",
  //strcat("at+wifi_conf=", ssid) + strcat(",", "wpa2_aes") + strcat(",", pass) +"",
  "at+dhcpc=1",
  "at+remoteip=192-168.1.200",
  "at+remoteport=8000\r\n",
  "at+remotepro=tcp\r\n",
  "at+timeout=0\r\n",
  "at+mode=server\r\n",
  "at+uart=115200,8,n,1\r\n",
  "at+uartpacklen=64\r\n",
  "at+uartpacktimeout=10\r\n",
  "at+net_commit=1\r\n",
  "at+reconn=1\r\n"
  };*/

//****************************************************************START WIFI-HILINK
/*
int status = WL_IDLE_STATUS;
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
IPAddress server(192,168,1,200);  // numeric IP for Google (no DNS)
//char server[] = "192.168.1.200";    // name address for Google (using DNS)

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
WiFiRM04Client client;
String readString;
*/
//*******************************************************************END WIFI-HILINK

void Connection::begin(){ //init variables

#if DEBUG_MODE
  Serial.println(F("Connection to AP"));
#endif
  connStatus = false;
  httpStatus = false;
  wifiStatus = false;
  
  if(!attachWIFI()){
#if DEBUG_MODE
    Serial.println(F("\ Failed to connecting to AP..."));
#endif  
    wifiStatus = false;
  }else{
   wifiStatus = true;
#if DEBUG_MODE
    Serial.println(F("\ Connection stablished to AP..."));
#endif  
  }
  delay(1000);  
}

boolean Connection::connectTCP(const char *addr, int port) {

  if (connStatus) {
    disconnectTCP();
  } 

  if(enterCommandMode()){
    
    simpleWrite(F("open "));
    simpleWrite(addr);
    simpleWrite(F(" "));
    Serial1.print(port);
    simpleWriteln("");
    if (!waitResp("*OPEN*", 3000)){
#if DEBUG_MODE  
      Serial.println("no open TCP ...");
#endif      
      connStatus = false;

      return false;
    }else{
     connStatus = true;
     return true;
    } 
  }
  connStatus = false;  
  return false;
}

boolean Connection::disconnectTCP() {
  if (!connStatus) {
    return true;
  }
  simpleWriteln("close");
  if (waitResp("*CLOS*", 3000)) {
    connStatus = false;
    return true;
  }
  connStatus = false;
  return false;
}

boolean Connection::attachWIFI(){

#if DEBUG_MODE
 
#endif

#if TYPE_LINK_CONNECTION == 1
   Serial.println(F("entering command mode TYPE_LINK_CONNECTION == 1 "));
  if(enterCommandMode()){
#if DEBUG_MODE
    Serial.println(F("sending commads to module"));
#endif
    //sendCommands();
    simpleWriteln(F("set comm remote 0"));
    simpleWriteln(F("set wlan auth 0")); // 4
    simpleWriteln(F("set ip dhcp 1"));
    simpleWriteln(F("set ip proto 10"));
    simpleWriteln(F("set wlan ssid Lobo_Robot"));
    simpleWriteln(F("set wlan phrase loborobot2015")); 
    simpleWriteln(F("set wlan linkmon 5"));
    simpleWriteln(F("set wlan channel 0"));
    simpleWriteln(F("set wlan join 0")); //1
    simpleWriteln(F("save"));
    simpleWriteln(F("reboot"));

#if DEBUG_MODE

    Serial.println(F("set wlan auth 0")); // 4
    Serial.println(F("set ip dhcp 1"));
    Serial.println(F("set ip proto 10"));
    Serial.println(F("set wlan ssid Lobo_Robot"));
    Serial.println(F("set wlan phrase loborobot2015"));
    Serial.println(F("set comm remote 0"));
    Serial.println(F("set wlan linkmon 5"));
    Serial.println(F("set wlan channel 0"));
    Serial.println(F("set wlan join 0")); //1
    Serial.println(F("save"));
    Serial.println(F("reboot"));
    Serial.println(F("all commands were sended."));

#endif

    wifiStatus = true;
    return true;
  } else 
  {
    wifiStatus = false;
#if DEBUG_MODE
    Serial.println(F("couldn't enter in command mode"));
#endif
    Serial.println(F("debug repairing"));
    repair();
    return false;
  }

#elif TYPE_LINK_CONNECTION  == 2
  Serial.println(F("entering command mode TYPE_LINK_CONNECTION == 2 "));
  
  if(enterCommandModeHLK())
  {
    simpleWriteln(F("at+netmode=2"));
    simpleWriteln(F("at+wifi_conf=WLAN_16D2,wpa2_aes,Z1460809D16D2"));
    //simpleWriteln(F("at+" + strcat("wifi_conf=",ssid) + strcat(",","wpa2_aes") + strcat(",",pass) + ""));
    simpleWriteln(F("at+dhcpc=1"));
    simpleWriteln(F("at+remoteip=remote-device.dyndns-at-work.com"));
    simpleWriteln(F("at+remoteport=8080"));
    simpleWriteln(F("at+timeout=0")); 
    simpleWriteln(F("at+uart=115200,8,n,1"));
    simpleWriteln(F("at+uartpacklen=64"));
    simpleWriteln(F("at+net_commit=1"));
    simpleWriteln(F("at+reconn=1"));

#if DEBUG_MODE  
     Serial.println(F(" all commands were sended."));
#endif

    wifiStatus = true;
    return true;
    
  }else
  {
#if DEBUG_MODE
    Serial.println(F("couldn't enter in command mode HLK "));
#endif    
    wifiStatus = false;
    return false;

  }
  
#elif TYPE_LINK_CONNECTION  == 3
  Serial.println(F("entering command mode TYPE_LINK_CONNECTION == 3 "));
#endif

} 

boolean Connection::httpPOST(const char* server, int port, long *value_sensors){

  char itoaBuffer[8];
  byte n_of_at=0;
  byte i;
  
   
  while(n_of_at<3){
      
      if(!connectTCP(server, port)){

        #if DEBUG_MODE
          Serial.print(F("It can't connect to Remote Server Try # "));
          Serial.println(n_of_at);
        #endif
          n_of_at++;
          connStatus = false;            
      } else {
          n_of_at = 3;
          connStatus = true;
      }
        
  }
  
  if(!connStatus) return false;    
  
  connStatus = true;
  
  for(i=0; i<2; i++){
    if(i==4){
      Serial1.print(HTTPPOST[i]);
      //itoa(strlen(data),itoaBuffer,10);
      Serial1.println(itoaBuffer);
    } else
      Serial1.println(HTTPPOST[i]);
  }

  Serial1.println();

  for(i = 0; i<SENSORS_NUMBER; i++){
    Serial1.print(bodyJSON2[i]);
    Serial1.print(value_sensors[i]);
  }
  
  Serial1.print(bodyJSON2[i]);
  Serial1.print(value_sensors[i]);
  Serial1.print(bodyJSON2[i+1]);
  Serial1.print(value_sensors[i+1]);
  Serial1.print(bodyJSON2[i+2]);
  Serial1.print(value_sensors[i+2]);
  Serial1.println();

#if DEBUG_MODE

  for(i = 0; i<SENSORS_NUMBER; i++){
    Serial.println(bodyJSON2[i]);
    Serial.print(value_sensors[i]);  
  }
  
  Serial.print(bodyJSON2[i]);
  Serial.print(value_sensors[i]);
  
  Serial.print(bodyJSON2[i+1]);
  Serial.print(value_sensors[i+1]);
  
  Serial.print(bodyJSON2[i+2]);
  Serial.print(value_sensors[i+2]);
  Serial.println();
#endif
 //if(close()) return true;
  

  return true;
  
}
void Connection::sendCommands() {

  byte i = 0;
  while( i < ENTRY ) {
#if DEBUG_MODE  
    Serial.println(cmds_rn[i]);
#endif
    Serial1.println(cmds_rn[i]);
    delay(COMMAND_DELAY);
    
    while(Serial1.available()) {
#if DEBUG_MODE    
      Serial.println(Serial1.read());
#endif   
    }
    i++;
  }
  
} 

void Connection::simpleWrite(const __FlashStringHelper *cmd) {
  Serial1.print(cmd);
  delay(20);
  
} 

void Connection::simpleWriteln(const __FlashStringHelper *cmd) {
  Serial1.print(cmd);
  delay(20);
  Serial1.flush();
  Serial1.println();

} 

void Connection::simpleWrite(const char *cmd) {
  Serial1.print(cmd);
  delay(20);
} 

void Connection::simpleWriteln(const char *cmd) {
  Serial1.print(cmd);
  delay(20);
  Serial1.flush();
  Serial1.println();

} 

boolean Connection::waitResp(const char *toMatch, unsigned int timeOut = 1000) {
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
    Serial1.print("$$$");
    delay(COMMAND_MODE_GUARD_TIME);
    //Serial1.println();
    //Serial1.println();
   
    if (waitResp("CMD\r\n<", 1000))
    {
      return true;
    }
  
    return false;

}

boolean Connection::exitCommandMode() {
  
  simpleWriteln(F("exit"));
  
  if (waitResp("EXIT", 3000)){
      return true;
  }
  
  return false;
}


boolean Connection::reset() {
  enterCommandMode();
  simpleWriteln(F("factory R"));
  simpleWriteln(F("save")); 
  simpleWriteln(F("reboot"));
  
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

//***************************************************************END WIFI-RN171

/*
boolean Connection::enterCommandModeHLK(){

  pinMode(SCAPE_PIN, OUTPUT);
  digitalWrite(SCAPE_PIN, !SCAPE_PIN_ACTIVE);
  
  digitalWrite(SCAPE_PIN, SCAPE_PIN_ACTIVE);
  // according to user manual, delay should be > Tes time, so we plus 50ms here
  delay(TES_TIME_IN_MS+50);
  digitalWrite(SCAPE_PIN, !SCAPE_PIN_ACTIVE);
  
  return true;
}*/


void Connection::clearSerialRxData()
{
  while(Serial1.available())
    Serial1.read();
}

bool Connection::echoTest(long timeout)
{
  char buf[MAX_TEMP_BUF_SIZE+1];
  int i;
  char *token;
 
  clearSerialRxData();
  Serial1.println("#32767");
  Serial1.flush();
  Serial1.setTimeout(timeout);
 
  i = Serial1.readBytes(buf, MAX_TEMP_BUF_SIZE);
  
  if(i == 0) {
#if DEBUG_MODE  
  Serial.println("Echo no resp");
#endif
    return false;
  }
  
  buf[i] = NULL;
  token = buf;
  
  while(*token && *token++ != '#');
  
  int ret = strtol(token, NULL, 10);

  clearSerialRxData();
  
  return (ret == 32767)? true:false;
}

boolean Connection::enterCommandModeHLK()
{
  int retryCount = 0;
  // flush tx buffer's data first
  Serial1.flush();

  retry:
  digitalWrite(ESCAPE_PIN, ESCAPE_PIN_ACTIVE);
  // according to user manual, delay should be > Tes time, so we plus 50ms here
  delay(TES_TIME_IN_MS+50*(retryCount+1));
  digitalWrite(ESCAPE_PIN, !ESCAPE_PIN_ACTIVE);
  // MUST preserve all data in rx buffer here, or these data would be mixed with at command response
  // TODO: QueueList would cause heap fragmentation?
  if(retryCount == 0) {
    while(Serial1.available()) {
      Serial.println(Serial1.read());
    }
  }
  
  if(echoTest()) {
    modeAT = true;
    return true;
  }
  else
    modeAT = false;
  
  if(retryCount++ < 5)
    goto retry;
  
  return false;
}
/*
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
