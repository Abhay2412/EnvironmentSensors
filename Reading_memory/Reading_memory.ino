#include <EEPROM.h>
int i = 0;
int j = 1;
int k = 2;
int l = 3;
int n = 4;
int h = 5;
int m = 6;
int s = 7;
int x = 8;
void setup() {
Serial.begin(9600);
}
void loop() {
  
  //---------------------- Reading EEPROM address value ------------------
  int Value1 = EEPROM.read(i);
  int Value2 = EEPROM.read(j);
  int Value3 = EEPROM.read(k);
  int Value4 = EEPROM.read(l);
  int Value5 = EEPROM.read(n);
  int Value6 = EEPROM.read(h);
  int Value7 = EEPROM.read(m);
  int Value8 = EEPROM.read(s);
  int Value9 = EEPROM.read(x);

  Serial.print("Time:");
  Serial.println();
  Serial.print(Value6);
  Serial.print(":");
  Serial.print(Value7);
  Serial.print(":");
  Serial.print(Value8);
  Serial.println();
  Serial.print("Temperture value: ");
  Serial.print(Value1);
  Serial.print(" c");
  Serial.println();
  Serial.print("Humidity value: ");
  Serial.print(Value2);
  Serial.print("%");
  Serial.println();
  Serial.print("Light sensor value: ");
  Serial.print(Value3);
  Serial.print(" lumen");
  Serial.println();
  Serial.print("Water lelvel value: ");
  Serial.print(Value4);
  Serial.println();
  Serial.print("Sound level value: ");
  Serial.print(Value5);
  Serial.print(" DB");
  Serial.println();
  if (Value9 == 1){
    Serial.print("Motion Detected!");
    Serial.println();
  }
  else
    Serial.print("No Motion Detected");
    Serial.println();
  Serial.println();
  i=i+9;
  j=j+9;
  k=k+9;
  l=l+9;
  n=n+9;
  h=h+9;
  m=m+9;
  s=s+9;
  x=x+9;
  delay(1000);
}
