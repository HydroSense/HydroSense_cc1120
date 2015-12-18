#include "components/common/hal_rf.h"

#include "rfsettings.h"

void configureRadio()
{

// 915.000 MHz
// 1.2 ksps, 1.2 kbps
// 2-FSK, 32 MHz xtal, 25 kHz filter BW, PA ramping, 27 dBM TX power
// Rf settings for CC1120
//
halRfWriteReg(CC112X_IOCFG3,0xB0);        //GPIO3 IO Pin Configuration
halRfWriteReg(CC112X_IOCFG2,0x06);        //GPIO2 IO Pin Configuration
halRfWriteReg(CC112X_IOCFG1,0xB0);        //GPIO1 IO Pin Configuration
halRfWriteReg(CC112X_IOCFG0,0x40);        //GPIO0 IO Pin Configuration
halRfWriteReg(CC112X_SYNC_CFG1,0x0B);     //Sync Word Detection Configuration Reg. 1
halRfWriteReg(CC112X_DCFILT_CFG,0x1C);    //Digital DC Removal Configuration
halRfWriteReg(CC112X_PREAMBLE_CFG1,0x18); //Preamble Length Configuration Reg. 1
halRfWriteReg(CC112X_IQIC,0xC6);          //Digital Image Channel Compensation Configuration
halRfWriteReg(CC112X_CHAN_BW,0x08);       //Channel Filter Configuration
halRfWriteReg(CC112X_MDMCFG0,0x05);       //General Modem Parameter Configuration Reg. 0
halRfWriteReg(CC112X_AGC_REF,0x20);       //AGC Reference Level Configuration
halRfWriteReg(CC112X_AGC_CS_THR,0x19);    //Carrier Sense Threshold Configuration
halRfWriteReg(CC112X_AGC_CFG1,0xA9);      //Automatic Gain Control Configuration Reg. 1
halRfWriteReg(CC112X_AGC_CFG0,0xCF);      //Automatic Gain Control Configuration Reg. 0
halRfWriteReg(CC112X_FIFO_CFG,0x00);      //FIFO Configuration
halRfWriteReg(CC112X_SETTLING_CFG,0x03);  //Frequency Synthesizer Calibration and Settling Con..
halRfWriteReg(CC112X_FS_CFG,0x12);        //Frequency Synthesizer Configuration
halRfWriteReg(CC112X_PKT_CFG0,0x20);      //Packet Configuration Reg. 0
halRfWriteReg(CC112X_PA_CFG2,0x77);       //Power Amplifier Configuration Reg. 2
halRfWriteReg(CC112X_PKT_LEN,0xFF);       //Packet Length Configuration
halRfWriteReg(CC112X_IF_MIX_CFG,0x00);    //IF Mix Configuration
halRfWriteReg(CC112X_FREQOFF_CFG,0x22);   //Frequency Offset Correction Configuration
halRfWriteReg(CC112X_FREQ2,0x72);         //Frequency Configuration [23:16]
halRfWriteReg(CC112X_FREQ1,0x60);         //Frequency Configuration [15:8]
halRfWriteReg(CC112X_FS_DIG1,0x00);       //Frequency Synthesizer Digital Reg. 1
halRfWriteReg(CC112X_FS_DIG0,0x5F);       //Frequency Synthesizer Digital Reg. 0
halRfWriteReg(CC112X_FS_CAL1,0x40);       //Frequency Synthesizer Calibration Reg. 1
halRfWriteReg(CC112X_FS_CAL0,0x0E);       //Frequency Synthesizer Calibration Reg. 0
halRfWriteReg(CC112X_FS_DIVTWO,0x03);     //Frequency Synthesizer Divide by 2
halRfWriteReg(CC112X_FS_DSM0,0x33);       //FS Digital Synthesizer Module Configuration Reg. 0
halRfWriteReg(CC112X_FS_DVC0,0x17);       //Frequency Synthesizer Divider Chain Configuration ..
halRfWriteReg(CC112X_FS_PFD,0x50);        //Frequency Synthesizer Phase Frequency Detector Con..
halRfWriteReg(CC112X_FS_PRE,0x6E);        //Frequency Synthesizer Prescaler Configuration
halRfWriteReg(CC112X_FS_REG_DIV_CML,0x14);//Frequency Synthesizer Divider Regulator Configurat..
halRfWriteReg(CC112X_FS_SPARE,0xAC);      //Frequency Synthesizer Spare
halRfWriteReg(CC112X_FS_VCO0,0xB4);       //FS Voltage Controlled Oscillator Configuration Reg..
halRfWriteReg(CC112X_XOSC5,0x0E);         //Crystal Oscillator Configuration Reg. 5
halRfWriteReg(CC112X_XOSC1,0x03);         //Crystal Oscillator Configuration Reg. 1
halRfWriteReg(CC112X_PARTNUMBER,0x48);    //Part Number
halRfWriteReg(CC112X_PARTVERSION,0x21);   //Part Revision
halRfWriteReg(CC112X_MODEM_STATUS1,0x10); //Modem Status Reg. 1


}
