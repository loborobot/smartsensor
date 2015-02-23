#ifndef SIMCOM900_H
#define SIMCOM900_H
#include <SoftwareSerial.h>
#include "GSM.h"

class SIMCOM900 : public virtual GSM
{

  private:
    int configandwait(char* pin);
    int setPIN(char *pin);
    int changeNSIPmode(char);

  public:
    SIMCOM900();
    ~SIMCOM900();
    int getCCI(char* cci);
        int getIMEI(char* imei);
    int sendSMS(const char* to, const char* msg);
    int attachGPRS(char* domain, char* dom1, char* dom2);
    boolean readSMS(char* msg, int msglength, char* number, int nlength);
    boolean readCall(char* number, int nlength);
    boolean call(char* number, unsigned int milliseconds);
    int dettachGPRS();
    int connectTCP(const char* server, int port);
    int disconnectTCP();
    int connectTCPServer(int port);
    boolean connectedClient();
    virtual int read(char* result, int resultlength);
        virtual uint8_t read();
    int readCellData(int &mcc, int &mnc, long &lac, long &cellid);
    void SimpleRead();
    void WhileSimpleRead();
    void SimpleWrite(char *comm);
    void SimpleWrite(char const *comm);
        void SimpleWriteWOln(char *comm);
    void SimpleWrite(int comm);
};

extern SIMCOM900 gsm;

#endif

