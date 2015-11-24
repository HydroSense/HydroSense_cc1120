/*
 * cc1120 test code.
 * used a TeensyLC on SPI0 with 
 *   pin 10 = SPI chip select
 *   pin 7  = cc1120 reset pin
 * Copyright 2015 Alan Marchiori
 */


// include the SPI library:
#include <SPI.h>


struct cc1120_status {
 uint8_t reserved : 4;
 uint8_t STATE : 3;
 uint8_t CHIP_RDY : 1;
};
union cc_st{
  struct cc1120_status st;
  uint8_t val;
};


SPISettings s = SPISettings(4000000, MSBFIRST, SPI_MODE0);
union cc_st status;

// the Teensy had hardware CS, so it should do this automatically.
const int cs_cc1120 = 10;

const int cc1120_reset = 7;


// misc bits
#define READ_BIT 0x80

// registers
#define FREQ0 0x0e
#define EXTENDED_ADDRESS 0x2f

// strobes
#define SNOP     0x3d
#define SFSTXON  0x31
#define SXOFF    0x32

// extended registers
#define PARTNUMBER 0x8f
#define PARTVERSION 0x90

void setup() {
  
  Serial.begin(9600);
  
  // set the slaveSelectPin as an output:
  pinMode (cs_cc1120, OUTPUT);
  digitalWrite (cs_cc1120, HIGH);
  
  pinMode (cc1120_reset, OUTPUT);
  digitalWrite (cc1120_reset, HIGH);
  // initialize SPI:
  SPI.begin(); 
}

uint8_t i;
void loop() {
   uint8_t val;
  
  
  strobe ( SNOP );  
  
  Serial.print("cc1120_status.CHIP_RDY: ");
  Serial.print(status.st.CHIP_RDY, HEX);
  Serial.print(" cc1120_status..STATE:");
  Serial.print(status.st.STATE, HEX);
  Serial.print("\n");

  
  val = readExtendedRegister( PARTNUMBER );
  Serial.print("Partnumber: 0x");
  Serial.print(val, HEX);
  Serial.print("\n");
  
  val = readExtendedRegister( PARTVERSION );
  Serial.print("Partversion: 0x");
  Serial.print(val, HEX);
  Serial.print("\n");
  
  
  writeRegister( FREQ0, i );
  Serial.print("write Freq0: 0x");
  Serial.print(i, HEX);
  Serial.print("\n");

  val = readRegister( FREQ0 );
  Serial.print("Ferq0: 0x");  
  Serial.print(val, HEX);
  Serial.print("\n");
  
  
  if ( i & 0x1 ){
    strobe( SFSTXON );
    
  } else {
    strobe ( SXOFF );
  }
  
  i++;
  
  delay(500);
}

void strobe ( uint8_t strobe ){  
  SPI.beginTransaction(s);
  digitalWrite(cs_cc1120, LOW);
  status.val = SPI.transfer( READ_BIT | strobe );
  digitalWrite(cs_cc1120, HIGH);
  SPI.endTransaction();  
}

uint8_t transferByte(uint8_t cmd_address, uint8_t value) {
  uint8_t r;
  SPI.beginTransaction(s);
  digitalWrite(cs_cc1120, LOW);
  status.val = SPI.transfer( cmd_address );
  r = SPI.transfer( value ); // do the write.
  digitalWrite(cs_cc1120, HIGH);
  SPI.endTransaction();
  
  return r;
}
uint8_t writeRegister(uint8_t address, uint8_t value) {
  return transferByte( address, value );
}

uint8_t readRegister(uint8_t address) {
  return transferByte( READ_BIT | address, 0x00 );
}

uint8_t readExtendedRegister(uint8_t extended_address) {
  uint8_t r;
  SPI.beginTransaction(s);
  digitalWrite(cs_cc1120, LOW);
  status.val = SPI.transfer(READ_BIT | EXTENDED_ADDRESS);
  SPI.transfer(extended_address);
  r = SPI.transfer(0xff);
  digitalWrite(cs_cc1120, HIGH);
  SPI.endTransaction();
  return r;
}
