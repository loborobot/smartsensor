void startSensorDHT11()
{
  //Serial.println("\n");

  int chk = p_dht11.read(DHT11PIN);

  //Serial.print("Read sensor: ");
  switch (chk)
  {
    case 0: //Serial.println("OK");
            Serial.print("RH: ");
            Serial.print(((float)p_dht11.humidity)/10, 2);Serial.print(" %");
            Serial.print(" Temperature: ");
            Serial.print(((float)p_dht11.temperature)/10, 2);Serial.println(" (oC)");
            delay(2000);break;
    case -1: Serial.println("Checksum error"); break;
    case -2: Serial.println("Time out error"); break;
    default: Serial.println("Unknown error"); break;
  }
}


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

void startSensorLDR()
{
  valorLDR = p_LDR.readDataLDR(LDR_PIN);
  if(valorLDR == -2)
  {
    Serial.print("LDR (IN): ");
    Serial.print(p_LDR.getValueLDR());
    Serial.print (" LDR (OUT): Maxima ilumicacion");
  }
  else
  {  
    Serial.print("LDR (IN): ");
    Serial.print(p_LDR.getValueLDR());
    Serial.print (" LDR (OUT): ");
    Serial.print(valorLDR);  
    Serial.println(" lux");
  }
  delay(1000);
}

void startSensorUV()
{
  valorUV = p_UV.readDataUV(UV_PIN);
  Serial.print ("UV (IN): ");
  Serial.print(p_UV.getValueUV());
  Serial.print (" UV (OUT): ");
  Serial.print(valorUV); 
  Serial.println(" UVI");
  delay(1000);  
}

void startSensorSound()
{
  valorSOUND = analogRead(SOUND_PIN);
  Serial.print("SOUND: ");
  Serial.println(valorSOUND);
  delay(250);
}
