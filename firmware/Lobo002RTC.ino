/*void startRTC()
{
  RTC.begin();
  if (!RTC.isrunning()) 
    {
      Serial.println("RTC is NOT running!");
      RTC.adjust(DateTime(__DATE__, __TIME__));
    } 
}


void displayRTC()
{
    DateTime now = RTC.now();//get the current data
    
    delay(250);
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
}*/
