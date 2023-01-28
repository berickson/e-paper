// Heltec EPS32 WiFi Kit V3 
//
// Pin map: https://resource.heltec.cn/download/WiFi_Kit_32_V3/HTIT-WB32_V3.png

#include <Arduino.h>

#include <GxEPD2_3C.h>
#include <Fonts/FreeMonoBold9pt7b.h>

// Amazon product: 
// Resolution 296 × 128

// e-paper library https://github.com/ZinggJM/GxEPD2


const uint8_t pin_dc = 26;
const uint8_t pin_busy = 48;
const uint8_t pin_rst = 47;

const uint8_t pin_cs = 34;
const uint8_t pin_mosi = 35;
const uint8_t pin_sck = 36;
const uint8_t pin_miso = 33;
GxEPD2_3C<GxEPD2_290c, GxEPD2_290c::HEIGHT> display(GxEPD2_290c(/*CS=D8*/ pin_cs, /*DC=D3*/ pin_dc, /*RST=D4*/ pin_rst, /*BUSY=D2*/ pin_busy));

SPIClass fspi = SPIClass(FSPI);
const char * HelloWorld = "Hello, world!";

void helloWorld()
{
  Serial.println("helloWorld");
  display.setRotation(1);
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
  int16_t tbx, tby; uint16_t tbw, tbh;
  display.getTextBounds("HelloWorld", 0, 0, &tbx, &tby, &tbw, &tbh);
  // center bounding box by transposition of origin:
  uint16_t x = ((display.width() - tbw) / 2) - tbx;
  uint16_t y = ((display.height() - tbh) / 2) - tby;
  display.setFullWindow();
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_BLACK);
    display.setCursor(x, y);
    display.print(HelloWorld);
    //display.display();
  }
  while (display.nextPage());
  //display.nextPage();
  Serial.println("helloWorld done");
}

void helloFullScreenPartialMode()
{
  //Serial.println("helloFullScreenPartialMode");
  const char fullscreen[] = "full screen update";
  const char fpm[] = "fast partial mode";
  const char spm[] = "slow partial mode";
  const char npm[] = "no partial mode";
  display.setPartialWindow(0, 0, display.width(), display.height());
  display.setRotation(1);
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
  const char* updatemode;
  if (display.epd2.hasFastPartialUpdate)
  {
    updatemode = fpm;
  }
  else if (display.epd2.hasPartialUpdate)
  {
    updatemode = spm;
  }
  else
  {
    updatemode = npm;
  }
  // do this outside of the loop
  int16_t tbx, tby; uint16_t tbw, tbh;
  // center update text
  display.getTextBounds(fullscreen, 0, 0, &tbx, &tby, &tbw, &tbh);
  uint16_t utx = ((display.width() - tbw) / 2) - tbx;
  uint16_t uty = ((display.height() / 4) - tbh / 2) - tby;
  // center update mode
  display.getTextBounds(updatemode, 0, 0, &tbx, &tby, &tbw, &tbh);
  uint16_t umx = ((display.width() - tbw) / 2) - tbx;
  uint16_t umy = ((display.height() * 3 / 4) - tbh / 2) - tby;
  // center HelloWorld
  display.getTextBounds(HelloWorld, 0, 0, &tbx, &tby, &tbw, &tbh);
  uint16_t hwx = ((display.width() - tbw) / 2) - tbx;
  uint16_t hwy = ((display.height() - tbh) / 2) - tby;
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(hwx, hwy);
    display.print(HelloWorld);
    display.setCursor(utx, uty);
    display.print(fullscreen);
    display.setCursor(umx, umy);
    display.print(updatemode);
  }
  while (display.nextPage());
  //Serial.println("helloFullScreenPartialMode done");
}

void setup() {
  Serial.begin(921600);
  delay(3000);
  Serial.println("Begin Setup");

 

  fspi.begin(pin_sck, pin_miso, pin_mosi, pin_miso);
  display.epd2.selectSPI(fspi, SPISettings(4000000, MSBFIRST, SPI_MODE0));
  
  Serial.println("Initializing display");
  display.init(921600, true, 2, false);

  display.clearScreen(GxEPD_RED);

  display.setTextColor(GxEPD_BLACK);
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_RED);
  }
  while (display.nextPage());


  Serial.println("Begin writing to display");
  helloWorld();
  // helloFullScreenPartialMode();
  Serial.println("Done Writing to hellow world");
}

/*
  FSI Pins / Connected

         Heltec    E-Paper
  GPIO33 n/c
  GPIO34 FSPICS0  CS
  GPIO35 FSPID    DIN
  GPIO36 FSPICLK  CLK
  GPIO37
  GPIO38



  customizing SPI: https://randomnerdtutorials.com/esp32-spi-communication-arduino/
*/
void loop() {
  delay(2000);
  Serial.println("Begin Writing to hello world parital");
  helloFullScreenPartialMode();
  Serial.println("Done Writing to hello world parital");
}