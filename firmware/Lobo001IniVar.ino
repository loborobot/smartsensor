/*void startVal()
{
 for (_i=0;_i<7;_i++)
    {
      pinMode(_led[_i], OUTPUT);//define leds tipe OUTPUT
    } 
}*/

void startIniSim900()
{
  mySerial.begin(9600);

  lastInt = millis();
  
  pinMode(3,INPUT_PULLUP); 
  attachInterrupt(0, pin3OnFalling, FALLING);
  
  pinMode(2,INPUT_PULLUP); 
  attachInterrupt(1, pin2OnFalling, FALLING);

  Serial.print("Arduino: Setup finished.\r\n");
}

void pin3OnFalling() {
  if((millis()-lastInt)>500) {
    startShutDown = true;
    lastInt = millis();
  }
}

void pin2OnFalling() {
  if((millis()-lastInt)>500) {
    sendMessage = true;
    lastInt = millis();
  }
}

void shutDown() {
  pushSlow("\r\n");
  pushSlow("AT+CPOWD=1\r\n");
}

void shutdownGRPS() {
  pushSlow("AT+CIPSHUT\r\n");
  waitForData(3000);
  pushSlow("AT+CGATT=0\r\n");
  waitForData(3000);
  currentGPRS = DISCONNECTED;
}

void setupGPRS() {
  pushSlow("\r\n");
  waitForData(1000);
  shutdownGRPS();
  pushSlow("AT+CGATT=1\r\n");
  if(!waitForString("OK",10000)) {
    shutdownGRPS();
    return;
  }
  pushSlow("AT+CSTT=\"internet.tigo.sv\",\"\",\"\"\r\n");
  if(!waitForString("OK",10000)) {
    shutdownGRPS();
    return;
  }
  pushSlow("AT+CIICR\r\n"); 
  if(!waitForString("OK",10000)) {
    shutdownGRPS();
    return;
  }
  pushSlow("AT+CIFSR\r\n"); 
  waitForData(1000);
  currentGPRS = CONNECTED;
}

void GET() {
  pushSlow("\r\n");
  waitForData(1000);
  pushSlow("AT+CIPSTART=\"TCP\",\"dev.teubi.co\",80\r\n");
  if(!waitForString("CONNECT OK",30000)) {
    return;
  }
  pushSlow("AT+CIPSEND\r\n");
  waitForData(1000);
  pushSlow("GET /hola.php HTTP/1.1\r\n");
  pushSlow("Host: dev.teubi.co\r\n");
  pushSlow("Connection: Keep-Alive\r\n");
  pushSlow("\r\n");
  pushSlow("\x1A");
  if(!waitForString("SEND OK",15000)) {
    return;
  }
  waitForData(15000);
  pushSlow("AT+CIPCLOSE\r\n");
  if(!waitForString("CLOSE OK",5000)) {
    return;
  }
}


boolean checkForSIM900() {
  // Figure out if already connected
  pushSlow("AT\r\n");
  if(!waitForString("OK",5000))
    return false;
  pushSlow("AT+CPIN?\r\n");
  if(!waitForString("+CPIN: READY",5000))
    return false;
  pushSlow("AT+CFUN?\r\n");
  if(!waitForString("+CFUN: 1",5000))
    return false;
  pushSlow("AT+CCALR?\r\n");
  if(!waitForString("+CCALR: 1",5000))
    return false;
  return true;
}

void stratSim900()
{
 if(currentStatus==OFFLINE) {
    Serial.write("Trying to detect SIM900 module\r\n");
    if(waitForString("Call Ready",5000)) {
      Serial.write("GSM Module detected. Current status: ONLINE\r\n");
      currentStatus=ONLINE;
    } else if(checkForSIM900()) {
      Serial.write("GSM Module already connected. Changing status to: ONLINE\r\n");
      currentStatus=ONLINE;
    }
  }
  if(currentStatus==ONLINE) {
    if(sendMessage) {
      if(currentGPRS==DISCONNECTED) {
        Serial.print("GPRS is disconnected. Starting up GPRS data.\r\n");
        Serial.print("Current request is going to be ignored.\r\n");
        setupGPRS();
        sendMessage = false;
      } else if(currentGPRS==CONNECTED) {
        Serial.print("DATA Interrupt received\r\n");
        GET();
        sendMessage = false;
      }
    }
    bridge();
  }
  if(startShutDown) {
    Serial.print("Shutdown interrupt received\r\n");
    shutDown();
    startShutDown = false;
    currentStatus = OFFLINE;
    currentGPRS = DISCONNECTED;
  }  
}


void clearBuffer() {
  for(int i=0;i<BUFFSIZE;i++) {
    buffer[i] = 0;
  }
  buffSize = 0;
}

void appendToBuffer(char c) {
  if(buffSize<BUFFSIZE) {
    buffer[buffSize++] = c;
  } else {
    Serial.print(buffer);
    clearBuffer();
    buffer[buffSize++] = c;
  }
}

void pushSlow(char* command,int charaterDelay,int endLineDelay) {
  for(int i=0; i<strlen(command); i++) {
    mySerial.write(command[i]);
    if(command[i]=='\n') {
      delay(endLineDelay);
    } else {
      delay(charaterDelay);
    }
  }
}

bool waitForString(char* string, long waitForMilliseconds) {
  clearBuffer();
  long startTime = millis();
  boolean found = false;
  Serial.print("Waiting for string: \"");
  Serial.print(string);
  Serial.print("\"\r\n");
  while((millis()-startTime)<waitForMilliseconds) {
    if (mySerial.available()) {
      chr = mySerial.read();
      Serial.write((char)chr);
      appendToBuffer((char)chr);
      if((char)chr=='\n') {
        if(strstr(buffer,string)!=NULL) {
          found = true;
          break;
        } else if(strstr(buffer,"ERROR")!=NULL) {
          found = false;
          break;
        }
        clearBuffer();
      }
    }
    if (Serial.available())
      mySerial.write(Serial.read());
  }
  return found;
}

bool waitForString(char *string, int waitForMilliseconds) {
  return waitForString(string,(long)waitForMilliseconds);
}


void waitForData(int wait_delay) {
  long startTime = millis();
  while((millis()-startTime)<wait_delay) {
    if (mySerial.available())
      Serial.write(mySerial.read());
    if (Serial.available()) {
      mySerial.write(Serial.read());
    }
  }
}

void bridge() {
  if (mySerial.available())
    Serial.write(mySerial.read());
  if (Serial.available()) {
    mySerial.write(Serial.read());
  }
}
