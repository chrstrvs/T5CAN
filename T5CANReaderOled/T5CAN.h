#include <Arduino.h>
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

void setT5CAN() {
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

void Send_msg(byte msg[8]) {
  sndStat = CAN0.sendMsgBuf(0x05, 0, 8, msg);
  if (sndStat == CAN_OK) {
    //Serial.println("Message Sent Successfully!");
    //Serial.print("Sent: ");
    //PrintHex8(msg, 8); //Prints the sent data to the serial monitor, just to be sure the correct data is being sent
    //Serial.println();
  } else {
    Serial.println("Error Sending Message: ");
  }
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
      Serial.println("error: symbol length more than 2");
      return 0;
    }
  }
}

void printSymbol(byte nr) {
  // manipulate the recieved data to view it correctly
  int16_t value = 0;
  switch (nr) {
    case P_Manifold:
      value = (getSymbol(nr) - 100) * 0.01;
      break;
    case P_Manifold10:
      value = (getSymbol(nr) - 100) * 0.01;
      break;
    case Medeltryck:
      value = getSymbol(nr);
      break;
    case Regl_tryck:
      value = getSymbol(nr);
      break;
    case Medellast:
      value = getSymbol(nr);
      break;
    case Batt_volt:
      value = getSymbol(nr);
      break;
    case Kyl_temp:
      value = getSymbol(nr);
      break;
    case Lufttemp:
      value = getSymbol(nr);
      break;
    case Rpm:
      value = getSymbol(nr);
      break;
    case Rpm_byte:
      value = getSymbol(nr);
      break;
    case Rpm_pol:
      value = getSymbol(nr);
      break;
    case Bil_hast:
      value = getSymbol(nr);
      break;
    case Gear:
      value = getSymbol(nr);
      break;
    case Apc_decrese:
      value = getSymbol(nr);
      break;
    case Knock_status:
      value = getSymbol(nr);
      break;
    default:
      value = getSymbol(nr);
      break;

  }
  Serial.println(symbol_Label[nr] + ": " + value + " " + symbol_Unit[nr]);
}
