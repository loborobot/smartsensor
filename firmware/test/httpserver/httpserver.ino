/*
 * WiFlyHQ Example httpserver.ino
 *
 * This sketch implements a simple Web server that waits for requests
 * and serves up a small form asking for a username, then when the
 * client posts that form the server sends a greeting page with the
 * user's name and an analog reading.
 *
 * This sketch is released to the public domain.
 *
 */

 /* Notes:
  * Uses chunked message bodies to work around a problem where
  * the WiFly will not handle the close() of a client initiated
  * TCP connection. It fails to send the FIN to the client.
  * (WiFly RN-XV Firmware version 2.32).
  */

/* Work around a bug with PROGMEM and PSTR where the compiler always
 * generates warnings.
 */
#undef PROGMEM 
#define PROGMEM __attribute__(( section(".progmem.data") )) 
#undef PSTR 
#define PSTR(s) (__extension__({static prog_char __c[] PROGMEM = (s); &__c[0];})) 

#include <WiFlyHQ.h>

//#include <SoftwareSerial.h>

//SoftwareSerial wifiSerial(8,9);

//#include <AltSoftSerial.h>
//AltSoftSerial wifiSerial(8,9);

WiFly wifly;

const  char mySSID[] = "Lobo_Robot"; 
const  char myPassword[] = "loborobot2015";   


//static  char ssid[] = "WLAN_A490"; 
//static  char pass[] = "Z2C26C54CA490";   

void sendIndex();
void sendGreeting(char *name);
void send404();

char buf[120];

#define POST_INTERVAL 10000

void setup()
{
    Serial.begin(9600);
    Serial1.begin(9600);
    
    Serial.println(F("Starting"));
    Serial.print(F("Free memory: "));
    Serial.println(wifly.getFreeMemory(),DEC);

    //wifiSerial.begin(19200);
    if (!wifly.begin(&Serial1, &Serial)) {
        Serial.println(F("Failed to start wifly"));
	wifly.terminal();
    }

    /* Join wifi network if not already associated */
    if (!wifly.isAssociated()) {
	/* Setup the WiFly to connect to a wifi network */
	Serial.println(F("Joining network"));
	wifly.setSSID(mySSID);
	wifly.setPassphrase(myPassword);
	wifly.enableDHCP();
	wifly.save();

	if (wifly.join()) {
	    Serial.println(F("Joined wifi network"));
	} else {
	    Serial.println(F("Failed to join wifi network"));
	    wifly.terminal();
	}
    } else {
        Serial.println(F("Already joined network"));
    }

    wifly.setBroadcastInterval(0);	// Turn off UPD broadcast

    //wifly.terminal();

    Serial.print(F("MAC: "));
    Serial.println(wifly.getMAC(buf, sizeof(buf)));
    Serial.print(F("IP: "));
    Serial.println(wifly.getIP(buf, sizeof(buf)));

    wifly.setDeviceID("Wifly-device");

    if (wifly.isConnected()) {
        Serial.println(F("Old connection active. Closing"));
	wifly.close();
    }

    wifly.setProtocol(WIFLY_PROTOCOL_TCP);
    
    if (wifly.getPort() != 80) {
        wifly.setPort(80);
	/* local port does not take effect until the WiFly has rebooted (2.32) */
	wifly.save();
	Serial.println(F("Set port to 80, rebooting to make it work"));
	wifly.reboot();
	delay(3000);
    }
    Serial.println(F("Ready"));
}


void readSerials(){
  
 unsigned long startTime = millis();
 unsigned long stopTime = startTime + POST_INTERVAL;
  
 
  while (millis() < stopTime){
    // wait for incomming data
   if (wifly.available() > 0) {

        /* See if there is a request */
	if (wifly.gets(buf, sizeof(buf))) {
            
            Serial.println("Raw data: ");
            Serial.println(buf);
            
	    if (strncmp_P(buf, PSTR("GET / "), 6) == 0) {
		/* GET request */
		Serial.println(F("Got GET request"));
		while (wifly.gets(buf, sizeof(buf)) > 0) {
		    /* Skip rest of request */
		}
                //wifly.flushRx();
		sendIndex();
		Serial.println(F("Sent index page"));
	    } else if (strncmp_P(buf, PSTR("POST /"), 6) == 0) {
	        /* Form POST */
	        char username[16];
	        Serial.println(F("Got POST"));

                byte i=0;
                while(i<9){
                   wifly.gets(buf, sizeof(buf));
                   Serial.println(buf);
                   i++;
                }

		/* Get posted field value 
		if (wifly.match(F("enabled="))) {
		    wifly.gets(username, sizeof(username));
		    wifly.flushRx();		// discard rest of input
		    sendGreeting(username);
		    Serial.println(F("Sent greeting page"));
		}*/
                send200();
	    } else if( strncmp_P(buf, PSTR("HTTP/1.0 200 OK"),15) == 0){
              Serial.println("got OK");
            } else {
	        /* Unexpected request */
		Serial.print(F("Unexpected: "));
		Serial.println(buf);
		//wifly.flushRx();		// discard rest of input
		Serial.println(F("Sending 404"));
		send404();
	    }

            wifly.flushRx(); // YA NO IMPORTA LO QUE VIENE LUEGO lo descarto con flush
	}
    } // end if
   

  } 
 
  startTime = 0;
  stopTime = 0;

}

char bodyPOST[]="{\"tmp\": \"0\", \"hum\": \"0\", \"lig\": \"0\", \"uv\": \"0\", \"snd\": \"0\", \"flm\": \"0\", \"no2\": \"0\", \"co\": \"0\", \"id\": \"166d77ac1b46a1ec38aa35ab7e628ab5\", \"tstamp\": \"2015-02-10T22:02:27.321Z\"}";    
char itoaBuffer[8];
void loop()
{
   
    //if(wifly.open("smartsensor-test.herokuapp.com", 80)){
      if(wifly.open("192.168.1.34", 8080)){
    
      Serial.println(F("Posting... "));
      
      wifly.println("POST /webapi/update/ HTTP/1.1");
      wifly.println("Host: 192.168.1.34");
      wifly.println("User-Agent: SmartSensor");
      wifly.println("Content-Type: application/json");
      wifly.println("Accept: application/json");
      //wifly.println("Connection: close");
      wifly.println("Transfer-Encoding: chunked");
      itoa(strlen(bodyPOST), itoaBuffer, 10);
      wifly.print("Content-Length: ");
      wifly.println(itoaBuffer);
      wifly.println("");
      wifly.println("");
      wifly.print(bodyPOST);
      Serial.print(F("Posted... "));
      Serial.print(itoaBuffer);
      Serial.println(F(" bytes"));
      Serial.println(bodyPOST);
      
      /*if(wifly.close());
      {
        Serial.println(F("Failed to close tcp"));
        
      }*/
      
    }else{
      Serial.print(F("Failed to open tcp tried# "));
    }
  
    readSerials();
}

/** Send an index HTML page with an input box for a username */
void sendIndex()
{
    /* Send the header direclty with print */
    wifly.println(F("HTTP/1.1 200 OK"));
    wifly.println(F("Content-Type: text/html"));
    wifly.println(F("Transfer-Encoding: chunked"));
    wifly.println();

    /* Send the body using the chunked protocol so the client knows when
     * the message is finished.
     * Note: we're not simply doing a close() because in version 2.32
     * firmware the close() does not work for client TCP streams.
     */
    wifly.sendChunkln(F("<html>"));
    wifly.sendChunkln(F("<title>WiFly HTTP Server Example</title>"));
    wifly.sendChunkln(F("<h1>"));
    wifly.sendChunkln(F("<p>Hello</p>"));
    wifly.sendChunkln(F("</h1>"));
    wifly.sendChunkln(F("<form name=\"input\" action=\"/\" method=\"post\">"));
    wifly.sendChunkln(F("Username:"));
    wifly.sendChunkln(F("<input type=\"text\" name=\"user\" />"));
    wifly.sendChunkln(F("<input type=\"submit\" value=\"Submit\" />"));
    wifly.sendChunkln(F("</form>")); 
    wifly.sendChunkln(F("</html>"));
    wifly.sendChunkln();
}

/** Send a greeting HTML page with the user's name and an analog reading */
void sendGreeting(char *name)
{
    /* Send the header directly with print */
    wifly.println(F("HTTP/1.1 200 OK"));
    wifly.println(F("Content-Type: text/html"));
    wifly.println(F("Transfer-Encoding: chunked"));
    wifly.println();

    /* Send the body using the chunked protocol so the client knows when
     * the message is finished.
     */
    wifly.sendChunkln(F("<html>"));
    wifly.sendChunkln(F("<title>WiFly HTTP Server Example</title>"));
    /* No newlines on the next parts */
    wifly.sendChunk(F("<h1><p>Hello "));
    wifly.sendChunk(name);
    /* Finish the paragraph and heading */
    wifly.sendChunkln(F("</p></h1>"));

    /* Include a reading from Analog pin 0 */
    snprintf_P(buf, sizeof(buf), PSTR("<p>Analog0=%d</p>"), analogRead(A0));
    wifly.sendChunkln(buf);

    wifly.sendChunkln(F("</html>"));
    wifly.sendChunkln();
}

/* Send a 404 error */
void send404()
{
    wifly.println(F("HTTP/1.1 404 Not Found"));
    wifly.println(F("Content-Type: text/html"));
    wifly.println(F("Transfer-Encoding: chunked"));
    wifly.println();
    wifly.sendChunkln(F("<html><head>"));
    wifly.sendChunkln(F("<title>404 Not Found</title>"));
    wifly.sendChunkln(F("</head><body>"));
    wifly.sendChunkln(F("<h1>Not Found</h1>"));
    wifly.sendChunkln(F("<hr>"));
    wifly.sendChunkln(F("</body></html>"));
    wifly.sendChunkln();
}

void send200()
{
    wifly.println(F("HTTP/1.1 200 OK"));
    wifly.println(F("Content-Type: text/html"));
    wifly.println(F("Transfer-Encoding: chunked"));
    wifly.println();

}
