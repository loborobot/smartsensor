/*
void startSensorDHT22()
{
  DHT22_ERROR_t errorCode;
  delay(2000);

  Serial.print("Requesting data...");
  errorCode = p_DHT22.readData();
  switch(errorCode)
  {
    case DHT_ERROR_NONE:
      Serial.print("Got Data ");
      Serial.print(p_DHT22.getTemperatureC());
      Serial.print("C ");
      Serial.print(p_DHT22.getHumidity());
      Serial.println("%");
      break;

    case DHT_ERROR_CHECKSUM:
      Serial.print("check sum error ");
      Serial.print(p_DHT22.getTemperatureC());
      Serial.print("C ");
      Serial.print(p_DHT22.getHumidity());
      Serial.println("%");
      break;

    case DHT_BUS_HUNG:
      Serial.println("BUS Hung ");
      break;

    case DHT_ERROR_NOT_PRESENT:
      Serial.println("Not Present ");
      break;

    case DHT_ERROR_ACK_TOO_LONG:
      Serial.println("ACK time out ");
      break;

    case DHT_ERROR_SYNC_TIMEOUT:
      Serial.println("Sync Timeout ");
      break;

    case DHT_ERROR_DATA_TIMEOUT:
      Serial.println("Data Timeout ");
      break;

    case DHT_ERROR_TOOQUICK:
      Serial.println("Polled to quick ");
      break;
  }
}


void startSensorDHT11()
{
  //Serial.println("\n");

  int chk = p_sensors.read(DHT11PIN);
  //Serial.print("Read sensor: ");
  switch (chk)
  {
    case 0: //Serial.println("OK");
            Serial.print("RH: ");
            Serial.print(((float)p_sensors.humidity11)/10, 2);Serial.print(" %");
            Serial.print(" Temperature: ");
            Serial.print(((float)p_sensors.temperature11)/10, 2);Serial.println(" (oC)");
            delay(2000);break;
    case -1: Serial.println("Checksum error"); break;
    case -2: Serial.println("Time out error"); break;
    default: Serial.println("Unknown error"); break;
  }
}

void startSensorLDR()
{
  light = p_sensors.readDataLDR(LDR_PIN);
  if(light == -2)
  {
    Serial.print("LDR (IN): ");
    Serial.print(p_sensors.getValueLDR());
    Serial.print (" LDR (OUT): Maxima ilumicacion");
  }
  else
  {  
    Serial.print("LDR (IN): ");
    Serial.print(p_sensors.getValueLDR());
    Serial.print (" LDR (OUT): ");
    Serial.print(light);  
    Serial.println(" lux");
  }
  delay(1000);
}


void startSensorUV()
{
  ultra_violet = p_sensors.readDataUV(UV_PIN);
  Serial.print ("UV (IN): ");
  Serial.print(p_sensors.getValueUV());
  Serial.print (" UV (OUT): ");
  Serial.print(ultra_violet); 
  Serial.println(" UVI");
  delay(1000);  
}


void startSensorSound()
{
  sound = analogRead(SOUND_PIN);
  //valorSOUND = constrain(valorSOUND, 0, 100); // value normalize
  sound = map(sound, 0, 1023, 0, 100);
  Serial.print("SOUND: ");
  Serial.println(sound);
  delay(250);
}


void startFlowMeter()
{
  NbTopsFan = 0;   //Set NbTops to 0 ready for calculations
  //sei();      //Enables interrupts
  delay(1000);   //Wait 1 second
  //cli();      //Disable interrupts
  flowmeter = ((NbTopsFan * 60 / 5.5)/60); //(Pulse frequency x 60) / 5.5Q, = flow rate in L/hour 
  volume = ((flowmeter * 60)/1000);
  //Calc = (Calc / 60);
  //Serial.print (Calc, DEC); //Prints the number calculated above
  Serial.print((float)flowmeter, 2);
  Serial.print (" L/min\r\n"); //Prints "L/hour" and returns a  new line
  Serial.print((float)volume, 2);
  Serial.print (" m3/min\r\n"); //Prints "L/hour" and returns a  new line
}*/
