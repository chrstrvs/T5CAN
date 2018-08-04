#include "symbols.h"
#include "T5CAN.h"
#include "Oled.h"

void setup()
{
  setT5CAN();
  setOled();
}

void loop()
{
  printSymbol(P_Manifold);
  printSymbol(P_Manifold10);
  printSymbol(Rpm);
  showBoostText(P_Manifold);

}
