#include <mcp_can.h>
#include <SPI.h>


#define CAN0_INT 2    // Set INT to pin 2 for the Sparkfun CAN BUS shield
MCP_CAN CAN0(10);     // Set CS to pin 10 for the Sparkfun CAN BUS shield
long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
char msgString[128]; // Array to store serial string
byte sndStat;

// Message prototyes
/////////////////////////////////////
byte T5ReadFromSram[8] = {0xC7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; //Command to read sram
byte T5SendAck[8] = {0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; //Command to send acknowledge to T5
byte T5ReadSymTab1[8] = {0xC4, 0x53, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; //Command to read symbol table in T5 #1
byte T5ReadSymTab2[8] = {0xC4, 0x0D, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; //Command to read symbol table in T5 #2
byte T5ReadCommand[8] = {0xC4, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// Arrays for the symbols
/////////////////////////////////////
const byte symbols = 15;
uint16_t symbol_Address[symbols] = {0x0000};
byte symbol_Length[symbols];
String symbol_Label[symbols];
String symbol_Unit[symbols];

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

void setSymbols(){
// Symbols
/////////////////////////////////////
symbol_Address[P_Manifold] = 0x1013;
symbol_Length[P_Manifold] = 1;
symbol_Label[P_Manifold] = "Boost";
symbol_Unit[P_Manifold] = "Bar";
/////////////////////////////////////
symbol_Address[P_Manifold10] = 0x1060;
symbol_Length[P_Manifold10] = 2;
symbol_Label[P_Manifold10] = "Boost";
symbol_Unit[P_Manifold10] = "Bar";
/////////////////////////////////////
symbol_Address[Medeltryck] = 0x334D;
symbol_Length[Medeltryck] = 1;
symbol_Label[Medeltryck] = "avg Boost";
symbol_Unit[Medeltryck] = "Bar";
/////////////////////////////////////
symbol_Address[Regl_tryck] = 0x3353;
symbol_Length[Regl_tryck] = 1;
symbol_Label[Regl_tryck] = "Target Boost";
symbol_Unit[Regl_tryck] = "Bar";
/////////////////////////////////////
symbol_Address[Medellast] = 0x1037;
symbol_Length[Medellast] = 1;
symbol_Label[Medellast] = "avg Load";
symbol_Unit[Medellast] = "Nm?";
/////////////////////////////////////
symbol_Address[Batt_volt] = 0x101B;
symbol_Length[Batt_volt] = 1;
symbol_Label[Batt_volt] = "Battery";
symbol_Unit[Batt_volt] = "V";
/////////////////////////////////////
symbol_Address[Kyl_temp] = 0x102E;
symbol_Length[Kyl_temp] = 1;
symbol_Label[Kyl_temp] = "Coolant";
symbol_Unit[Kyl_temp] = "C";
/////////////////////////////////////
symbol_Address[Lufttemp] = 0x1033;
symbol_Length[Lufttemp] = 1;
symbol_Label[Lufttemp] = "IAT";
symbol_Unit[Lufttemp] = "C";
/////////////////////////////////////
symbol_Address[Rpm] = 0x1066;
symbol_Length[Rpm] = 2;
symbol_Label[Rpm] = "RPM";
symbol_Unit[Rpm] = "rpm";
/////////////////////////////////////
symbol_Address[Rpm_byte] = 0x46EA;
symbol_Length[Rpm_byte] = 1;
symbol_Label[Rpm_byte] = "RPM";
symbol_Unit[Rpm_byte] = "rpm";
/////////////////////////////////////
symbol_Address[Rpm_pol] = 0x1048;
symbol_Length[Rpm_pol] = 1;
symbol_Label[Rpm_pol] = "RPM";
symbol_Unit[Rpm_pol] = "rpm";
/////////////////////////////////////
symbol_Address[Bil_hast] = 0x101C;
symbol_Length[Bil_hast] = 1;
symbol_Label[Bil_hast] = "Speed";
symbol_Unit[Bil_hast] = "km/u";
/////////////////////////////////////
symbol_Address[Gear] = 0x3355;
symbol_Length[Gear] = 1;
symbol_Label[Gear] = "Gear";
symbol_Unit[Gear] = "-";
/////////////////////////////////////
symbol_Address[Apc_decrese] = 0x334E;
symbol_Length[Apc_decrese] = 1;
symbol_Label[Apc_decrese] = "Apc Decrease";
symbol_Unit[Apc_decrese] = "bar";
/////////////////////////////////////
symbol_Address[Knock_status] = 0x1119;
symbol_Length[Knock_status] = 1;
symbol_Label[Knock_status] = "Knock";
symbol_Unit[Knock_status] = "-";
/////////////////////////////////////

}
void setup()
{
  setSymbols();
  Serial.begin(115200);
  // Initialize MCP2515 running at 16MHz,
  //with a baudrate of 615kb/s and the masks and filters disabled.
  if (CAN0.begin(MCP_ANY, CAN_615KBPS, MCP_16MHZ) == CAN_OK)
    Serial.println("MCP2515 Initialized Successfully!");
  else
    Serial.println("Error Initializing MCP2515...");
  CAN0.setMode(MCP_NORMAL);   // Change to normal mode to allow messages to be transmitted
  pinMode(CAN0_INT, INPUT);   // Configuring pin for /INT input
}

void loop()
{
  printSymbol(P_Manifold);
}

void printSymbol(byte nr) {

  Serial.println(symbol_Label[nr] + ": " + (int (getSymbol(nr)-100)*0.01) + " " + symbol_Unit[nr]);
}

uint16_t getSymbol(byte nr) {
  byte data[8] = {0xC7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; //Command to read sram
  data[4] = symbol_Address[nr]; //AL
  data[3] = (symbol_Address[nr] >> 8); //AH

  Send_msg(data); //Sends the data to T5
  delay(200); // wait before reading recieved message

  //Read message
  if (!digitalRead(CAN0_INT))                        // If CAN0_INT pin is low, read receive buffer
  {
    CAN0.readMsgBuf(&rxId, &len, rxBuf);      // Read data: len = data length, buf = data byte(s)

    for (int i = 0; i < 8; i++) {
      data[i] = rxBuf[i];
    }
    if (symbol_Length[nr] == 1) {
      return rxBuf[2]; // 1st byte
    }
    else if (symbol_Length[nr] == 2) {
      uint16_t val = rxBuf[2] | (rxBuf[3] << 8);
      return val;
    }
    else {
      Serial.println("Symbol length more than 2");
      return 0;
    }
  }
}

void Send_msg(byte msg[8]) {
  sndStat = CAN0.sendMsgBuf(0x05, 0, 8, msg);
  if (sndStat == CAN_OK) {
    Serial.println("Message Sent Successfully!");
    Serial.print("Sent: ");
    PrintHex8(msg, 8); //Prints the sent data to the serial monitor, just to be sure the correct data is being sent
    Serial.println();
  } else {
    Serial.println("Error Sending Message: ");
  }
}

void PrintHex8(uint8_t *ddata, uint8_t length) // prints 8-bit data in hex with leading zeroes
{
  for (int i = 0; i < length; i++) {
    Serial.print("0x");
    if (ddata[i] < 0x10) {
      Serial.print("0");
    }
    Serial.print(ddata[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}
void Read_msg() {
  byte dat[8] = {0x00};
  if (!digitalRead(CAN0_INT))                        // If CAN0_INT pin is low, read receive buffer
  {
    CAN0.readMsgBuf(&rxId, &len, rxBuf);      // Read data: len = data length, buf = data byte(s)

    for (int i = 0; i < 8; i++) {
      dat[i] = rxBuf[i];
    }
    Serial.print("Recieved: "); PrintHex8(dat, 8); Serial.print("\n");
    Serial.println(rxBuf[2], DEC); // 1st byte
    // Serial.println(rxBuf[3], DEC); // 2nd byte
    // Serial.println(rxBuf[4], DEC); // 3rd byte

  }
  Serial.println();
}

