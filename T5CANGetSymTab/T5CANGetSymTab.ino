
#include <mcp_can.h>
#include <SPI.h>


#define CAN0_INT 2    // Set INT to pin 2 for the Sparkfun CAN BUS shield
MCP_CAN CAN0(10);     // Set CS to pin 10 for the Sparkfun CAN BUS shield

/////////////////////////////////////
String symbol_label[14] = {"Boost", "Boost", "avg Boost", "Target Boost", "avg Load", "Battery", "Coolant", "IAT", "RPM", "RPM", "Speed", "Gear", "APC Decrease", "Knock",};
byte symbol_AH[14] =      {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
byte symbol_AL[14] =      {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
byte symbol_Length[14] =  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
/////////////////////////////////////
byte P_Manifold = 0;     // boost press, length = 1
byte P_manifold10 = 1;   // boost press, length = 2
byte Medeltryck = 2;     // avg boost press, length = 1
byte Regl_tryck = 3;     // target pressure, length = 1
byte Medellast = 4;      // avg load, length = 1
byte Batt_volt = 5;      // battery voltage, length = 1
byte Kyl_temp = 6;       // coolant temp, length = 1
byte Lufttemp = 7;       // iat, lenght = 1
byte Rpm = 8;            // rpm, length = 2
byte Rpm_pol = 9;        // rpm, length = 1
byte Bil_hast = 10;      // vehicle speed, length = 1
byte Gear = 11;          // current gear, length = 1
byte Apc_decrease = 12;  // apc decrease, length = 1
byte Knock_status = 13;  // knock status, length = 1
//Adapt_injfaktor


long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[128];
//char msgString[128];                        // Array to store serial string
byte sndStat;
//byte T5ReadFromSram[8] = {0xC7, 0x00, 0x00, 0x10, 0x2D, 0x00, 0x00, 0x00}; //Command to read Kyl_temp in my specific T5 binary
byte T5ReadFromSram[8] = {0xC7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; //Command to read Kyl_temp in my specific T5 binary
byte T5SendAck[8] = {0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; //Command to send acknowledge to T5
byte T5ReadSymTab1[8] = {0xC4, 0x53, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; //Command to read symbol table in T5 #1/2
byte T5ReadSymTab2[8] = {0xC4, 0x0D, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; //Command to read symbol table in T5 #2/2



void setup()
{
  Serial.begin(115200);

  // Initialize MCP2515 running at 16MHz with a baudrate of 615kb/s and the masks and filters disabled.
  if (CAN0.begin(MCP_ANY, CAN_615KBPS, MCP_16MHZ) == CAN_OK)
    Serial.println("MCP2515 Initialized Successfully!");
  else
    Serial.println("Error Initializing MCP2515...");

  CAN0.setMode(MCP_NORMAL);   // Change to normal mode to allow messages to be transmitted

  pinMode(CAN0_INT, INPUT);   // Configuring pin for /INT input

  getSymbolTable();

  printSymbolTable();

}

void loop()
{
  int value = getValue(P_Manifold);
  Serial.println(symbol_label[P_Manifold] + ": " + value);
}

int getValue(byte indexNr) {
  int val = 0;
  byte data[8];
  for (int i = 0; i < 8; i++) {
    (data[i] = T5ReadFromSram[i]);
  }
  data[4] = symbol_AH[indexNr];
  data[3] = symbol_AL[indexNr];

  // Send command to read
  Send_msg(data); //Sends the data to T5
  PrintHex8(data, 8); Serial.print("\n"); //Prints the sent data to the serial monitor, just to be sure the correct data is being sent

  delay(200);

  //Read message
  Read_msg(); //Read the incoming message T5

  if (symbol_Length[indexNr] == 1) {
    val = rxBuf[2];
  }
  //else if
  return val;
}

void requestSymbolTableCommand() {
  // Send command #1 to read symbol table. The for loop copies the data from the byte array T5ReadSymTab1 to the byte array data
  Send_msg(T5ReadSymTab1); //Sends the data to T5
  delay(200);
  //Read message
  Read_msg(); //Read the incoming message T5
  delay(200);
  // Send command #2 to read symbol table
  Send_msg(T5ReadSymTab2);
  delay(200);
  //Read message
  Read_msg();
  delay(200);
}

bool findDelimiter() {
  Send_msg(T5SendAck);
  delay(10); //Not sure if a delay is needed here, but we'll start with 10 ms
  Read_msg(); //Read the incoming message T5
  // if D and then A
  if (rxBuf[2] == 13) {
    Send_msg(T5SendAck);
    delay(10); //Not sure if a delay is needed here, but we'll start with 10 ms
    Read_msg(); //Read the incoming message T5
    if (rxBuf[2] == 10)
    {
      return 1;
    }
  }
  else {
    return 0;
  }

}

void saveSymbol(byte symbol[], byte s) {
  char symbolName[s-4];
  for (int i = 4; i < s; i++) {
    if (symbol[i]==13) {
      if (symbol[i+1]==10) {
        i==32;
      }
    }
    else {
    symbolName[i - 4] = symbol[i];
    }

  }
  byte adress[2];
  
  byte leng[2];


   String str((char*)symbolName);
   if(str == "P_Manifold"){
    symbol_AH[P_Manifold] = (symbol[0]-30) << 4; 
    symbol_AH[P_Manifold] = (symbol[1]-30);
    symbol_AL[P_Manifold] = (symbol[2]-30) << 4;
    symbol_AL[P_Manifold] = (symbol[3]-30);
   }

   else if(str == "P_Manifold10"){
    
   }
   else if(str == "Medeltryck"){
    
   }
   else if(str == "Regl_tryck"){
    
   }
   else if(str == "Medellast"){
    
   }
   else if(str == "Batt_volt"){
    
   }
   else if(str == "Kyl_temp"){
    
   }
   else if(str == "Lufttemp"){
    
   }
   else if(str == "Rpm"){
    
   }
   else if(str == "Rpm_pol"){
    
   }
   else if(str == "Bil_hast"){
    
   }
   else if(str == "Gear"){
    
   }
   else if(str == "Apc_decrease"){
    
   }
   else if(str == "Knock_status"){
    
   }
}


void getSymbolTable() {
  byte symbolCounter = 0;
  byte symbol[32] = {0};
  byte e = 0;
  requestSymbolTableCommand();

  // find symbol delimiter
  while (!findDelimiter) {
    e++;
  }
  e = 0;
  while (e <= 32) {
    for (int j = 0; j < 32; j++) {
      e = j;
      if (findDelimiter) {
        j = 32;
      }
      else {
        symbol[j] = rxBuf[2];
      }
    }
    saveSymbol(symbol,32);
    e = 0;
    for (int i = 0; i < 8; i++) {
      (symbol[i] = {0});
    }

    symbolCounter++;
    Serial.print("symbols: ");
    Serial.println(symbolCounter + " / 700 done");
    // print percentage done

  }
  Serial.println("Symbol table readout done");
}

void printSymbolTable() {
  Serial.println("symbols available:"); // print list of symbols
}

void Send_msg(byte msg[8]) {
  sndStat = CAN0.sendMsgBuf(0x05, 0, 8, msg);
  if (sndStat == CAN_OK) {
    Serial.print("Message Sent Successfully!");
  }
  else {
    Serial.print("Error Sending Message: ");
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
}
void Read_msg() {
  if (!digitalRead(CAN0_INT))  // If CAN0_INT pin is low, read receive buffer
  {
    CAN0.readMsgBuf(&rxId, &len, rxBuf);   // Read data: len = data length, buf = data byte(s)
  }
}

