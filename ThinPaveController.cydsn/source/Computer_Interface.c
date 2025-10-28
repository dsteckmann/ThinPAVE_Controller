/**************************   ***************************************************************** ***************************/
//
// Title:             
// Company:     Instrotek
//
// Document No.:
// Issue:
// Description:  
//
// Filename:    Computer_Interface .c
// Author:      
//
// Processor:    
// Clock:	    z	
//
//
// History: date, version, personal initials with comments about any changes
//
/**********************************************************************************************************************/

/*********************************************  INCLUDE FILES  ***************************************************/
#include "project.h"

#include "Globals.h"
#include "DataStructs.h"
#include "conversions.h"
#include "UARTS.h"
#include "LCD_drivers.h"


/************************************* EXTERNAL VARIABLE AND BUFFER DECLARATIONS  *************************************/

/************************************************  LOCAL DEFINITIONS  *************************************************/

/*****************************************  VARIABLE AND BUFFER DECLARATIONS  *****************************************/
                        
 /******************************************************************************
 *  Name:
 *  
 *  PARAMETERS:None
 *
 *  DESCRIPTION: 
 *
 *  RETURNS: 
 *
 *****************************************************************************/ 
#define COMM_DELAY()   delay_ms(100) 

void CommDelay(){
    //while ( UART2_GetTxBufferSize() != 0 );    //RGS UART2_PutChar blocks so none of this is necessary, might be now, Dirk may have removed it.
    //COMM_DELAY();    //RGS for the display?
}
void UART2_DOUBLE_FLOAT(DOUBLE_FLOAT value, char* str){
    sprintf( lcdstr, str, value);
    UART2_PutString ( (const char*)lcdstr ) ; 
    COMM_DELAY();
}
void UART2_UINT16(uint16_t value, char* str){
    sprintf( lcdstr, str, value);
    UART2_PutString ( (const char*)lcdstr ) ; 
    COMM_DELAY();
}
void UART2_PrintThinLayerValues(){
    DOUBLE_FLOAT  value;
        // get the value from EEPROM (not ram)
    NV_EE_RD ( Constants.A1, &value ); 
    UART2_DOUBLE_FLOAT(value,"\r\nA1 = %.5Lf\n\r");
    NV_EE_RD ( Constants.B1, &value ); 
    UART2_DOUBLE_FLOAT(value,"B1 = %.5Lf\n\r");
    NV_EE_RD ( Constants.C1, &value ); 
    UART2_DOUBLE_FLOAT(value,"C1 = %.5Lf\n\r");
    NV_EE_RD ( Constants.A2, &value ); 
    UART2_DOUBLE_FLOAT(value,"A2 = %.5Lf\n\r");
    NV_EE_RD ( Constants.B2, &value ); 
    UART2_DOUBLE_FLOAT(value,"B2 = %.5Lf\n\r");
    NV_EE_RD ( Constants.C2, &value ); 
    UART2_DOUBLE_FLOAT(value,"C2 = %.5Lf\n\r");
    NV_EE_RD ( Constants.A11, &value ); 
    UART2_DOUBLE_FLOAT(value,"A11 = %.5Lf\n\r");
    NV_EE_RD ( Constants.A12, &value ); 
    UART2_DOUBLE_FLOAT(value,"A12 = %.5Lf\n\r");
    NV_EE_RD ( Constants.A13, &value ); 
    UART2_DOUBLE_FLOAT(value,"A13 = %.5Lf\n\r");
    NV_EE_RD ( Constants.A21, &value ); 
    UART2_DOUBLE_FLOAT(value,"A21 = %.5Lf\n\r");
    NV_EE_RD ( Constants.A22, &value ); 
    UART2_DOUBLE_FLOAT(value,"A22 = %.5Lf\n\r");
    NV_EE_RD ( Constants.A23, &value ); 
    UART2_DOUBLE_FLOAT(value,"A23 = %.5Lf\n\r");
    COMM_DELAY();
}
void UART2_PrintStds(){
    uint16_t std_value; 
    // get the value from EEPROM (not ram)
    NV_EE_RD (Constants.DENS_1_CAL_STD, &std_value );
    UART2_UINT16(std_value, "DEN 1 = %u\n\r");
    NV_EE_RD (Constants.DENS_2_CAL_STD, &std_value );     
    UART2_UINT16(std_value, "DEN 2 = %u\n\r");
    COMM_DELAY();

}
void UART2_PrintDate(){
    date_time_t cal_date;
    cal_date = NV_RAM_MEMBER_RD(Constants.CAL_DATE);
    UART2_UINT16(cal_date.iyear, "Year = %u\n\r");
    sprintf( lcdstr, "Month = %u\n\r", cal_date.imonth );
    UART2_PutString ( (const char*)lcdstr ) ; 
    sprintf( lcdstr, "Day = %u\n\r", cal_date.iday );
    UART2_PutString ( (const char*)lcdstr ) ; 
    COMM_DELAY();    
}
Bool WaitYN(){
    clearComputerRXBuffer();
    UART2_PutString ("Are these values correct? Y or N\n\n\r"); 
    COMM_DELAY();
    uint16 ch = waitc_RX(&s_Computer_RX, 600000);
    if ( ch == 'Y') { return TRUE; }
    return FALSE;
}
/////COMPUTER INTERFACE FUNCTIONS ////////////////////////////////////////////////////////
void get_const_computer(void) {  // receives constants from computer
  date_time_t cal_date;
  char space[] = ",";
  char cal_values[256]; 
  UART2_Start();
  isrUART2_Enable();
  UART2_EnableTxInt();
                                                   //end "Enter Moisture values E, F  and <enter>\n\r");  
  while(1) { // get thin layer values
    clearComputerRXBuffer();
    UART2_PutString ( "Enter Thin Layer Values A1, B1, C1 ,A2 ,B2, C2, A11, A12, A13, A21, A22, A23\r\n" ) ; 
    COMM_DELAY();
    gets_comp(cal_values); 
    
    NV_MEMBER_STORE(Constants.A1, atof(strtok(cal_values,space)));             
    NV_MEMBER_STORE(Constants.B1, atof(strtok(0,space)));
    NV_MEMBER_STORE(Constants.C1, atof(strtok(0,space)));
    NV_MEMBER_STORE(Constants.A2, atof(strtok(0,space)));
    NV_MEMBER_STORE(Constants.B2, atof(strtok(0,space)));
    NV_MEMBER_STORE(Constants.C2, atof(strtok(0,space)));
    NV_MEMBER_STORE(Constants.A11, atof(strtok(0,space)));
    NV_MEMBER_STORE(Constants.A12, atof(strtok(0,space)));
    NV_MEMBER_STORE(Constants.A13, atof(strtok(0,space)));
    NV_MEMBER_STORE(Constants.A21, atof(strtok(0,space)));
    NV_MEMBER_STORE(Constants.A22, atof(strtok(0,space)));
    NV_MEMBER_STORE(Constants.A23, atof(strtok(0,space)));
    UART2_PrintThinLayerValues();
    if(WaitYN() == TRUE) { break; }
  }                                                    //end get thin layer values
    while(1) { // get standard counts
        clearComputerRXBuffer();
        UART2_PutString ( "Enter Standard Counts Values DEN1 and DEN2 <enter>\n\r" ) ;
        gets_comp(cal_values);
        NV_MEMBER_STORE(Constants.DENS_1_CAL_STD, atoi(strtok(cal_values,space)));   // Store Density 1 std
        NV_MEMBER_STORE(Constants.DENS_2_CAL_STD, atoi(strtok(0,space)));            // Store Density 2 s    UART2_PrintStds();
        UART2_PrintStds();
        if ( WaitYN() == TRUE) { break;  }
    }                                                    //end get standard counts
  while(1) { // get cal date
    clearComputerRXBuffer();
    UART2_PutString ( "Enter Cal. Year, Month, Day and <enter>\n\r" ) ; 
    COMM_DELAY();
    gets_comp(cal_values);
    cal_date.iyear  = atoi(strtok(cal_values,space)); // set the date and time in the date struct
    cal_date.imonth = atoi(strtok(0,space));
    cal_date.iday   = atoi(strtok(0,space));
    cal_date.isecond = 0;
    cal_date.ihour   = 12;
    cal_date.iminute = 0;
    //write the date/time to memory
    NV_MEMBER_STORE(Constants.CAL_DATE, cal_date);
    UART2_PrintDate();    
    if(WaitYN() == TRUE) { break; }
  }                                                    //end get cal date
  
    UART2_PrintThinLayerValues();
    UART2_PrintStds();
    UART2_PrintDate();
 
}
 /******************************************************************************
 *  Name:
 *  
 *  PARAMETERS:None
 *
 *  DESCRIPTION: 
 *
 *  RETURNS: 
 *
 *****************************************************************************/ 
void wait_for_serial(void) { // if computer is connected, wait for input from the computer
    char command;
    initUARTS();      // moved to main.c to support the Plus depth sensor comms.
    UART2_Start();
    isrUART2_Enable();
    clearComputerRXBuffer();
    while ( IS_ACTICE_RS232_DETECTED()  )
    {
        UART2_PutString ( "Enter 'L'(load cal data), 'A'(access serial #) or 'E'(exit)\n\r" ) ; 
        //while ( UART2_GetTxBufferSize() != 0 );  // not necessary
        delay_ms(100);
        while ( ( getRXNumEntries ( &s_Computer_RX) == 0 ) && ( IS_ACTICE_RS232_DETECTED()  ) )
        {
            delay_us(10);
        }
        delay_ms (1);
        if ( getRXNumEntries(&s_Computer_RX) > 0 )  
        { 
            command =  getcRXBuffer(&s_Computer_RX);
            if(command == 'L')                       // load constants Ai, Bi, Ci,...E, F
            {
              get_const_computer();        
            }    
            else if (command == 'A')               //   access serial #
            {
              serial_number(true);     
            }    
            else if (command == 'E')               //   exit            
            {
                break;            
            }    
        } 
    }
   
}
 /******************************************************************************
 *  Name:
 *  
 *  PARAMETERS:None
 *
 *  DESCRIPTION: 
 *
 *  RETURNS: 
 *
 *****************************************************************************/ 
void gets_comp(char* s) { // reads a string from UART2 XMIT = PIN_F5, RCV = PIN_F4 
  #define MAX_RX_TIME_ERROR   0x1122
  uint16_t len = 0, c;
  do
  {
    c = waitc_RX ( &s_Computer_RX , 1000 );
    if ( c != MAX_RX_TIME_ERROR )
    {
     if(c>=' ' && c<='~')
     {
       s[len++]=c;
     }  
    }
  } while(c!=13);
  
  s[len]='\0';
}
 /******************************************************************************
 *  Name:
 *  
 *  PARAMETERS:None
 *
 *  DESCRIPTION: 
 *
 *  RETURNS: 
 *
 *****************************************************************************/ 
void puts_printer ( char *s ) {  //print the string to attached printer
  uint8_t c = 0;  
  char data_byte ;
  CLEAR_BUFFER2;
  
  ok_2_print = TRUE;
  
  do
  {
    if ( ok_2_print )
    {
      data_byte  = *(s + c++);   
      UART2_PutChar( data_byte );  //send character to printer up to and including CR
      while ( UART2_GetTxBufferSize() != 0 )   
      {
        CyDelayUs ( 100 );
      }  
    }    
   else
   {
      delay_ms(10);
   }    
  
  }while ( data_byte != '\r' );      
 
 
}
 /******************************************************************************
 *  Name:
 *  
 *  PARAMETERS:None
 *
 *  DESCRIPTION: 
 *
 *  RETURNS: 
 *
 *****************************************************************************/ 
void hold_buzzer(void) {
  while ( BUZZER_Read() ) { delay_us(10); }  
}
