

#include "DHT.h"

#define DHTPIN 2             // what pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT22

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 9, 7, 6, 5);

volatile int NbTopsFan; //measuring the rising edges of the signal
int Calc;                               
int hallsensor = 3;    //The pin location of the sensor




DHT dht(DHTPIN, DHTTYPE);
// These constants won't change.  They're used to give names
// to the pins used:
const int analogInPin   = A0;  // IN LDR
//const int analogOutPin  = 9;   // OUT LDR
const int analogInPin1  = A1;  // IN NO2
//const int analogOutPin1 = 10;  // OUT NO2
const int analogInPin2  = A2;  // IN CO
//const int analogOutPin2 = 11;  // OUT CO
const int analogInPin4  = A4;  // IN Sonido
//const int analogOutPin4 = 12;  // OUT Sonido
const int analogInPin5  = A5;  // IN UV
//const int analogOutPin5 = 13;  // OUT UV

// The setup() method runs once, when the sketch starts


//LDR
int sensorValue = 0;        // value read from the pot
float Value     = 0;        // valor intercalo
float Valuecc   = 0;        // valor intercalo
//int outputValue = 0;        // value output to the PWM (analog out)
//NO2
int sensorValue1 = 0;        // value read from the pot
int Value1       = 0;           // valor intercalo
//int outputValue1 = 0;        // value output to the PWM (analog out)
//CO
int sensorValue2 = 0;        // value read from the pot
int Value2       = 0;           // valor intercalo
//int outputValue2 = 0;        // value output to the PWM (analog out)
//Sonido
int sensorValue4 = 0;        // value read from the pot
double Value4b   = 0;           // valor intercalo
double Value4a   = 0;           // valor intercalo
double Value4    = 0;           // valor intercalo
//int outputValue4 = 0;        // value output to the PWM (analog out)
//UV
int sensorValue5 = 0;        // value read from the pot
int Value5       = 0;           // valor intercalo
//int outputValue5 = 0;        // value output to the PWM (analog out)

//caudalimetro

void rpm ()     //This is the function that the interupt calls 
{ 
 NbTopsFan++;  //This function measures the rising and falling edge of the 

                //hall effect sensors signal
} 
//end caudalimetro


// The setup() method runs once, when the sketch starts

void setup() {
  // initialize serial communications at 9600 bps:
    lcd.begin(16, 2);
  pinMode(hallsensor, INPUT); //initializes digital pin 2 as an input

  Serial.begin(9600); 
  Serial.println("DHTxx test!");
  dht.begin();
  //caudalimetro
  //initialised,
   attachInterrupt(0, rpm, RISING); //and the interrupt is attached

}

void loop() {

  sensors();
  flujometro();

//****************************lcd-begin****************************

//  lcd.print("S.LDR = " );                       
//  lcd.print(sensorValue);      
//  lcd.print(" LDR = ");      
//  lcd.print(Value);
//  lcd.print(" lux " ); 
//  //E.LDR
//  
//  //B.NO2
//  lcd.print(" S.NO2 = " );                       
//  lcd.print(sensorValue1);      
//  lcd.print(" NO2 = ");      
//  lcd.print(Value1);
//  lcd.print(" ppb " ); 
//  //E.NO2
//  
//  //B.CO
//  lcd.print(" S.CO = " );                       
//  lcd.print(sensorValue2);      
//  lcd.print(" CO = ");      
//  lcd.print(Value2);
//  lcd.print(" ppm " ); 
//  //E.CO
//  
//  //B.Sonido
//  lcd.print(" S.Sonido = " );                       
//  lcd.print(sensorValue4);      
//  lcd.print(" Ruido = ");      
//  lcd.print(Value4);
//  lcd.print(" db " ); 
//  //E.Sonido
//  
//  //B.UV
//  lcd.print(" S.UV = " );                       
//  lcd.print(sensorValue5);      
//  lcd.print(" UV = ");      
//  lcd.print(Value5);
//  lcd.print(" UVI " ); 
//  //E.UV
//    
//  //B.Caudalimetro
//
//  //E.Caudalimetro
//
// //B. T&H
//  lcd.print("Humidity: "); 
//  lcd.print(h);
//  lcd.print(" % ");
//  lcd.print("Temperature: "); 
//  lcd.print(t);
//  lcd.println(" C ");
//  //E. T&H

//****************************lcd-end****************************
  
}

void flujometro(){
//
 NbTopsFan = 0;      //Set NbTops to 0 ready for calculations
 sei();            //Enables interrupts
 delay (1000);      //Wait 1 second
 cli();            //Disable interrupts
 Calc = (NbTopsFan * 60 / 7.5); //(Pulse frequency x 60) / 7.5Q, = flow rate 

}

void sensors(){
  // read the analog in value:
  
  // Begin caudalimetro


  // End caudalimetro
  
  //Begin T & H
  
    // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit
  float f = dht.readTemperature(true);
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index
  // Must send in temp in Fahrenheit!
  float hi = dht.computeHeatIndex(f, h);
  
  //End T & H

  
 
  
  //
  
  
  //begin sensor LDR
  sensorValue = analogRead(analogInPin); 

    if (sensorValue < 5)
      {
      Value = 0;
      }
    else if (sensorValue >= 5 && sensorValue < 36)
      {
      Valuecc = map(sensorValue, 5, 36, 1, 10);
      Value = Valuecc/10;
       }
    else if (sensorValue >= 36 && sensorValue < 215)
      {
      Value = map(sensorValue, 36, 215, 1, 10); 
      }
    else if (sensorValue >= 215 && sensorValue < 477)
      {
      Value = map(sensorValue, 215, 477, 10, 40); 
      }
    else if (sensorValue >= 477 && sensorValue < 673)
      {
      Value = map(sensorValue, 477, 673, 40, 100); 
      }
    else if (sensorValue >= 673 && sensorValue < 795)
      {
      Value = map(sensorValue, 673, 795, 100, 200); 
      }
    else if (sensorValue >= 795 && sensorValue < 955)
      {
      Value = map(sensorValue, 795, 955, 200, 1000); 
      }
    else
      {
      Value = map(sensorValue, 955, 1023, 100, 10000); 
      }
  //end sensor LDR
  
  //begin sensor NO2
  sensorValue1 = analogRead(analogInPin1); 
  Value1 = map(sensorValue1, 0, 1023, 0, 1000);
  //End sensor NO2  
  
  //begin sensor CO
  sensorValue2 = analogRead(analogInPin2);
  
    if (sensorValue2 < 950)
      {
      Value2 = map(sensorValue2, 0, 950, 0, 5);
      }
    else 
      {
      Value2 = map(sensorValue2, 950, 1023, 5, 10000); 
      } 
  //Value2 = map(sensorValue2, 0, 1023, 0, 1000);
  //End sensor CO  
  
  //begun sensor Sonido
  sensorValue4 = analogRead(analogInPin4);
  Value4b = sensorValue4/24; // tomando como Vr=120mV equivalente a 24bit
    if (Value4b < 2)
      {
      Value4a = map(Value4b, 0, 2, 1, 2);
      }
    else 
      {
      Value4a = map(Value4b, 3, 1023, 3, 1023);
      }   
  Value4 = 20*log10(Value4a);
  //End sensor Sonido 
  
  //begun sensor UV
  sensorValue5 = analogRead(analogInPin5);
  Value5 = map(sensorValue5, 0, 30, 0, 15); 

  //End sensor UV 
  

  
     
  // map it to the range of the analog out:
  /*outputValue  = map(sensorValue, 0, 1023, 0, 255);
    outputValue1 = map(sensorValue1, 0, 1023, 0, 255);  
    outputValue2 = map(sensorValue2, 0, 1023, 0, 255);  
    outputValue4 = map(sensorValue4, 0, 1023, 0, 255);  
    outputValue5 = map(sensorValue5, 0, 1023, 0, 255);  
  */


  
  //outputValue = map(Value, 0, 10000, 0, 255);  
  // change the analog out value:
//  analogWrite(analogOutPin, outputValue);           
  // print the results to the serial monitor:
  //B.LDR
  Serial.print("S.LDR = " );                       
  Serial.print(sensorValue);      
  Serial.print(" LDR = ");      
  Serial.print(Value);
  Serial.print(" lux " ); 
  //E.LDR
  
  //B.NO2
  Serial.print(" S.NO2 = " );                       
  Serial.print(sensorValue1);      
  Serial.print(" NO2 = ");      
  Serial.print(Value1);
  Serial.print(" ppb " ); 
  //E.NO2
  
  //B.CO
  Serial.print(" S.CO = " );                       
  Serial.print(sensorValue2);      
  Serial.print(" CO = ");      
  Serial.print(Value2);
  Serial.print(" ppm " ); 
  //E.CO
  
  //B.Sonido
  Serial.print(" S.Sonido = " );                       
  Serial.print(sensorValue4);      
  Serial.print(" Ruido = ");      
  Serial.print(Value4);
  Serial.print(" db " ); 
  //E.Sonido
  
  //B.UV
  Serial.print(" S.UV = " );                       
  Serial.print(sensorValue5);      
  Serial.print(" UV = ");      
  Serial.print(Value5);
  Serial.print(" UVI " ); 
  //E.UV
    
  //B.Caudalimetro

  //E.Caudalimetro

 //B. T&H
  Serial.print("Humidity: "); 
  Serial.print(h);
  Serial.print(" % ");
  Serial.print("Temperature: "); 
  Serial.print(t);
  Serial.print(" C ");
  //E. T&H
 
 
 //B. flowmeter 
                                 //in L/hour 
 Serial.print("Flow: "); //Prints the number calculated above
 Serial.print(Calc, DEC); //Prints the number calculated above
 Serial.println(" L/hour\r\n"); //Prints "L/hour" and returns a  new line

 //E: flowmeter 

  // wait 2 milliseconds before the next loop
  // for the analog-to-digital converter to settle
  // after the last reading:
  delay(2); 
 
}  
