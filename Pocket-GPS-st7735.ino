#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include <TinyGPSPlus.h>

#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMono12pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSansOblique9pt7b.h>
#include "gfx.h"


  #define TFT_CS     9
  #define TFT_RST    33
  #define TFT_DC     13
  #define TFT_SCLK   12 
  #define TFT_MOSI   11


// For ST7735-based displays, we will use this call
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
const uint16_t GRAY = 0x3131;
const uint16_t BLUE = 0x001f;
const uint16_t RED = 0xf800;
const uint16_t GREEN = 0x07e0;
const uint16_t CYAN = GREEN + BLUE;
const uint16_t MAGENTA = RED + BLUE;

#define RXD2 18
#define TXD2 17
HardwareSerial neogps(1);

TinyGPSPlus gps;

float p = 3.1415926;

void setup(void) {
  Serial.begin(115200);
  Serial.print(F("Hello! ST77xx TFT Test"));
  neogps.begin(9600, SERIAL_8N1, RXD2, TXD2);
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(0);

  tft.fillScreen(ST7735_BLACK);
}

void loop() {

  boolean newData = false;
  for (unsigned long start = millis(); millis() - start < 1000;) {
    while (neogps.available()) {
      if (gps.encode(neogps.read())) {
        newData = true;
      }
    }
  }

  if (gps.location.isValid() == 1) {

    tft.drawBitmap(10, 90, satellite, 50, 50, ST7735_WHITE);
    tft.fillRect(80, 80, 50, 50, ST7735_BLACK);

    tft.setTextColor(ST7735_WHITE);
    tft.setFont();
    tft.setTextSize(1);
    tft.setCursor(5, 60);
    tft.print("Detected Satellites:");
    tft.setFont(&FreeSansOblique9pt7b);
    tft.setTextSize(2);
    tft.setCursor(80, 120);
    tft.print(gps.satellites.value());

    delay(5000);
    tft.fillScreen(ST7735_BLACK);

    tft.drawBitmap(6, 5, cloud, 30, 30, ST7735_YELLOW);
    tft.fillRect(70, 5, 40, 70, ST7735_BLACK);

    tft.setTextColor(0x07FF);
    tft.setFont();
    tft.setTextSize(1);
    tft.setCursor(40, 10);
    tft.print("Lat: ");
    tft.print(gps.location.lat(), 4);
    tft.setCursor(40, 25);
    tft.print("Lng: ");
    tft.print(gps.location.lng(), 4);

    //String gps_speed = String(gps.speed.kmph());

    tft.drawBitmap(6, 50, speed, 50, 50, ST7735_YELLOW);
    tft.fillRect(60, 75, 50, 40, ST7735_BLACK);

    tft.setTextColor(0x07FF);
    tft.setFont();
    tft.setTextSize(0);
    tft.setCursor(60, 65);
    tft.print("Speed(km/h)");
    tft.setFont(&FreeSansOblique9pt7b);
    tft.setTextSize(1);
    tft.setCursor(60, 90);
    tft.print(gps.speed.kmph());


    tft.drawBitmap(6, 100, alt, 50, 50, ST7735_YELLOW);
    tft.fillRect(60, 120, 50, 35, ST7735_BLACK);

    tft.setTextColor(0x07FF);
    tft.setFont();
    tft.setTextSize(0);
    tft.setCursor(60, 115);
    tft.print("Altitude");
    tft.setFont(&FreeSansOblique9pt7b);
    tft.setTextSize(1);
    tft.setCursor(60, 140);
    tft.print(gps.altitude.meters(), 0);


  } else {
    // tft.drawBitmap(6, 5, boot, 100, 100, ST7735_WHITE);
    tft.fillRect(60, 135, 10, 10, ST7735_BLACK);

    tft.setTextColor(ST7735_WHITE);
    tft.setFont();
    tft.setTextSize(1);
    tft.setCursor(30, 110);
    tft.print("Satellites: ");
    tft.print(gps.satellites.value());


    delay(5000);
    tft.fillScreen(ST7735_BLACK);
  }



  if (newData == true) {
    Serial.println("No Data");
    newData = false;
    Serial.println(gps.satellites.value());
  }
  if (newData == false) {
    Serial.println("No Data");
  }
}