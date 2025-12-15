#include <Arduino.h>
#include <SPI.h>
#include <XPT2046_Touchscreen.h>

#define T_CS   22
#define T_IRQ  21   // comment out if not connected

#define T_CLK  18
#define T_MISO 19
#define T_MOSI 23

XPT2046_Touchscreen ts(T_CS, T_IRQ);

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n=== TOUCH LEVEL-2 DEBUG ===");

  SPI.begin(T_CLK, T_MISO, T_MOSI);

  pinMode(T_IRQ, INPUT_PULLUP);

  if (!ts.begin()) {
    Serial.println("Touch init FAILED");
    while (1);
  }

  ts.setRotation(0);

  Serial.println("Touch init OK");
  Serial.println("Press screen...");
}

void loop() {
  bool irq_state = digitalRead(T_IRQ);

  if (ts.touched()) {
    TS_Point p = ts.getPoint();

    Serial.print("IRQ=");
    Serial.print(irq_state);
    Serial.print("  X=");
    Serial.print(p.x);
    Serial.print("  Y=");
    Serial.print(p.y);
    Serial.print("  Z=");
    Serial.println(p.z);
  } else {
    Serial.print("no touch | IRQ=");
    Serial.println(irq_state);
  }

  delay(200);
}
