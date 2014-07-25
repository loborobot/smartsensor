/*Lobo Robot Electronics*/

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <inttypes.h> //DHT22

#ifndef LoboRE_h
#define LoboRE_h

#define DHT11LIB_VERSION "0.3.2"
#define DHT22_ERROR_VALUE -99.5 //DHT22

#define GSMONLINE 1
#define GSMOFFLINE 0
#define GPRSCONNECTED 1
#define GPRSDISCONNECTED 0


//=============Start Sim900====
#define BUFFSIZE 1024
#include <../SoftwareSerial/SoftwareSerial.h>
#include <string.h>
#include <../GSM/GSM.h>
//#include "enums.h"
//#include "utils.h"

// Enumerador para deteccion de conexion de modulo GSM.
/*typedef enum {
  OFFLINE,
  ONLINE
} gsmStatus;

// Enumerador para deteccion de conexion GPRS.
typedef enum {
  DISCONNECTED,
  CONNECTED
} gprsStatus;*/
//====================End Sim900==============00

typedef enum
{
  DHT_ERROR_NONE = 0,
  DHT_BUS_HUNG,
  DHT_ERROR_NOT_PRESENT,
  DHT_ERROR_ACK_TOO_LONG,
  DHT_ERROR_SYNC_TIMEOUT,
  DHT_ERROR_DATA_TIMEOUT,
  DHT_ERROR_CHECKSUM,
  DHT_ERROR_TOOQUICK
} DHT22_ERROR_t;

class sDHT11
{
public:
    	int read(int pin);
	int humidity;
	int temperature;
};

class sDHT22
{
  private:
    uint8_t _bitmask;
    volatile uint8_t *_baseReg;
    unsigned long _lastReadTime;
    float _lastHumidity;
    float _lastTemperature;

  public:
    sDHT22(uint8_t pin);
    DHT22_ERROR_t readData(void);
    inline float getHumidity(){return _lastHumidity;};
    inline float getTemperatureC(){return _lastTemperature;};
    inline void clockReset(){_lastReadTime = millis();};
};

class sLDR
{
  private:
    uint8_t _pinLDR;
    float _valueLDR;
    float _lastValueLDR;
    float _minLDR;
    float _maxLDR;

  public:
    float readDataLDR(uint8_t pin);
    inline float getValueLDR(){return _lastValueLDR;};
};

class sUV
{
  private:
    uint8_t _pinUV;
    float _valueUV;
    float _lastValueUV;
    float _minUV;
    float _maxUV;

  public:
    float readDataUV(uint8_t pin);
    inline float getValueUV(){return _lastValueUV;};
};

class sSOUND
{
};

class sCO2
{
};

class sNO2
{
};

class sFLOWMETER
{
};

class Sim900
{
  private:
    char buffer[BUFFSIZE];
    int buffSize;
    volatile byte chr;
    volatile long lastInt;
    int currentStatusGSM;
    int currentStatusGPRS;
    volatile bool sendMessage;
    volatile bool startShutDown;
    SoftwareSerial mySerial;
    

  public:
    Sim900(int rx, int tx);
    void setupSim900(void);
    void shutDown(void);
    void shutdownGRPS(void);
    void setupGPRS(void);
    void GET(void);
    boolean checkForSIM900(void);
    void clearBuffer(void);
    void appendToBuffer(char c);
    void pushSlow(char* command,int charaterDelay=100,int endLineDelay=1000);
    bool waitForString(char* string, long waitForMilliseconds=5000);
    bool waitForString(char *string, int waitForMilliseconds=5000){return waitForString(string,(long)waitForMilliseconds);};
    void waitForData(int wait_delay);
    void bridge(void);
    inline int getCurrentStatusGSM(){return currentStatusGSM;};
    inline int getCurrentStatusGPRS(){return currentStatusGPRS;};
    inline bool getSendMessage(){return sendMessage;};
    inline bool getStartShutDown(){return startShutDown;};
    inline void setCurrentStatusGSM(int OnOff){currentStatusGSM = OnOff;};
    inline void setCurrentStatusGPRS(int OnOff){currentStatusGPRS = OnOff;};
    inline void setSendMessage(bool TrueFalse){sendMessage = TrueFalse;};
    inline void setStartShutDown(bool TrueFalse){startShutDown = TrueFalse;};

};

class reqHTTP
{
  private:
    String reqGet;
    String reqPost;
    String data;
    
  public:
    String get();
    String post();
    String dataJson(float humidity,float temperature, float light, float ultra_violet, float sound, float flowmeter, float volume, float nitro_dioxide, float carbon_monoxide);
    char* convertF2C(float val);
};
#endif
