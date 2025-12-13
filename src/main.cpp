#include <TFT_eSPI.h>

TFT_eSPI tft;

void setup() {
  tft.init();
  tft.setRotation(1);   // landscape
  tft.fillScreen(TFT_BLACK);

  // -------- Smiley --------
  int cx = 240;
  int cy = 120;
  int r  = 60;

  // Face
  tft.fillCircle(cx, cy, r, TFT_YELLOW);

  // Eyes
  tft.fillCircle(cx - 25, cy - 20, 6, TFT_BLACK);
  tft.fillCircle(cx + 25, cy - 20, 6, TFT_BLACK);

  // Smile (upward curve)
  for (int i = -30; i <= 30; i += 2) {
    int y = cy + 20 - (i * i) / 60;
    tft.fillCircle(cx + i, y, 2, TFT_BLACK);
  }

  // -------- Text area --------
  // Black background strip (explicit)
  tft.fillRect(0, 220, 480, 40, TFT_BLACK);

  // Text (GLCD font only)
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);

  tft.setCursor(200, 230);
  tft.print("Hello Moti");

  // Border for reference
  tft.drawRect(20, 20, 440, 280, TFT_GREEN);
}

void loop() {}
