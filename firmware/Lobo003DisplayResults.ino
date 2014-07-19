/*void resultDisplay()
{
    
    if (Serial.available() > 0) //read serialport
    {
      _dato1 = Serial.read();// read byte into
    }      
    
    for(_i=0;_i<=5;_i++)
    {
      _Value[_i]=analogRead(_i);//read analog 
    }
    
    for(_i=0;_i<=5;_i++)
    {
      _val[_i]=map(_Value[_i], 0, 1023, 0, 255);// remap 0 / 255 de la variable Value5
    }
    
    for(_i=0;_i<=5;_i++)
    {
      _into[_i]=map(_Value[_i], 0, 1023, 0, 100);//convert 0-100
    }
    
    if (_dato1 == 65)
    {
      digitalWrite(_led[6],LOW);//stop led 13
    }
    else if (_dato1 == 66)
    {
      digitalWrite(_led[6],HIGH);//start led 13
    }
    
    for(_i=0;_i<6;_i++)
    {
      analogWrite(_led[_i], _val[_i]);//start leds
    }
     
    //Sending data for serialport
    for (_i=0;_i<=5;_i++)
    {
      delay(250);
      Serial.print("Sensor ");
      Serial.print(_i);
      Serial.print(": ");
      Serial.print(_into[_i], DEC);
      Serial.println("% ");
      delay(250);
    }
  
    delay(250);
    
}*/
