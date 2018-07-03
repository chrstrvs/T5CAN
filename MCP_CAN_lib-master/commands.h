byte T5_RDCMND[] {0xC4, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // C4 Command template for reading something from the T5 ECU
byte T5_WRCMND[] {0xC4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // C4 Command template for writing something to the T5 ECU
byte T5ACKCMND[] {0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // C6 Command for acknowledging receipt of last message from T5 ECU or

byte T5RAMCMND[] {0xC7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // C7 Command for reading T5 RAM (all the symbols and adaption data is in RAM)
byte GETBOOST[] {0xC7, 0x00, 0x10, 0x13, 0x00, 0x00, 0x00, 0x00}; // C7 Command for reading T5 RAM (all the symbols and adaption data is in RAM)
byte GETBAT[] {0xC7, 0x00, 0x10, 0x1B, 0x00, 0x00, 0x00, 0x00}; // C7 Command for reading T5 RAM (all the symbols and adaption data is in RAM)

byte AH = 0x10;
byte AL = 0x1B;
byte GETADDRESS[] {0xC7, 0x00, 0x00, AL, AH, 0x00, 0x00, 0x00}; // C7 Command for reading T5 RAM (all the symbols and adaption data is in RAM)

byte T5STARTMS[] {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00}; // T5 sends this message whenever the ignition is turned on? - message id is RESPID
byte T5RESETMSG[] {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // T5 sends this message to say that it has reset - message id is RSETID



uint8_t MISCMSG    [8] = {0};

const uint8_t REBOOT = 0xC2;
const uint8_t ERASE5 = 0xC0;
uint8_t FoundSymbols;


