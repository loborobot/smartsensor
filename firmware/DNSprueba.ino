/* DynDNS
 
 created 2011
 by Boris Landoni
 
 This example code is in the public domain.
 
http://www.open-electronics.org
 
 
http://www.futurashop.it
 
 */
 
//http://username:password@members.dyndns.org/nic/update?hostname=yourhostname&myip=ipaddress
 
#include <WiFiRM04.h>
//#include <EthernetDNS.h>
#include <WString.h>
#include <EEPROM.h>
 
#define UPDATE_INTERVAL            30000    // if the connection is good wait 30 seconds before updating again - should not be less than 5
 
int ledR =  8;
int ledG =  9;
 
unsigned long pubblica=0;
 
byte mac[] = { 0x54,0x55,0x58,0x10,0x00,0x24 };
byte ip[] = { 192, 168, 0, 98 };
 
byte domainaddrip[]= { 208, 78, 70, 70 }; //http://checkip.dyndns.com/
byte ipAddr[4];
 
byte domaindydns[]= { 204, 13, 248, 112 }; // members.dyndns.org
char hostname[ ]   = "fincyt";
char userpwdb64[ ] = "fincyt";  //http://www.functions-online.com/base64_encode.html    use->   username:password
 
String ipadsl     = String(20);
String ipadslold  = String(20);
String inString   = String(51);
 
//Client client(server, 80);
const char* ip_to_str(const uint8_t*);
//void nameFound(const char* name, const byte ipAddr[4]);

 WiFiRM04Client client;
 WiFiRM04Client clientdyn;
 
//WiFiRM04Client client(domainaddrip, 80);
//WiFiRM04Client clientdyn(domaindydns, 80);
// 
void setup()
{
  pinMode(ledR, OUTPUT); 
  pinMode(ledG, OUTPUT); 
//  Ethernet.begin(mac, ip);
 
  //EthernetDNS.setDNSServer(dnsServerIp); 
  Serial.begin(9600);
 
  Serial.println("*****************************************Start***************************************************"); 
  delay(1000);
 
  //risolviip();
  //delay(1000);
}
 
void loop()
{
 
  if (millis() < pubblica) pubblica = millis();
 
      if ((millis() - pubblica) > UPDATE_INTERVAL){
          //Serial.print("Passati "); 
          //Serial.print(UPDATE_INTERVAL);
          //Serial.println(" ms"); 
          Serial.print("tempo trascorso dall'accensione ");          
          Serial.println(millis());
          Serial.println(pubblica);          
          pubblica = millis();  
 
          acquisisciip();          
      }
 
}
 
void acquisisciip()
{
 
  int timeout=0; 
  int skip=0;
  ipadsl="";
  inString="";
  digitalWrite(ledG, HIGH);
 
  Serial.print("connecting to ");
  //Serial.print(domain);
  //Serial.print(" = ip ");
  //Serial.println(ip_to_str(domainaddrip));
 
  if (client.connect(domainaddrip,80)) {
 
    Serial.println("connected");
    client.println("GET / HTTP/1.0");
    //client.println("GET / HTTP/1.0");
    client.print("HOST: ");
    client.println("www.dyndns.com");
    //client.println(ip_to_str(domainaddrip));
    client.println();
  } else {
    Serial.println("connection failed");
  }
 
  while (!client.available() && timeout<50) 
  {
    timeout++;
    Serial.print("Time out ");
    Serial.println(timeout);
    delay(100);
  }
 
  while (client.available()) 
  {
      //Serial.println("client.available");
      //if (client.available()) 
      //{
        char c = client.read();
        //Serial.print(c);
        skip++;
        if (skip>220)
        {
          if ((inString.length())<50)
           //inString.append(c);      
           inString = inString + c;
        }
      //}
 
  }         
 
  client.flush();
 
      if ((inString.length())>5)   
      {
          //Serial.print("inString: ");
          //Serial.println(inString); 
          if (inString="Address")
            {    
              int from=(inString.indexOf("Address")+9);
              int lunghe=inString.length();
              int to=(lunghe-16);
              /*Serial.print("Lunghezza instring    = ");
              Serial.println(lunghe);  
              Serial.print("Posizione address + 9 = ");
              Serial.println(from);  
              Serial.print("Posizione fine   - 16 = ");
              Serial.println(to);  
              */
              //strcpy(ipadsl,(inString.substring((inString.indexOf("Address")+9),(inString.length()-16))));
              ipadsl=inString.substring(from,to); 
              //Serial.print("Lunghezza ipadsl      = ");
              //Serial.println(ipadsl.length()); 
              Serial.print("IP rilevato dal sito: >");
              Serial.print(ipadsl);          
              Serial.println("<");
 
            }    
      }              
 
   if (!client.connected()) 
   {
 
        Serial.println("disconnecting.");
        client.stop();
        delay (1000);
 
        Serial.print("lunghezza IP ");
        //Serial.println(strlen(ipadsl)); 
        Serial.println(ipadsl.length());
 
     //   if ((strlen(ipadsl))!=0)
        if (ipadsl!=0)

        { 
              Serial.print("IP nuovo  : >");
              Serial.print(ipadsl);          
              Serial.println("<");
 
              for (int i=0; i<(ipadsl.length()); i++) {    //salva in memoria l'indirizzo acquisito
                ipadslold[i] = EEPROM.read(i);
                //Serial.print("carattere  : >");
                //Serial.print(ipadslold[i]);          
                //Serial.println("<");
              } 
 
              Serial.print("IP vecchio: >");
              Serial.print(ipadslold);          
              Serial.println("<");
 
              if (ipadsl!=ipadslold)
              {
                Serial.println("IP diverso PUBBLICO");
                  digitalWrite(ledR, HIGH);
                pubblicaip(); 
                  digitalWrite(ledR, LOW);
              } 
              else
              {            
                Serial.println("IP uguale");    
 
              }
                digitalWrite(ledG, LOW);
 
              //ipadslold="";
              //ipadslold.append(ipadsl);
              //ipadslold="";
              //Serial.print("IP vecchio azzerato: >");
              //Serial.print(ipadslold);          
              //Serial.println("<");
              //strcpy(ipadslold,ipadsl);
              for (int i=0; i<(ipadsl.length()); i++) {    //salva in memoria l'indirizzo acquisito
                EEPROM.write(i, ipadsl[i]); 
              } 
 
        }
 
   }
 
}
 
void pubblicaip()
{
  int timeout=0;
  Serial.print("connecting to ");
  Serial.println(ip_to_str(domaindydns));
 
  Serial.print("IP da pubblicare ");
  Serial.println(ipadsl);  
 
if (clientdyn.connect(domaindydns, 80)) {
    Serial.println("connected");
    clientdyn.print("GET /nic/update?hostname=");
    clientdyn.print(hostname);
    clientdyn.print("&myip=");
    clientdyn.print(ipadsl);
    clientdyn.println(" HTTP/1.0 ");
    //clientdyn.println("Host: members.dyndns.org ");
 
    clientdyn.print("Host: ");
    clientdyn.println(ip_to_str(domaindydns));
 
    clientdyn.print("Authorization: Basic ");
    clientdyn.println(userpwdb64);
 
    clientdyn.println("User-Agent: futura - arduinodydns - 1.1");
    clientdyn.println();
  } else {
    Serial.println("connection failed");
  }
 
 //   delay(3000);
 
  while (!clientdyn.available() && timeout<50) 
  {
    timeout++;
    Serial.print("Time out ");
    Serial.println(timeout);
    delay(100);
  }
 
  while (clientdyn.available()) 
  {
      if (clientdyn.available()) 
      {
        char c = clientdyn.read();
        Serial.print(c);
      }
  }
 
  if (!clientdyn.connected()) 
  {
        Serial.println();
        Serial.println("disconnecting.");
        clientdyn.stop();
  }
 
}
 
// This is just a little utility function to format an IP address as a string.
const char* ip_to_str(const uint8_t* ipAddr)
{
  static char buf[16];
  sprintf(buf, "%d.%d.%d.%d\0", ipAddr[0], ipAddr[1], ipAddr[2], ipAddr[3]);
  return buf;
}
