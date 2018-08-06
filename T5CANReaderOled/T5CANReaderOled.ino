#include "symbols.h"
#include "T5CAN.h"
#include "Oled.h"

void setup()
{
  // initialize oled screen
  setOled();
  // initialize can comunication
  setT5CAN();
  
}

void loop()
{
  printSymbol(P_Manifold);
//  delay(200); // wait before reading recieved message
//  printSymbol(P_Manifold10);
//  delay(200); // wait before reading recieved message
//  printSymbol(Lufttemp);
//  delay(200); // wait before reading recieved message
  showBoostText(P_Manifold);

  symbolAlert();
}
