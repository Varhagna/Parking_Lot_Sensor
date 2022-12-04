#include <RH_ASK.h>
#include <SPI.h>
#define NUM_SENSORS 3

RH_ASK driver(2000, 8, 7, 6, false); // initialize RH_ASK driver in order to use wire
uint8_t distances[2];

void setup() {

  Serial.begin(115200);   
  digitalWrite(SS, HIGH);
  if (!driver.init())
		Serial.println("init failed");

  // initialize SPI by dividing clock frequency by 8
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV8);
}

void loop()
{
    uint8_t buf[2];
    uint8_t buflen = sizeof(buf);
    if (driver.recv(buf, &buflen)) // Non-blocking
    {
    	int i;
      // Message with a good checksum received, dump it.
      distances[0] = buf[1];
      distances[1] = buf[0];
      
      Serial.print("ID: ");
      Serial.println(buf[1]);
      Serial.print("Dist: ");
      Serial.println(buf[0]);

      // transfer data using SPI
      digitalWrite(SS, LOW);
      SPI.transfer(distances[0]);
      SPI.transfer(distances[1]);
      digitalWrite(SS, HIGH);
      // add delay to ensure proper response.
      delay(1000);
    }

}
