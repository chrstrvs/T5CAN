#include <Arduino.h>
#include <Adafruit_SSD1306.h>

#include "font.h"

#define OLED_RESET 4
Adafruit_SSD1306 display1(OLED_RESET);

// run this at setup,
// it initializes the oled screen 
/////////////////////////////////////
void setOled() {
  display1.setTextColor(WHITE);
  display1.begin(SSD1306_SWITCHCAPVCC, 0x3D);  // initialize with the I2C addr 0x3D (for the 128x64). Change address from 0x3D to 0x3C
  display1.display();
  delay(2000);
}

// prints boost value to oled screen
/////////////////////////////////////
void showBoostText(byte nr) {
  display1.clearDisplay();
  display1.setFont(&Dialog_plain_48);
  display1.setCursor(0, 53);
  //display1.print(" ");
  display1.print(getSymbol(nr), 2);
  display1.display(); //you have to tell the display to...display
}
