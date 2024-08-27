//DHT from brainy bits
//BMP from circuitbasics
#include <dht.h>
#include <SFE_BMP180.h>
#include <Wire.h>
#define dht_apin A3 // Analog Pin sensor is connected to
#define ALTITUDE 76
 
dht DHT;
SFE_BMP180 bmp180;
int myArray[5];
int i=0;
const int motorPin = 7;      //Turn motor on and off
//const int moisturePin = A3; 
void setup(){
 
  Serial.begin(9600);
  digitalWrite(motorPin,LOW);
  delay(500);
  bool success = bmp180.begin();
  if (success) {
    Serial.println("SENSORS INITIATED");
  }
  else{
    Serial.println("SENSOR INITIAION FAILED :(");
    while(1); //Pause forever.
  }
   //pinMode(mosfetPin, OUTPUT);
}
 
void loop(){
  //Start of Program 
    char status;
    double T, P; //BMP180 VARIABLES
    bool success = false;
    int z;

    double dhumidity; //DHT VARIABLES
    double dtemperature; //DHT VARIABLES
    
    int ps; //pressure status

    //MOSFET
    Serial.print("Moisture value: ");
    int sensorValue = analogRead(A0);
  // print out the value you read:
    Serial.print(sensorValue);
    Serial.println();
    //digitalWrite(motorPin, HIGH); // Turn MOSFET ON
    //delay(2000); // Wait for 1 second
    //digitalWrite(motorPin, LOW); // Turn MOSFET OFF

    //DHT11
    DHT.read11(dht_apin);
    Serial.print("Humidity = ");
    dhumidity = DHT.humidity;
    Serial.print(dhumidity);
    Serial.print("%  ");
    //Serial.print("temperature = ");
    //dtemperature = DHT.temperature;
    //Serial.print(dtemperature); 
    //Serial.println("C  ");
   
    //BMP180
    status = bmp180.startTemperature();
    if (status != 0) {
      delay(1000);
      status = bmp180.getTemperature(T);

      if (status != 0) {
        status = bmp180.startPressure(3);

        if (status != 0) {
          delay(status);
          status = bmp180.getPressure(P, T);

          if (status != 0) {
            Serial.print("Pressure: ");
            Serial.print(P);
            Serial.println(" hPa");

            Serial.print("Temperature: ");
            Serial.print(T);
            Serial.println(" C");
          }
        }
      }
    }
    if(i<5){
      myArray[i] = P;
      i+=1;
    }
    else{
      i=0;
    }
    if(i>=1){
      ps = myArray[i] - myArray[i-1];
      if(ps>1){ //pressure rising
        z = 185 - (0.16 * P);
      }
      else if(ps<1){ //pressure falling
        z = 127 - (0.12 * P);
      }
      else{ //pressure steady
        z = 144 - (0.13*P);
      }
      switch(z) {
        case 1:
            Serial.println("Weather Update: Settled Fine");
            break;
        case 2:
            Serial.println("Weather Update: Fine Weather");
            break;
        case 3:
            Serial.println("Weather Update: Fine, Becoming Less Settled");
            break;
        case 4:
            Serial.println("Weather Update: Fairly Fine, Showery Later");
            break;
        case 5:
            Serial.println("Weather Update: Showery, Becoming More Unsettled");
            break;
        case 6:
            Serial.println("Weather Update: Unsettled, Rain Later");
            break;
        case 7:
            Serial.println("Weather Update: Rain at Times, Worse Later");
            break;
        case 8:
            Serial.println("Weather Update: Rain at Times, Becoming Very Unsettled");
            break;
        case 9:
            Serial.println("Weather Update: Very Unsettled, Rain");
            break;
        case 10:
            Serial.println("Weather Update: Settled Fine");
            break;
        case 11:
            Serial.println("Weather Update: Fine Weather");
            break;
        case 12:
            Serial.println("Weather Update: Fine, Possibly Showers");
            break;
        case 13:
            Serial.println("Weather Update: Fairly Fine, Showers Likely");
            break;
        case 14:
            Serial.println("Weather Update: Showery, Bright Intervals");
            break;
        case 15:
            Serial.println("Weather Update: Changeable, Some Rain");
            break;
        case 16:
            Serial.println("Weather Update: Unsettled, Rain at Times");
            break;
        case 17:
            Serial.println("Weather Update: Rain at Frequent Intervals");
            break;
        case 18:
            Serial.println("Weather Update: Very Unsettled, Rain");
            break;
        case 19:
            Serial.println("Weather Update: Stormy, Much Rain");
            break;
        case 20:
            Serial.println("Weather Update: Settled Fine");
            break;
        case 21:
            Serial.println("Weather Update: Fine Weather");
            break;
        case 22:
            Serial.println("Weather Update: Becoming Fine");
            break;
        case 23:
            Serial.println("Weather Update: Fairly Fine, Improving");
            break;
        case 24:
            Serial.println("Weather Update: Fairly Fine, Possibly Showers Early");
            break;
        case 25:
            Serial.println("Weather Update: Showery Early, Improving");
            break;
        case 26:
            Serial.println("Weather Update: Changeable, Mending");
            break;
        case 27:
            Serial.println("Weather Update: Rather Unsettled, Clearing Later");
            break;
        case 28:
            Serial.println("Weather Update: Unsettled, Probably Improving");
            break;
        case 29:
            Serial.println("Weather Update: Unsettled, Short Fine Intervals");
            break;
        case 30:
            Serial.println("Weather Update: Very Unsettled, Finer at Times");
            break;
        case 31:
            Serial.println("Weather Update: Stormy, Possibly Improving");
            break;
        case 32:
            Serial.println("Weather Update: Stormy, Much Rain");
            break;
        default:
            Serial.println("Weather Update: Invalid value for z");
      }
  }
  else{
    Serial.println("Waiting for pressure trends");
  } 
  
    delay(2500);
  if(sensorValue<800){ //SATURATED SOIL!
    digitalWrite(motorPin,LOW);
    Serial.println("Saturated Soil, not watering");
  }
  
  else if((z<=4) || (z>=10 && z<=12) || (z>=20 && z<=23)) //fine weather KEEP WATERING)
    {
      digitalWrite(motorPin,HIGH);
      Serial.println("Fine weather, watering soil");
    }
    else if(((z>=5 && z<=7)||(z>=13 && z<=16)||(z>=24 && z<=27))&&sensorValue>950) //unsettled OR soil not yet saturated (KEEP WATERING)
  {
    digitalWrite(motorPin,HIGH);
    Serial.println("Unsettled weather, watering soil a bit");
  }
  else if(((z>=5 && z<=7)||(z>=13 && z<=16)||(z>=24 && z<=27))&&sensorValue<950) //unsettled OR soil a bit wet (STOP WATERING)
  {
    digitalWrite(motorPin,LOW);
    Serial.println("Soil moist enough, not watering, waiting for rain");
  }
  else{ //rainy or saturated soil (STOP WATERING)
    digitalWrite(motorPin,LOW);
    Serial.println("It is raining, not watering.");
  } 
    Serial.println(" "); 
    Serial.println(" ");
  delay(3000);//Wait 5 seconds before accessing sensor again.
}// end loop() 