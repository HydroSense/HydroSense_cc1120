/******************************************************************************
*  Filename: hal_spi_rf_trxeb.c
*
*  Description: Implementation file for common spi access with the CCxxxx
*               tranceiver radios using trxeb. Supports CC1101/CC112X radios
*
*  Copyright (C) 2013 Texas Instruments Incorporated - http://www.ti.com/
*
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*    Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*******************************************************************************/


/******************************************************************************
 * INCLUDES
 */
#include <Arduino.h>
#include <SPI.h>

#include "components/common/hal_types.h"
#include "components/common/hal_defs.h"
#include "components/targets/teensy/hal_spi_rf_trxeb.h"

// the Teensy had hardware CS, so it should do this automatically
const int cs_cc1120 = 10;
const int reset_cc1120 = 7;
SPISettings spi_settings = SPISettings(4000000, MSBFIRST, SPI_MODE0);

/******************************************************************************
 * LOCAL FUNCTIONS
 */
static void trxReadWriteBurstSingle(uint8 addr,uint8 *pData,uint16 len) ;


/******************************************************************************
 * FUNCTIONS
 */

/******************************************************************************
 * @fn          trxRfSpiInterfaceInit
 *
 * @brief       Function to initialize TRX SPI. CC1101/CC112x is currently
 *              supported. The supported prescalerValue must be set so that
 *              SMCLK/prescalerValue does not violate radio SPI constraints.
 *
 * input parameters
 *
 * @param       prescalerValue - SMCLK/prescalerValue gives SCLK frequency
 *
 * output parameters
 *
 * @return      void
 */
void trxRfSpiInterfaceInit(uint8 prescalerValue)
{
  // set slaveSelectPin as output
  pinMode(cs_cc1120, OUTPUT);
  digitalWrite(cs_cc1120, HIGH);

  // set the reset bits to OUTPUT
  pinMode(reset_cc1120, OUTPUT);
  digitalWrite(reset_cc1120, HIGH);

  // initialize SPI
  SPI.begin();
}


/*******************************************************************************
 * @fn          trx8BitRegAccess
 *
 * @brief       This function performs a read or write from/to a 8bit register
 *              address space. The function handles burst and single read/write
 *              as specfied in addrByte. Function assumes that chip is ready.
 *
 * input parameters
 *
 * @param       accessType - Specifies if this is a read or write and if it's
 *                           a single or burst access. Bitmask made up of
 *                           RADIO_BURST_ACCESS/RADIO_SINGLE_ACCESS/
 *                           RADIO_WRITE_ACCESS/RADIO_READ_ACCESS.
 * @param       addrByte - address byte of register.
 * @param       pData    - data array
 * @param       len      - Length of array to be read(TX)/written(RX)
 *
 * output parameters
 *
 * @return      chip status
 */
rfStatus_t trx8BitRegAccess(uint8 accessType, uint8 addrByte, uint8 *pData, uint16 len)
{
  // this is for non-extended access
  uint8 readValue;

  // start the transaction and assert the CS
  SPI.beginTransaction(spi_settings);
  digitalWrite(cs_cc1120, LOW);

  // transfer data
  readValue = SPI.transfer(accessType|addrByte);
  trxReadWriteBurstSingle(accessType|addrByte,pData,len);

  // finish up the transaction
  digitalWrite(cs_cc1120, HIGH);
  SPI.endTransaction();

  return readValue;
}

/******************************************************************************
 * @fn          trx16BitRegAccess
 *
 * @brief       This function performs a read or write in the extended adress
 *              space of CC112X.
 *
 * input parameters
 *
 * @param       accessType - Specifies if this is a read or write and if it's
 *                           a single or burst access. Bitmask made up of
 *                           RADIO_BURST_ACCESS/RADIO_SINGLE_ACCESS/
 *                           RADIO_WRITE_ACCESS/RADIO_READ_ACCESS.
 * @param       extAddr - Extended register space address = 0x2F.
 * @param       regAddr - Register address in the extended address space.
 * @param       *pData  - Pointer to data array for communication
 * @param       len     - Length of bytes to be read/written from/to radio
 *
 * output parameters
 *
 * @return      rfStatus_t
 */
rfStatus_t trx16BitRegAccess(uint8 accessType, uint8 extAddr, uint8 regAddr, uint8 *pData, uint8 len)
{
  // this is for non-extended access
  uint8 readValue;

  // start the transaction and assert the CS
  SPI.beginTransaction(spi_settings);
  digitalWrite(cs_cc1120, LOW);

  readValue = SPI.transfer(accessType|extAddr);
  SPI.transfer(regAddr);

  trxReadWriteBurstSingle(accessType|extAddr, pData, len);

  // finish up the transaction
  digitalWrite(cs_cc1120, HIGH);
  SPI.endTransaction();

  return readValue;
}

/*******************************************************************************
 * @fn          trxSpiCmdStrobe
 *
 * @brief       Send command strobe to the radio. Returns status byte read
 *              during transfer of command strobe. Validation of provided
 *              is not done. Function assumes chip is ready.
 *
 * input parameters
 *
 * @param       cmd - command strobe
 *
 * output parameters
 *
 * @return      status byte
 */
rfStatus_t trxSpiCmdStrobe(uint8 cmd)
{
  uint8 rc;
  SPI.beginTransaction(spi_settings);
  digitalWrite(cs_cc1120, LOW);

  rc = SPI.transfer(cmd);

  digitalWrite(cs_cc1120, HIGH);
  SPI.endTransaction();

  return rc;
}

/*******************************************************************************
 * @fn          trxReadWriteBurstSingle
 *
 * @brief       When the address byte is sent to the SPI slave, the next byte
 *              communicated is the data to be written or read. The address
 *              byte that holds information about read/write -and single/
 *              burst-access is provided to this function.
 *
 *              Depending on these two bits this function will write len bytes to
 *              the radio in burst mode or read len bytes from the radio in burst
 *              mode if the burst bit is set. If the burst bit is not set, only
 *              one data byte is communicated.
 *
 *              NOTE: This function is used in the following way:
 *
 *              TRXEM_SPI_BEGIN();
 *              while(TRXEM_PORT_IN & TRXEM_SPI_MISO_PIN);
 *              ...[Depending on type of register access]
 *              trxReadWriteBurstSingle(uint8 addr,uint8 *pData,uint16 len);
 *              TRXEM_SPI_END();
 *
 * input parameters
 *
 * @param       none
 *
 * output parameters
 *
 * @return      void
 */
static void trxReadWriteBurstSingle(uint8 addr,uint8 *pData,uint16 len)
{
	uint16 i;
	/* Communicate len number of bytes: if RX - the procedure sends 0x00 to push bytes from slave*/
  if(addr&RADIO_READ_ACCESS)
  {
    if(addr&RADIO_BURST_ACCESS)
    {
      for (i = 0; i < len; i++)
      {
          *pData = SPI.transfer(0);
          pData++;
      }
    }
    else
    {
      *pData = SPI.transfer(0);
    }
  }
  else
  {
    if(addr&RADIO_BURST_ACCESS)
    {
      /* Communicate len number of bytes: if TX - the procedure doesn't overwrite pData */
      for (i = 0; i < len; i++)
      {
        SPI.transfer(*pData);
        pData++;
      }
    }
    else
    {
      SPI.transfer(*pData);
    }
  }
  return;
}
