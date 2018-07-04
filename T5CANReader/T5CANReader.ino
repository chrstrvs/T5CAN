// CAN Send Example
//

#include <mcp_can.h>
#include <SPI.h>

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
char msgString[128];                        // Array to store serial string
byte data[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
byte sndStat;
byte T5ReadFromSram[8] = {0xC7, 0x00, 0x00, 0x10, 0x2D, 0x00, 0x00, 0x00}; //Command to read Kyl_temp in my specific T5 binary
byte T5SendAck[8] = {0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; //Command to send acknowledge to T5
byte T5ReadSymTab1[8] = {0xC4, 0x53, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; //Command to read symbol table in T5 #1
byte T5ReadSymTab2[8] = {0xC4, 0x0D, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; //Command to read symbol table in T5 #2

#define CAN0_INT 2    // Set INT to pin 2 for the Sparkfun CAN BUS shield
MCP_CAN CAN0(10);     // Set CS to pin 10 for the Sparkfun CAN BUS shield

void setup()
{
  Serial.begin(115200);

  // Initialize MCP2515 running at 16MHz with a baudrate of 615kb/s and the masks and filters disabled.
  if(CAN0.begin(MCP_ANY, CAN_615KBPS, MCP_16MHZ) == CAN_OK)
    Serial.println("MCP2515 Initialized Successfully!");
  else
    Serial.println("Error Initializing MCP2515...");

  CAN0.setMode(MCP_NORMAL);   // Change to normal mode to allow messages to be transmitted

  pinMode(CAN0_INT, INPUT);   // Configuring pin for /INT input
}

void loop()
{
  // Send command to read Kyl_temp at address 00102D. The for loop copies the data from the byte array T5ReadFromSram to the byte array data
  for(int i = 0; i<8; i++){
        (data[i] = T5ReadFromSram[i]);
      }
  Send_msg(); //Sends the data to T5
  PrintHex8(data,8); Serial.print("\n"); //Prints the sent data to the serial monitor, just to be sure the correct data is being sent
  
  delay(200);

  //Read message
  Read_msg(); //Read the incoming message T5

  delay(1000);   //send the request every 1000 ms
}

void Send_msg(){
  sndStat = CAN0.sendMsgBuf(0x05, 0, 8, data);
  if(sndStat == CAN_OK){
    Serial.print("Message Sent Successfully!");
  } else {
    Serial.print("Error Sending Message: ");
  }
}

void PrintHex8(uint8_t *ddata, uint8_t length) // prints 8-bit data in hex with leading zeroes
{
       for (int i=0; i<length; i++) {
         Serial.print("0x");
         if (ddata[i]<0x10) {Serial.print("0");}
         Serial.print(ddata[i],HEX);
         Serial.print(" ");
       }
}
void Read_msg(){

if(!digitalRead(CAN0_INT))                         // If CAN0_INT pin is low, read receive buffer
  {
    CAN0.readMsgBuf(&rxId, &len, rxBuf);      // Read data: len = data length, buf = data byte(s)
    
    if((rxId & 0x80000000) == 0x80000000)     // Determine if ID is standard (11 bits) or extended (29 bits)
      sprintf(msgString, "Extended ID: 0x%.8lX  DLC: %1d  Data:", (rxId & 0x1FFFFFFF), len);
    else
      sprintf(msgString, "Standard ID: 0x%.3lX       DLC: %1d  Data:", rxId, len);
  
    Serial.print(msgString);
  
    if((rxId & 0x40000000) == 0x40000000){    // Determine if message is a remote request frame.
      sprintf(msgString, " REMOTE REQUEST FRAME");
      Serial.print(msgString);
    } else {
      for(byte i = 0; i<len; i++){
        sprintf(msgString, " 0x%.2X", rxBuf[i]);
        Serial.print(msgString);
      }
    }
    Serial.println();  
    Serial.println(rxBuf[2],DEC);
  }
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
