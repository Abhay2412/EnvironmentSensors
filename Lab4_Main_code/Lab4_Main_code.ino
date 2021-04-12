

/*
   Various Sensors Interfacing V1

   This program will provide reference codes for the following sensors:
      - Temperature
      - Humidity
      - Light level
      - Sound level
      - Motion sensor
      - IR Receiver
      -Water Level
   In addition, the data will be timestamped using the Real Time Clock (RTC) module

   Requires:
      - ELEGOO Car Kit
      - MEGA 2560 board
      - Breadbord
      - Wires
      - DHT11 Temperature and Humidy Module
          --> Installation of EduIntro by Arduino LLC
      - Photoresistor
      - Sound Sensor Module
      - HC-SR501 PIR Motion Sensor Module
      - IR Receiver Module (part of ELEGGO Car Kit)
          --> Installation of IRremote library by shirriff, z3t00
      - DS1307 RTC Module
          --> Installation of RTClib by Adafruit

   Pinout (you may also refer to picture for connections):
      - Signal of DHT11 --> Pin 30
      - Photoresitor --> Pin A15
      - A0 of Sound Sensor Module --> Pin A14
      - D0 of Sound Sensor Module --> Pin 48
      - Signal of Motion Sensor Module --> Pin 40
      - Y of IR Receiver module --> Pin 12 (Part of ELEGOO Kit)
      - SDA of RTC Module --> Pin 20
      - SCL of RTC Module --> Pin 21

*/

#include <EEPROM.h>
#include <avr/pgmspace.h>

#define LED 41  //LED test pin

//---------------------------- DHT 11 Temperature and Humidty Module ----------------------------
#include <EduIntro.h>               // Include the EduIntro library
DHT11 dht11(30);                    // Creating DHT11 object sensor on pin 30
//-----------------------------------------------------------------------------------------------


//---------------------------------------- Photoresistor ----------------------------------------
#define PHOTORESISTOR_PIN A15       // Define digital pin location for phototresistor
//-----------------------------------------------------------------------------------------------

//---------------------------------------- Water Level ------------------------------------------
#define WATERLEVEL_PIN A10       // Define digital pin location for Water Level Sensor
//-----------------------------------------------------------------------------------------------



//------------------------------------- Sound Sensor Module -------------------------------------
#define SOUND_DIGITAL_PIN 48        // Define digital pin location for sound sensor module
#define SOUND_ANALOG_PIN A14        // Define analog pin location for sound sensor module
//-----------------------------------------------------------------------------------------------


//------------------------------ HC-SR501 PIR Motion Sensor Module -------------------------------
#define PIR_PIN 40                  // Define pin location for Motion sensor module
int trackMotion;                    // Keeps track of motion detecting 
//-----------------------------------------------------------------------------------------------


//------------------------------------- IR Receiver Module --------------------------------------
// IMPORTANT: Cannot use IRremote library with DHT (or Servo) due to timer conflict
/*
#include <IRremote.h>
#define IR_REC_PIN 12               // Define pin location for IR receiver module
IRrecv irrecv(IR_REC_PIN);          // Declare object instance of IR receiver
decode_results results;             // Special type of variable for IR Receiver data
*/
//-----------------------------------------------------------------------------------------------


//------------------------------------- DS1307 RTC Module ---------------------------------------
#include <Wire.h>                     // Include the Wire library for I2C communication
#include "RTClib.h"                   // Include the RTC library
RTC_DS1307 rtc;                       // Declare an RTC object instance
int startSecond;
int startMinute;
int startHour;
//-----------------------------------------------------------------------------------------------


int loopCounter = 0;                // Keeps track of current void loop() iteration

int TempCEEPROM = 0;                // EEPROM address for temperture in celecius 
int HumidEEPROM = 1;                // EEPROM address for humidity 
int PhotoEEPROM = 2;                // EEPROM address for photo sensor
int WaterEEPROM = 3;                // EEPROM address for water level
int SoundEEPROM = 4;                // EEPROM address for sound level in DB
int HourEEPROM = 5;                 // EEPROM address for RTC hour 
int MinuteEEPROM = 6;               // EEPROM address for RTC minute 
int SecondEEPROM = 7;               // EEPROM address for RTC second
int MotionEEPROM = 8;               // EEPROM address for motion 

void setup() {

  Serial.begin(9600);               // Initialize Serial communications

  pinMode(LED, OUTPUT);
  
  //---------------------------------------- Photoresistor ----------------------------------------
  pinMode(PHOTORESISTOR_PIN, INPUT); // Declare photoresistor as input to Arduino
  //-----------------------------------------------------------------------------------------------

  //---------------------------------------- Water Level ------------------------------------------
  pinMode(WATERLEVEL_PIN, INPUT); // Declare Water Level Sensor as input to Arduino
  //-----------------------------------------------------------------------------------------------

  //------------------------------------- Sound Sensor Module -------------------------------------
  pinMode(SOUND_DIGITAL_PIN, INPUT);// Declare sound sensor digital as input to Arduino
  pinMode(SOUND_ANALOG_PIN, INPUT); // Declare sound sensor analog as input to Arduino
  //-----------------------------------------------------------------------------------------------


  //------------------------------ HC-SR501 PIR Motion Sensor Module -------------------------------
  pinMode(PIR_PIN, INPUT);          // Declare motion sensor as input to Arduino
  //-----------------------------------------------------------------------------------------------


  //------------------------------------- IR Receiver Module --------------------------------------
  //  irrecv.enableIRIn();              // Start the IR receiver module
  //  irrecv.blink13(true);             // On board LED blinks every time the receiver gets a signal
  //-----------------------------------------------------------------------------------------------


  //------------------------------------- DS1307 RTC Module ---------------------------------------
  rtc.begin();                          // Initialize the RTC module
  // Following code should only be ever run once
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));   //sets the RTC to the date & time this sketch was compiled
  //rtc.adjust(DateTime(2021, 03, 17, 9, 33, 0)); //March 17 at 9:33am
  // You may also explicitly set date and time using rtc.adjust(DateTime(2020, 12, 23, 3, 0, 0));
  // Example above evaluates to December 23, 2020 at 3am

  DateTime now = rtc.now();         // Update RTC every loop and store in DateTime special variable
  
  startSecond = now.second();
  Serial.println(startSecond);
  
  startMinute = now.minute();
  Serial.println(startMinute);
  
  startHour = now.hour();
  Serial.println(startHour);
  //-----------------------------------------------------------------------------------------------

  delay(3000);                      // Wait 3 seconds to complete setup (not required)
}

void loop() {
  if (loopCounter >= 500) {
    while (1);                      // Only allows 500 iterations of void loop()
  }
  loopCounter++;                    // Increment loopCounter by 1           
  

  //------------------------------------- DS1307 RTC Module ---------------------------------------
  /*
   * Many functions available (however only some are used). RTClib also includes:
   *    - TimeSpan(); --> Determine the exact time during the future
   *    - now.unixTime(); --> Number of seconds that have elapsed since the Unix epoch (Midnight 1/1/1970)
   *    - etc.
   *    
   * NOTE: Cannot implement MPU 6050 and RTS at the same time due to use of SDA/SCL
   */
  DateTime now = rtc.now();         // Update RTC every loop and store in DateTime special variable
  Serial.print(now.hour(), DEC); Serial.print(':');
  Serial.print(now.minute(), DEC); Serial.print(':');
  Serial.print(now.second(), DEC); Serial.println();

  
  int currentSec = now.second();
  EEPROM.write(SecondEEPROM,currentSec);

  int currentMin = now.minute();
  EEPROM.write(MinuteEEPROM,currentMin);
  
  int currentHour = now.hour();
  EEPROM.write(HourEEPROM,currentHour);
  Serial.print(currentHour);
  
  //-----------------------------------------------------------------------------------------------


  //---------------------------- DHT 11 Temperature and Humidty Module ----------------------------
  /*
      Humidity Range: 20-90% ±5% Relative Humidity (RH)
      Temperature Range: 0-50 °C ±2% °C
  */
  dht11.update();                   // Updates current data in DHT11 (updates at every loop)
  float C = dht11.readCelsius();      // Reads the temperature in Celsius
  float F = dht11.readFahrenheit(); // Reads the temperature in Fahrenheit
  int H = dht11.readHumidity();     // Reads the humidity index

  Serial.print("H: "); Serial.print(H);
  Serial.print("\tC: "); Serial.print(C);
  Serial.print("\tF: "); Serial.println(F);
  EEPROM.write(TempCEEPROM,C);
  EEPROM.write(HumidEEPROM,H);
  
  //-----------------------------------------------------------------------------------------------


  //---------------------------------------- Photoresistor ----------------------------------------
  /*
      Resistance at light: 18-50 kΩ
      Resitance at dark: 2.0 MΩ

      Connection:
         - Photoresistor connected in series with 10kΩ resistor
         - Signal pin (A15) connected to resistor and photoresitor node
  */

  float lightSensorVal = analogRead(PHOTORESISTOR_PIN);
  int lux=sensorRawToPhys(lightSensorVal);
  Serial.print("Raw value from sensor= ");
  Serial.println(lightSensorVal); // the analog reading
  Serial.print("Physical value from sensor = ");
  Serial.print(lux); // the analog reading
  Serial.println(" lumen"); // the analog reading
  EEPROM.write(PhotoEEPROM,lux);
  
  //-----------------------------------------------------------------------------------------------

  //---------------------------------------- Water Level ------------------------------------------
  int liquid_level= analogRead(WATERLEVEL_PIN);//arduino reads the value from the liquid level sensor
  Serial.print("Water level: ");
  Serial.println(liquid_level);//prints out liquid level sensor reading
  delay(100);//delays 100ms
  EEPROM.write(WaterEEPROM,liquid_level);
  //-----------------------------------------------------------------------------------------------


  //------------------------------------- Sound Sensor Module -------------------------------------
  /*
      Sensitivity: 48 to 66 decibels
      May either have an analog or digital output (different pin for each)

      NOTE: Sounds sensor has a potentiometer, which can be used to set sensitivy
  */
  float sound = analogRead(SOUND_ANALOG_PIN);
  Serial.print("Sound level: ");
  Serial.println(sound);
  EEPROM.write(SoundEEPROM,sound);
  if (digitalRead(SOUND_DIGITAL_PIN) == HIGH) {
    Serial.println("It is too loud");
    digitalWrite(LED,HIGH);
  }

  else{
    digitalWrite(LED, LOW);
  }
  //-----------------------------------------------------------------------------------------------


  //------------------------------ HC-SR501 PIR Motion Sensor Module -------------------------------
  /*
      Detects at less than 100 degrees cone angle
      Output from 0V (LOW) to 3.3V (HIGH) so can be used as a standalone unit
      Need to wait about 30-60 secnods for te sensor to initialize
      Several adjustments possible:
        - Jumper:
            -- H: Repeat trigger (every time motion is detected, the delay timer is restarted)
            -- L: Single trigger (turn HIGH as soon as motion is detected and stays for time set below
        - Potentiometer on the inside:
            -- Sensitivity adjustment (CW to increase range from 3 to 7 meters)
        - Potentiometer on the outside
            -- Time-delay adjustment (CW to increase time-delay from 3 to 300 seconds)
  */
  if (digitalRead(PIR_PIN) == HIGH) {
    Serial.println("Motion Detected!");
    trackMotion = 1;
    EEPROM.write(MotionEEPROM,trackMotion);
  }
    else 
    trackMotion = 0;
    EEPROM.write(MotionEEPROM,trackMotion);

  TempCEEPROM = TempCEEPROM+9;                  
  HumidEEPROM = HumidEEPROM+9;                  
  PhotoEEPROM = PhotoEEPROM+9;                   
  WaterEEPROM = WaterEEPROM+9;                  
  SoundEEPROM = SoundEEPROM+9;  
  HourEEPROM = HourEEPROM+9;              
  MinuteEEPROM = MinuteEEPROM+9;     
  SecondEEPROM = SecondEEPROM+9;      
  MotionEEPROM = MotionEEPROM+9;       
 
  
  
  //-----------------------------------------------------------------------------------------------
  delay(30000);
  Serial.println();
}


  int sensorRawToPhys(int raw){
  // Conversion rule
  float Vout = float(raw) * (5 / float(1023));// Conversion analog to voltage
  float RLDR = (10000 * (5 - Vout))/Vout; // Conversion voltage to resistance
  int phys=500/(RLDR/1000); // Conversion resitance to lumen
  return phys;
}
