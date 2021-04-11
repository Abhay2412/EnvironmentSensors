#include <EEPROM.h>

int i =0;

void setup() {
  
Serial.begin(9600);
Serial.print("EEPROM has been cleared successfuly...");

}
void loop() {
  
  // -------------- Clearing EEPROM memory and replace it with zeroes-------------
  EEPROM.write(i,0);
  i++;
}
