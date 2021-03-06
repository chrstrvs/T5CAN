// list of the used symbols and their index number.
// index nr defines the address, length, label and unit of the symbol.

#include <Arduino.h>

// total number of symbols
/////////////////////////////////////
const byte symbols = 15;

// symbols index nr
// you can added or delete symbols here
// change symbols nr acordingly above here
/////////////////////////////////////
const byte P_Manifold = 0;
const byte P_Manifold10 = 1;
const byte Medeltryck = 2;
const byte Regl_tryck = 3;
const byte Medellast = 4;
const byte Batt_volt = 5;
const byte Kyl_temp = 6;
const byte Lufttemp = 7;
const byte Rpm = 8;
const byte Rpm_byte = 9;
const byte Rpm_pol = 10;
const byte Bil_hast = 11;
const byte Gear = 12;
const byte Apc_decrese = 13;
const byte Knock_status = 14;

// Arrays for the symbol information
/////////////////////////////////////
uint16_t symbol_Address[symbols]; // SRAM Address (0x0000)
byte symbol_Length[symbols];      // symbol length (data)
String symbol_Label[symbols];     // symbol representation (name)
String symbol_Unit[symbols];      // symbol unit (kPa, V, C, etc)
int16_t symbol_offset[symbols];   // symbol offset (-100 etc)
float symbol_factor[symbols];     // symbol factor (*100 etc)

// run this at setup,
// it fills the arrays with your symbols
// you can edit your symbols here:
/////////////////////////////////////
void setSymbols(){
// Symbols
/////////////////////////////////////
symbol_Address[P_Manifold] = 0x1013;
symbol_Length[P_Manifold] = 1;
symbol_Label[P_Manifold] = "Boost";
symbol_Unit[P_Manifold] = "Bar";
symbol_offset[P_Manifold] = -100;  // 100kPa = 1 bar, starting at -1 bar
symbol_factor[P_Manifold] = 0.01;
/////////////////////////////////////
symbol_Address[P_Manifold10] = 0x1060;
symbol_Length[P_Manifold10] = 2;
symbol_Label[P_Manifold10] = "Boost";
symbol_Unit[P_Manifold10] = "Bar";
symbol_offset[P_Manifold10] = -100;  
symbol_factor[P_Manifold10] = 0.01;
/////////////////////////////////////
symbol_Address[Medeltryck] = 0x334D;
symbol_Length[Medeltryck] = 1;
symbol_Label[Medeltryck] = "avg Boost";
symbol_Unit[Medeltryck] = "Bar";
symbol_offset[Medeltryck] = -100;  
symbol_factor[Medeltryck] = 0.01;
/////////////////////////////////////
symbol_Address[Regl_tryck] = 0x3353;
symbol_Length[Regl_tryck] = 1;
symbol_Label[Regl_tryck] = "Target Boost";
symbol_Unit[Regl_tryck] = "Bar";
symbol_offset[Regl_tryck] = -100;  
symbol_factor[Regl_tryck] = 0.01;
/////////////////////////////////////
symbol_Address[Medellast] = 0x1037;
symbol_Length[Medellast] = 1;
symbol_Label[Medellast] = "avg Load";
symbol_Unit[Medellast] = "Nm?";
symbol_offset[Medellast] = 0;  
symbol_factor[Medellast] = 1;
/////////////////////////////////////
symbol_Address[Batt_volt] = 0x101B;
symbol_Length[Batt_volt] = 1;
symbol_Label[Batt_volt] = "Battery";
symbol_Unit[Batt_volt] = "V";
symbol_offset[Batt_volt] = 0;  
symbol_factor[Batt_volt] = 1;
/////////////////////////////////////
symbol_Address[Kyl_temp] = 0x102E;
symbol_Length[Kyl_temp] = 1;
symbol_Label[Kyl_temp] = "Coolant";
symbol_Unit[Kyl_temp] = "C";
symbol_offset[Kyl_temp] = 0;  
symbol_factor[Kyl_temp] = 1;
/////////////////////////////////////
symbol_Address[Lufttemp] = 0x1033;
symbol_Length[Lufttemp] = 1;
symbol_Label[Lufttemp] = "IAT";
symbol_Unit[Lufttemp] = "C";
symbol_offset[Lufttemp] = 0;  
symbol_factor[Lufttemp] = 1;
/////////////////////////////////////
symbol_Address[Rpm] = 0x1066;
symbol_Length[Rpm] = 2;
symbol_Label[Rpm] = "RPM";
symbol_Unit[Rpm] = "rpm";
symbol_offset[Rpm] = 0;  
symbol_factor[Rpm] = 1;
/////////////////////////////////////
symbol_Address[Rpm_byte] = 0x46EA;
symbol_Length[Rpm_byte] = 1;
symbol_Label[Rpm_byte] = "RPM";
symbol_Unit[Rpm_byte] = "rpm";
symbol_offset[Rpm_byte] = 0;  
symbol_factor[Rpm_byte] = 1;
/////////////////////////////////////
symbol_Address[Rpm_pol] = 0x1048;
symbol_Length[Rpm_pol] = 1;
symbol_Label[Rpm_pol] = "RPM";
symbol_Unit[Rpm_pol] = "rpm";
symbol_offset[Rpm_pol] = 0;  
symbol_factor[Rpm_pol] = 1;
/////////////////////////////////////
symbol_Address[Bil_hast] = 0x101C;
symbol_Length[Bil_hast] = 1;
symbol_Label[Bil_hast] = "Speed";
symbol_Unit[Bil_hast] = "km/u";
symbol_offset[Bil_hast] = 0;  
symbol_factor[Bil_hast] = 1;
/////////////////////////////////////
symbol_Address[Gear] = 0x3355;
symbol_Length[Gear] = 1;
symbol_Label[Gear] = "Gear";
symbol_Unit[Gear] = "-";
symbol_offset[Gear] = 0;  
symbol_factor[Gear] = 1;
/////////////////////////////////////
symbol_Address[Apc_decrese] = 0x334E;
symbol_Length[Apc_decrese] = 1;
symbol_Label[Apc_decrese] = "Apc Decrease";
symbol_Unit[Apc_decrese] = "bar";
symbol_offset[Apc_decrese] = 0;  
symbol_factor[Apc_decrese] = 1;
/////////////////////////////////////
symbol_Address[Knock_status] = 0x1119;
symbol_Length[Knock_status] = 1;
symbol_Label[Knock_status] = "Knock";
symbol_Unit[Knock_status] = "-";
symbol_offset[Knock_status] = 0;  
symbol_factor[Knock_status] = 1;
/////////////////////////////////////
Serial.println("Symbols loaded from memory");
}

