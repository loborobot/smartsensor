  #include <Wire.h>
//#include <SD.h>
#include <RTClib.h>

#include "Constants.h"
#include "Core.h"

RTC_DS1307 RTC; // object to RTC

//DateTime now;
//File logFile;
// 27.608

void Core::begin(){ //init variables
  //pinMode(10, OUTPUT);
  //if (!SD.begin(4)) {
    //Serial.println("SD Card initialization failed");
    //return;
  //}
  //Serial.println("initialization done.");
  //logFile = SD.open("log.txt", FILE_WRITE);
  //logFile.close();
  //pconn1__.begin();  

  
  Wire.begin();
  RTC.begin();
  // run clock with time PC
  if (! RTC.isrunning()) {
    //RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  RTC.adjust(DateTime(__DATE__, __TIME__));
}

/*void Resources::execute(){ // init program
 writeSD();
}
*/


// Struct format date end time (read)
String Core::getRTC(){

  String date;
  DateTime now = RTC.now();// funcion que regresa tiempo
  //  2014-07-15T22:02:27.321Z
  date += now.year();
  date += "-";
  date += now.month();
  date += "-";
  date += now.day();
  date += "T";
  date += now.hour();
  date += ":";
  date += now.minute();
  date += ":";
  date += now.second();
  date += ".000Z";
  
  return date;
}

//printer data in SD card
void Core::writeSD(){
  /*
  if(!pconn1__.statusConn){;
  logFile = SD.open("log.txt", FILE_WRITE);
    if(logFile){
      //Serial.print("Writing to test.txt...");
      logFile.print(now.year());logFile.print("/");logFile.print(now.month());logFile.print("/");logFile.print(now.day());logFile.print(" ");
      logFile.print(now.hour());logFile.print(":");logFile.print(now.minute());logFile.print(":");logFile.print(now.second());logFile.print(" ");
      logFile.print(psens1__._humidity);logFile.print(" ");logFile.print(psens1__._valueLDR);logFile.print(" ");logFile.print(psens1__._valueUV);
      logFile.print(" ");logFile.print(psens1__._sound);logFile.print(" ");logFile.print(psens1__._flowmeter);logFile.print(" ");
      logFile.print(psens1__._volume);logFile.print(" ");logFile.print(psens1__._CO2);logFile.print(" ");logFile.print(psens1__._NO2);
      }
      logFile.close();
    }
    */
}

/*void setDS3231time(const DateTime &dt)
{
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(dt.second())); // set seconds
  Wire.write(decToBcd(dt.minute())); // set minutes
  Wire.write(decToBcd(dt.hour)); // set hours
  Wire.write(decToBcd(0)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dt.day())); // set date (1 to 31)
  Wire.write(decToBcd(dt.month())); // set month
  Wire.write(decToBcd(dt.year()-2000)); // set year (0 to 99)
  Wire.endTransmission();
}*/
/*
void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
dayOfMonth, byte month, byte year)
{
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}

void readDS3231time(byte *second,byte *minute,
                    byte *hour, byte *dayOfWeek,byte *dayOfMonth,
                    byte *month, byte *year)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}

char time[TIME_BUFFER_SIZE] // 2015-03-31T19:07:27.321Z
void displayTime(char *time)
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,&year);
  // send it to the serial monitor
  //time[i]= year/mod;
  // convert the byte variable to a decimal number when displayed
  Serial.print(":");
  if (minute<10)
  {
    Serial.print("0");
  }
  Serial.print(minute, DEC);
  Serial.print(":");
  if (second<10)
  {
    Serial.print("0");
  }
  Serial.print(second, DEC);
  Serial.print(" ");
  Serial.print(dayOfMonth, DEC);
  Serial.print("/");
  Serial.print(month, DEC);
  Serial.print("/");
  Serial.print(year, DEC);
  Serial.print(" Day of week: ");
  switch(dayOfWeek){
  case 1:
    Serial.println("Sunday");
    break;
  case 2:
    Serial.println("Monday");
    break;
  case 3:
    Serial.println("Tuesday");
    break;
  case 4:
    Serial.println("Wednesday");
    break;
  case 5:
    Serial.println("Thursday");
    break;
  case 6:
    Serial.println("Friday");
    break;
  case 7:
    Serial.println("Saturday");
    break;
  }
}*/
