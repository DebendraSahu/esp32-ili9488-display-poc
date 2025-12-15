#include <SPI.h>
#include <Arduino.h>
#define TOUCH_CS 22

SPISettings touchSPI(2000000, MSBFIRST, SPI_MODE0);

uint16_t readXPT(uint8_t cmd)
{
  SPI.beginTransaction(touchSPI);
  digitalWrite(TOUCH_CS, LOW);
  SPI.transfer(cmd);
  uint16_t v = SPI.transfer16(0x0000);
  digitalWrite(TOUCH_CS, HIGH);
  SPI.endTransaction();
  return v >> 3;
}

void setup()
{
  Serial.begin(115200);
  delay(1000);

  Serial.println("\nXPT2046 RAW TEST");

  pinMode(TOUCH_CS, OUTPUT);
  digitalWrite(TOUCH_CS, HIGH);

  SPI.begin(18, 19, 23);
}

void loop()
{
  uint16_t x = readXPT(0xD0); // X
  uint16_t y = readXPT(0x90); // Y
  uint16_t z = readXPT(0xB0); // Pressure

  Serial.printf("X:%4d  Y:%4d  Z:%4d\n", x, y, z);
  delay(300);
}
