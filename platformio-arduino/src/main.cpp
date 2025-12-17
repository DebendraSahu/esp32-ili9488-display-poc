#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>

// ================= TOUCH PINS =================
#define T_CS 22
#define T_IRQ 21
#define T_CLK 18
#define T_MISO 19
#define T_MOSI 23

// ================= RAW TOUCH LIMITS =================
// Adjust ONCE if your panel differs
#define TOUCH_MIN_X 200
#define TOUCH_MAX_X 3800
#define TOUCH_MIN_Y 200
#define TOUCH_MAX_Y 3800

// ================= UI CONFIG =================
#define TOOLBAR_HEIGHT 50
#define BOX_SIZE 40
#define BOX_PADDING 8
#define BRUSH_RADIUS 3

SPIClass spi_bus(VSPI);
TFT_eSPI tft = TFT_eSPI();
XPT2046_Touchscreen ts(T_CS, T_IRQ);

// ================= COLOR PALETTE =================
uint16_t colors[] = {
    TFT_WHITE,
    TFT_BLACK,
    TFT_RED,
    TFT_GREEN,
    TFT_BLUE,
    TFT_YELLOW,
    TFT_CYAN,
    TFT_MAGENTA};

// Borders chosen for visibility (not aesthetics)
uint16_t borderColors[] = {
    TFT_RED,   // white
    TFT_BLUE,  // black
    TFT_WHITE, // red
    TFT_WHITE, // green
    TFT_WHITE, // blue
    TFT_BLACK, // yellow
    TFT_BLACK, // cyan
    TFT_BLACK  // magenta
};

const int COLOR_COUNT = sizeof(colors) / sizeof(colors[0]);

uint16_t drawColor = TFT_WHITE;
int selectedIndex = 0;

// ================= UI =================
void drawToolbar()
{
  tft.fillRect(0, 0, tft.width(), TOOLBAR_HEIGHT, TFT_DARKGREY);

  for (int i = 0; i < COLOR_COUNT; i++)
  {
    int x = BOX_PADDING + i * (BOX_SIZE + BOX_PADDING);
    int y = 5;

    // Highlight selected color
    if (i == selectedIndex)
      tft.drawRect(x - 2, y - 2, BOX_SIZE + 4, BOX_SIZE + 4, TFT_WHITE);

    tft.drawRect(x, y, BOX_SIZE, BOX_SIZE, borderColors[i]);
    tft.fillRect(x + 2, y + 2, BOX_SIZE - 4, BOX_SIZE - 4, colors[i]);
  }
}

// ================= SETUP =================
void setup()
{
  Serial.begin(115200);
  delay(300);

  spi_bus.begin(T_CLK, T_MISO, T_MOSI);

  tft.init();
  tft.setRotation(0); // portrait
  tft.fillScreen(TFT_BLACK);

  pinMode(T_IRQ, INPUT_PULLUP);
  if (!ts.begin())
  {
    Serial.println("Touch init FAILED");
    while (1)
      ;
  }

  ts.setRotation(2); // must match physical orientation

  drawToolbar();
}

// ================= LOOP =================
void loop()
{
  if (!ts.touched())
    return;

  TS_Point p = ts.getPoint();

  int x = map(p.x, TOUCH_MIN_X, TOUCH_MAX_X, 0, tft.width());
  int y = map(p.y, TOUCH_MIN_Y, TOUCH_MAX_Y, 0, tft.height());

  x = constrain(x, 0, tft.width() - 1);
  y = constrain(y, 0, tft.height() - 1);

  // ===== TOOLBAR TOUCH =====
  if (y < TOOLBAR_HEIGHT)
  {
    for (int i = 0; i < COLOR_COUNT; i++)
    {
      int boxX = BOX_PADDING + i * (BOX_SIZE + BOX_PADDING);

      if (x > boxX && x < boxX + BOX_SIZE)
      {
        selectedIndex = i;
        drawColor = colors[i];
        drawToolbar(); // redraw highlight
        delay(180);    // debounce (POC-level)
        return;
      }
    }
    return;
  }

  // ===== CANVAS DRAWING =====
  tft.fillCircle(x, y, BRUSH_RADIUS, drawColor);

  delay(5);
}
