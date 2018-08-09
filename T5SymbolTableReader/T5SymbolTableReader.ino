#include <mcp_can.h>

#define CAN0_INT 2    // Set INT to pin 2 for the Sparkfun CAN BUS shield
MCP_CAN CAN0(10);     // Set CS to pin 10 for Uno or 53 for Mega 2560 for the Sparkfun CAN BUS shield

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
char msgString[128];                        // Array to store Serial string
byte data[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
byte sndStat;
byte T5SendAck[8] = {0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; //Command to send acknowledge to T5
byte T5ReadSymTab1[8] = {0xC4, 0x53, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; //Command to read symbol table in T5 #1
byte T5ReadSymTab2[8] = {0xC4, 0x0D, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; //Command to read symbol table in T5 #2
byte T5ReadFromSram[10][8];
String T5SearchForSymbols[10] = {"P_Manifold", "AD_EGR", "AD_trot"};
char symbolBuf[32];
char rxBufChar;
String rxBufString;
char symbolCompare[32];
int symbolLength;
int symbolCounter;
int readSymbols = 100; //Number of symbols to read
char byteBuf[8];
int getSymbolTableDone = 0;
char *endptr;

void setup() {

  Serial.begin(115200);

  // Initialize MCP2515 running at 16MHz with a baudrate of 615kb/s and the masks and filters disabled.
  if (CAN0.begin(MCP_ANY, CAN_615KBPS, MCP_16MHZ) == CAN_OK)
    Serial.println("MCP2515 Initialized Successfully!");
  else
    Serial.println("Error Initializing MCP2515...");

  CAN0.setMode(MCP_NORMAL);   // Change to normal mode to allow messages to be transmitted

  pinMode(CAN0_INT, INPUT);   // Configuring pin for /INT input

  // Send command #1 to read symbol table. The for loop copies the data from the byte array T5ReadSymTab1 to the byte array data
  for (int i = 0; i < 8; i++) {
    (data[i] = T5ReadSymTab1[i]);
  }
  Send_msg(); //Sends the data to T5

  Read_msg(); //Read the incoming message T5

  // Send command #2 to read symbol table
  for (int i = 0; i < 8; i++) {
    (data[i] = T5ReadSymTab2[i]);
  }
  Send_msg();

  //Read message
  Read_msg();

  //Read number of symbols set in readSymbols or until no more symbols are sent
  for (int i = 0; i < 8; i++) {
    (data[i] = T5SendAck[i]);
  }

  for (symbolCounter = 0; symbolCounter <= readSymbols;) {
    if (rxBufString.indexOf("END") > 0) {
      Send_msg();
      break;
    }
    Send_msg();

    Read_msg(); //Read the incoming message T5

  }

  Read_msg(); //Read the incoming message T5

  for (int p = 0; p < sizeof (T5ReadFromSram) / sizeof (T5ReadFromSram[0]);) {
    Serial.println(String(T5SearchForSymbols[p]) + " " + T5ReadFromSram[p][3] + T5ReadFromSram[p][4]);
    p++;
  }

  delay(1000);

  getSymbolTableDone = 1;
}

void loop() {

}

void Send_msg() {
  sndStat = CAN0.sendMsgBuf(0x05, 0, 8, data);
}

void Read_msg() {

  if (!digitalRead(CAN0_INT))                        // If CAN0_INT pin is low, read receive buffer
  {
    CAN0.readMsgBuf(&rxId, &len, rxBuf);      // Read data: len = data length, buf = data byte(s)

    if (getSymbolTableDone == 0) {
      //Compile a string of the incoming message unless rxBuf[2] == 0x0D or 0x0A, which marks the end of a read symbol
      if (rxBuf[2] != 0x0D || rxBuf[2] != 0x0A) {
        rxBufChar = rxBuf[2];                   //Convert the incoming message to char
        rxBufString = rxBufString + rxBufChar;  //Add the char to the string rxBufString
      }

      if (rxBuf[2] == 0x0D) {
        rxBufString.trim();
        symbolLength = rxBufString.length();
        rxBufString.substring(8, symbolLength).toCharArray(symbolCompare, symbolLength);
        Serial.println(String(symbolCounter) + " " + symbolCompare + " " + rxBufString.substring(0, 4) + " " + rxBufString.substring(4, 8));

        for (int l = 0; l < sizeof (T5ReadFromSram) / sizeof (T5ReadFromSram[0]);) {
          T5SearchForSymbols[l].toCharArray(symbolBuf, 32);
          if (strcmp(symbolCompare, symbolBuf) == 0) {
            rxBufString.substring(0, 2).toCharArray(byteBuf, 3);
            T5ReadFromSram[l][3] = strtol(byteBuf, &endptr, 16);
            rxBufString.substring(2, 4).toCharArray(byteBuf, 3);
            T5ReadFromSram[l][4] = strtol(byteBuf, &endptr, 16);
            T5ReadFromSram[l][0] = 0xC7;
          }
          l++;
        }

        rxBufString = ""; //Empty the string buffer
        symbolCounter++;
      }
    }
  }
}
