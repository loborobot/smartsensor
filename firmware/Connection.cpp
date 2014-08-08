//#include <GSM.h>
#include "Connection.h"
#include "Constants.h"


//GSMClient client;
//GPRS gprs;
//GSM gsmAccess;

void Connection::begin(){ //init variables
  statusConn = OFFGPRS;
  /*
  Serial.println("Starting Arduino web client");
  notConnected = true;
  
  while(notConnected)
  {
    if((gsmAccess.begin(PINNUMBER)==GSM_READY) &
        (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD)==GPRS_READY)){
          IPAddress LocalIP = gprs.getIPAddress();
          Serial.print("Server IP address="); Serial.println(LocalIP);
          notConnected = false;
        }
    else
    {
      Serial.println("Not connected");
      delay(1000);
    }
  }

  Serial.println("connecting..");*/
}

/*
uint8_t Connection::send(){
  if(!notConnected){
    if (client.connect(HTTPGET[0], 8000))
      {
        Serial.println("connected");
        client.println("GET /api/devices/ HTTP/1.0");
        client.println("Accept: application/json");
        client.println("Authorization: Basic ZmluY3l0OmZpbmN5dA==");
        client.println();
        
        return 0;
      } 
      else
      {
        Serial.println("connection failed");
        return -1;
      }
  }
}

void Connection::read(){
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
}
*/
