#include "symbols.h"
#include "T5CAN.h"
#include "Oled.h"

void setup()
{
  // initialize can comunication
  setT5CAN();
  // initialize oled screen
  setOled();

}

void loop()
{
  //  printSymbol(P_Manifold);
  for (int i = 0; i < 30; i++) {
    displayBoost(P_Manifold);
    printSymbol(P_Manifold);

  }
  for (int i = 0; i < 30; i++) {
    displayIat(Lufttemp);
    printSymbol(Lufttemp);

  }
  //symbolAlert();
}

