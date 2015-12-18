/*
 * cc1120 test code.
 * used a TeensyLC on SPI0 with
 *   pin 10 = SPI chip select
 *   pin 7  = cc1120 reset pin
 * Copyright 2015 Alan Marchiori
 */


// include the SPI library:
#include <SPI.h>

#include "components/devices/cc112x/cc112x_spi.h"
#include "components/targets/teensy/hal_spi_rf_trxeb.h"

#include "rfsettings.h"

// the Teensy had hardware CS, so it should do this automatically.
const int cs_cc1120 = 10;
const int cc1120_reset = 7;

union cc1120_status {
  struct {
    uint8_t reserved : 4;
    uint8_t STATE : 3;
    uint8_t CHIP_RDY : 1;
  } fields;
  uint8 data;
};

void setup() {
  Serial.begin(9600);
  Serial.print("Serial Initialized to 9600 baud\n");

  // initialize the generic TI trx interface
  trxRfSpiInterfaceInit(0);
  Serial.print("Teensy TI SPI HAL interface initialized\n");

  // configure the radio to auto-generated specs
  // packet type: fixed length
  // packet size: 5 bytes
  configureRadio();
}

uint8_t i;
cc1120_status status;

void loop() {
  Serial.print("Loop: ");
  Serial.print(i);
  Serial.print("\n");

  // put the radio into frequency on state
  status.data = trxSpiCmdStrobe(CC112X_SNOP);
  Serial.print("Top of loop state: ");
  Serial.print(status.fields.STATE);
  Serial.print("Chip ready: ");
  Serial.print(status.fields.CHIP_RDY);
  Serial.print("\n");

  // strobe to on
  Serial.print("After SFSTXON: ");
  status.data = trxSpiCmdStrobe(CC112X_STX);
  Serial.print(status.fields.STATE);
  Serial.print("\n");
  // put the radio into frequency on state
  status.data = trxSpiCmdStrobe(CC112X_SNOP);
  Serial.print("---: ");
  Serial.print(status.fields.STATE);
  Serial.print("\n");

  delay(10);

  // load up FIFO
  char dataToSend[] = "\05hello";
  cc112xSpiWriteTxFifo((uint8*)dataToSend, 6);

  delay(10);

  // send an STX
  status.data = trxSpiCmdStrobe(CC112X_STX);
  Serial.print("After STX: ");
  Serial.print(status.fields.STATE);
  Serial.print("\n");

  // flush the transmit strobe if there is an error
  if (status.fields.STATE == 7) {
    Serial.print("ERROR: Transmit FIFO error\n");
    trxSpiCmdStrobe(CC112X_SFTX);
  }

  i++;
  delay(500);
}
