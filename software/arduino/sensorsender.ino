//Libraries

#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "DHT.h"
#include <math.h>

//Constants

#define TdsSensorPin A1
#define SensorPin 0          //pH meter Analog output to Arduino Analog Input 0
#define VREF 5.0             // analog reference voltage(Volt) of the ADC
#define SCOUNT  30           // sum of sample point

#define DHTPIN 2             // what pin we're connected to
#define DHTTYPE DHT22        // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE);    // Initialize DHT 

//Variables

int counter = 0;

//TDS Meter
int analogBuffer[SCOUNT];    // store the analog value in the array, read from ADC
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0,copyIndex = 0;
float averageVoltage = 0,tdsValue = 0,temperature = 30;

//pH sensor
unsigned long int avgValue;  //Store the average value of the sensor feedback
float phValue=(((phValue=(float)avgValue*5.0/1024/6)*3.5));
float b;
int buf[10],temp;

//BME280
Adafruit_BME280 bme;

//dht22 
int chk;
float enc_humidity;  //Stores humidity value
float enc_temp; //Stores temperature value

//lightiness (open enclosure)
float lightsensor; //Resistance of grove light sensor (v1.2) in K

//ends 

void setup() {
  
  pinMode(13,OUTPUT);
  
  dht.begin();
  
  Serial.begin(9600);
  while (!Serial);

  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

  Serial.println("LoRa Sender");

  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  
  Serial.print("Sending packet: ");
  Serial.println(counter);
  //Serial.print("phing: ");
  //Serial.println(phValue,2);
  //Serial.print("phing: ");
    
  // send packet
  gettds();
  getph();
  getlight();
  LoRa.beginPacket();
  LoRa.print(bme.readTemperature());
  LoRa.print(",");
  LoRa.print(bme.readPressure() / 100.0F);
  LoRa.print(",");
  LoRa.print(bme.readHumidity());
  LoRa.print(",");
  LoRa.print(tdsValue);
  LoRa.print(",");
  LoRa.print(phValue);
  LoRa.print(",");
  LoRa.print(dht.readHumidity());
  LoRa.print(",");
  LoRa.print(dht.readTemperature());
  LoRa.print(",");
  LoRa.print(lightsensor,DEC);
  LoRa.print(",");
  LoRa.endPacket();

  counter++;

  delay(5000);
}


//Function things TDS, pH, Light


void gettds()
{
   static unsigned long analogSampleTimepoint = millis();
   if(millis()-analogSampleTimepoint > 40U)     //every 40 milliseconds,read the analog value from the ADC
   {
     analogSampleTimepoint = millis();
     analogBuffer[analogBufferIndex] = analogRead(TdsSensorPin);    //read the analog value and store into the buffer
     analogBufferIndex++;
     if(analogBufferIndex == SCOUNT) 
         analogBufferIndex = 0;
   }   
   static unsigned long printTimepoint = millis();
   if(millis()-printTimepoint > 800U)
   {
      printTimepoint = millis();
      for(copyIndex=0;copyIndex<SCOUNT;copyIndex++)
        analogBufferTemp[copyIndex]= analogBuffer[copyIndex];
      averageVoltage = getMedianNum(analogBufferTemp,SCOUNT) * (float)VREF / 1024.0; // read the analog value more stable by the median filtering algorithm, and convert to voltage value
      float compensationCoefficient=1.0+0.02*(temperature-25.0);    //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
      float compensationVolatge=averageVoltage/compensationCoefficient;  //temperature compensation
      tdsValue=(133.42*compensationVolatge*compensationVolatge*compensationVolatge - 255.86*compensationVolatge*compensationVolatge + 857.39*compensationVolatge)*0.5; //convert voltage value to tds value
      //Serial.print("voltage:");
      //Serial.print(averageVoltage,2);
      //Serial.print("V   ");
      Serial.print("TDS Value:");
      Serial.print(tdsValue,0);
      Serial.println("ppm");
   }
}
int getMedianNum(int bArray[], int iFilterLen) 
{
      int bTab[iFilterLen];
      for (byte i = 0; i<iFilterLen; i++)
      bTab[i] = bArray[i];
      int i, j, bTemp;
      for (j = 0; j < iFilterLen - 1; j++) 
      {
      for (i = 0; i < iFilterLen - j - 1; i++) 
          {
        if (bTab[i] > bTab[i + 1]) 
            {
        bTemp = bTab[i];
            bTab[i] = bTab[i + 1];
        bTab[i + 1] = bTemp;
         }
      }
      }
      if ((iFilterLen & 1) > 0)
    bTemp = bTab[(iFilterLen - 1) / 2];
      else
    bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
      return bTemp;
}

void getph()
{
  for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
  { 
    buf[i]=analogRead(SensorPin);
    delay(10);
  }
  for(int i=0;i<9;i++)        //sort the analog from small to large
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)                      //take the average value of 6 center sample
    avgValue+=buf[i];
  float phValue1=(float)avgValue*5.0/1024/6; //convert the analog into millivolt
  phValue=3.5*phValue1;                      //convert the millivolt into pH value
  Serial.print("pH:");  
  Serial.print(phValue);
  Serial.println(" ");
  digitalWrite(13, HIGH);       
  delay(800);
  digitalWrite(13, LOW);
 
}


void getlight()  {
  int sensorValue = analogRead(3); 
  lightsensor=(float)(1023-sensorValue)*10/sensorValue;
}
