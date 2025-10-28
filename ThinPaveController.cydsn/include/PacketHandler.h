/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#ifndef PACKETHANDLER_H
  
#define PACKETHANDLER_H

#include <project.h>
#include "stdbool.h"

#define BEGIN_FLAG_0     0xAA
#define BEGIN_FLAG_1     0x55 
#define CMD_REMOTE_START 0x53  // Start a measurement from the app          processPacketStream - input g_remote_start_flag=TRUE;  NO RESPONSE
#define CMD_REMOTE_RECALL 0x52 //                                           processPacketStream - input g_remote_recall_flag=TRUE; NO RESPONSE MIGHT BE IN RCALL
#define CMD_FLAG_READ    0x33  // new reading station data                  SendBLEData         - output  measureDensity(void) AFTER COUNT main_menu(void)-UNREACHABLE
#define CMD_SEND_CC      0x79  // send calibration constants                     SendBLEDataCC       - output  main_menu(void)-UNREACHABLE
#define ACK_FLAG_CC      0x78  // ack calibration constants                     SendBleCcAck        - output  NOT USED - MAY NEED TO BE, CALPLUS NEEDS THEM MIGHT COME ELSEWHERE
#define CMD_SET_CC       0x77  // setting CC in eeprom                      processPacketStream - input
#define CMD_FLAG_SN      0x88  // serial number                             SendBleSn           - output serial_number(Bool source) main_menu(void)-UNREACHABLE
#define CMD_FLAG_VER     0x89  // firmware version                          RequestBleVersion   - output NOT USED
#define CMD_SET_SN       0x90  // pc setting sn                             NOT USED - THIS MUST BE IMPLEMENTED - CALPLUS MUST BE ABLE TO SET IT PERHAPS IT COMES ELSEWHERE
#define CMD_FLAG_RESET   0x99  // software reset                            void SendBleReset(void) - NOT USED
#define DELIMITER_FLAG   '~'   // for receiving data from the ble module
#define END_FLAG         '\n'  // for receiving data from the ble module

  
extern bool PacketStarted;    // Set to true if a packet is being parsed
extern bool PacketComplete;  // Set to true when the whole packet is received
extern bool PacketProcessed;  // Goes true once we process the packet command or data

void checkDataStream ( uint8 ch );

#endif

/* [] END OF FILE */
