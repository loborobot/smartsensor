#include "inetGSM.h"


int InetGSM::httpGET(const char* server, int port, const char* path, char* result, int resultlength)
{
  int length_write;
  char end_c[2];
  end_c[0]=0x1a;
  end_c[1]='\0';

  /*
  Status = ATTACHED.
  if(gsm.getStatus()!=GSM::ATTACHED)
    return 0;
  */
 
  if(!gsm.connectTCP(server, port)){
        #ifdef DEBUG_ON
                Serial.println("DB:NOT CONN");
        #endif  
    return 0;
        }
       
  gsm.SimpleWrite("GET ");
  gsm.SimpleWrite(path);
  gsm.SimpleWrite(" HTTP/1.0\nHost: ");
  gsm.SimpleWrite(server);
  gsm.SimpleWrite("\n");
  gsm.SimpleWrite("User-Agent: Arduino");
  gsm.SimpleWrite("\n\n");
  gsm.SimpleWrite(end_c);

  switch(gsm.WaitResp(10000, 100, "SEND")){
        case RX_TMOUT_ERR:
                return 0;
        break;
        case RX_FINISHED_STR_NOT_RECV:
                return 0;
        break;
  }

        #ifdef DEBUG_ON
                Serial.println("DB:SENT");
        #endif  
  int res= gsm.read(result, resultlength);

  //gsm.disconnectTCP();
 
  //int res=1;
  return res;
}

int InetGSM::httpPOST(const char* server, int port, const char* path, const char* parameters, char* result, int resultlength)
{
  char itoaBuffer[8];
  int num_char;
 Serial.println("debug1.");
  if (!gsm.connectTCP(server, port)){
    return 0;
        }

  Serial.println("debug1.");
  strcpy(_buffer,"POST ");
  strcat(_buffer,path);
  strcat(_buffer," HTTP/1.1\nHost: ");
  strcat(_buffer,server);

  strcat(_buffer,"Content-Type: application/json\n");
  strcat(_buffer,"Accept: application/json\n");
  strcat(_buffer,"Connection: close\n");

  strcat(_buffer,"\nContent-Length: ");
  //gsm.SimpleWrite("User-Agent: Arduino\n");

  itoa(strlen(parameters), itoaBuffer, 10);  
  strcat(_buffer,itoaBuffer);
  strcat(_buffer,"\n\n");
  strcat(_buffer,parameters);
  strcat(_buffer,"\n\n");
  Serial.println("debug1.");
  Serial.println(_buffer);  
  gsm.SimpleWrite(_buffer);
  
  Serial.println("debug2.");
  Serial.println(_buffer);

//  int res= gsm.read(result, resultlength);

  gsm.disconnectTCP();
  //return res;
   return 1;
}

int InetGSM::tweet(const char* token, const char* msg)
{
  //    gsm.httpPOST("arduino-tweet.appspot.com",80,"/update", "token=15514242-eWYAlMwjRQfrhnZxQiOfDXUpaYwjbSvMl1Nm5Qyg&status=Spam", buffer, 200);
  char shortbuf[200];
  strcpy(shortbuf,"token=");
  strcat(shortbuf,token);
  strcat(shortbuf,"&status=");
  strcat(shortbuf, msg);
  httpPOST("arduino-tweet.appspot.com",80,"/update",shortbuf, shortbuf, BUFFERSIZE);
}

int InetGSM::openmail(char* server, char* loginbase64, char* passbase64, char* from, char* to, char* subj)
{
        /*
          if (!gsm.connectTCP(server, 25))
        return 0;
   
    delay(1000);
    gsm.read(_buffer, BUFFERSIZE);    
    gsm.SimpleWrite("HELO\n");
    delay(500);
    gsm.read(_buffer, BUFFERSIZE);
    gsm.SimpleWrite("AUTH LOGIN\n");
    delay(500);
    gsm.read(_buffer, BUFFERSIZE);
    gsm.SimpleWrite(loginbase64);gsm.SimpleWrite("\n");
    delay(500);
    gsm.read(_buffer, BUFFERSIZE);
    gsm.SimpleWrite(passbase64);gsm.SimpleWrite("\n");
    delay(500);
    gsm.read(_buffer, BUFFERSIZE);
    gsm.SimpleWrite("MAIL FROM: ");gsm.SimpleWrite(from);gsm.SimpleWrite("\n");
    delay(500);
    gsm.read(_buffer, BUFFERSIZE);
    gsm.SimpleWrite("RCPT TO: ");gsm.Simple																Write(to);gsm.SimpleWrite("\n");
    delay(500);
    gsm.read(_buffer, BUFFERSIZE);
    gsm.SimpleWrite("Subject: ");gsm.SimpleWrite(subj);gsm.SimpleWrite("\n\n");
    return 1;
        */
}
int InetGSM::closemail()
{
        gsm.SimpleWrite("\n.\n");
        gsm.disconnectTCP();
        return 1;
}

