#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>

// ===== TOUCH PINS =====
#define T_CS 22
#define T_IRQ 21
#define T_CLK 18
#define T_MISO 19
#define T_MOSI 23

SPIClass spi_bus(VSPI);
TFT_eSPI tft = TFT_eSPI();
XPT2046_Touchscreen ts(T_CS, T_IRQ);

// Calibration matrix
float touchMatrix[6]; // scaleX, scaleY, offsetX, offsetY, rotation etc

bool calibrated = false;

// Screen corners used for calibration
TS_Point calibPoints[4] = {
    {200, 200, 0},   // top-left raw
    {3800, 200, 0},  // top-right raw
    {3800, 3800, 0}, // bottom-right raw
    {200, 3800, 0}   // bottom-left raw
};

// Corresponding screen positions
TS_Point screenPoints[4];

// Wait for a single touch release
void waitForTouch()
{
  while (!ts.touched())
    delay(10);
  while (ts.touched())
    delay(10);
}

void setup()
{
  Serial.begin(115200);
  delay(500);

  // SPI init
  spi_bus.begin(T_CLK, T_MISO, T_MOSI);

  // TFT init
  tft.init();
  tft.setRotation(0); // portrait
  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);

  // Touch init
  pinMode(T_IRQ, INPUT_PULLUP);
  if (!ts.begin())
  {
    Serial.println("Touch init FAILED");
    while (1)
      ;
  }
  ts.setRotation(2);

  // Display calibration instructions
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(10, 10);
  tft.println("Touch calibration:");
  tft.println("Touch the RED circles");

  // Draw calibration points
  screenPoints[0] = {20, 20, 0};                              // top-left
  screenPoints[1] = {tft.width() - 20, 20, 0};                // top-right
  screenPoints[2] = {tft.width() - 20, tft.height() - 20, 0}; // bottom-right
  screenPoints[3] = {20, tft.height() - 20, 0};               // bottom-left

  for (int i = 0; i < 4; i++)
  {
    tft.fillCircle(screenPoints[i].x, screenPoints[i].y, 5, TFT_RED);
    waitForTouch();
    TS_Point p = ts.getPoint();
    calibPoints[i].x = p.x;
    calibPoints[i].y = p.y;
    tft.fillCircle(screenPoints[i].x, screenPoints[i].y, 5, TFT_GREEN);
  }

  // Compute simple linear scaling factors
  touchMatrix[0] = (float)(tft.width() - 40) / (calibPoints[1].x - calibPoints[0].x);  // scaleX
  touchMatrix[1] = (float)(tft.height() - 40) / (calibPoints[3].y - calibPoints[0].y); // scaleY
  touchMatrix[2] = 20 - calibPoints[0].x * touchMatrix[0];                             // offsetX
  touchMatrix[3] = 20 - calibPoints[0].y * touchMatrix[1];                             // offsetY

  calibrated = true;

  tft.fillScreen(TFT_BLACK);
  tft.setCursor(10, 10);
  tft.println("Touch calibrated!");
  delay(1000);
  tft.fillScreen(TFT_BLACK);
}

void loop()
{
  if (!calibrated)
    return;

  if (ts.touched())
  {
    TS_Point p = ts.getPoint();

    // Apply calibration
    int x = p.x * touchMatrix[0] + touchMatrix[2];
    int y = p.y * touchMatrix[1] + touchMatrix[3];

    x = constrain(x, 0, tft.width() - 1);
    y = constrain(y, 0, tft.height() - 1);

    // Display coordinates
    tft.fillRect(0, 0, tft.width(), 40, TFT_BLACK);
    tft.setCursor(10, 10);
    tft.printf("X=%d Y=%d Z=%d", x, y, p.z);

    // Draw circle at touch point
    tft.fillCircle(x, y, 4, TFT_RED);

    Serial.printf("X=%d Y=%d Z=%d\n", x, y, p.z);
  }

  delay(5);
}
