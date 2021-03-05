/*!*********************************************************************************************************************
@file user_app.c                                                                
@brief User's tasks / applications are written here.  This description
should be replaced by something specific to the task.

------------------------------------------------------------------------------------------------------------------------
GLOBALS
- NONE

CONSTANTS
- NONE

TYPES
- NONE

PUBLIC FUNCTIONS
- NONE

PROTECTED FUNCTIONS
- void UserApp1Initialize(void)
- void UserApp1Run(void)


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_<type>UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u8 G_u8UserAppFlags;                             /*!< @brief Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemTime1ms;                   /*!< @brief From main.c */
extern volatile u32 G_u32SystemTime1s;                    /*!< @brief From main.c */
extern volatile u32 G_u32SystemFlags;                     /*!< @brief From main.c */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp_<type>" and be declared as static.
***********************************************************************************************************************/


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @protectedsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*!--------------------------------------------------------------------------------------------------------------------
@fn void UserAppInitialize(void)

@brief
Initializes the application's variables.

Should only be called once in main init section.

Requires:
- NONE

Promises:
- NONE

*/
void UserAppInitialize(void)
{
    /* LED initialization */
    LATA = 0x80;
    
    /* Timer0 control register initialization to turn timer on, asynchronous mode, 16 bit
     * Fosc/4, 1:16 pre-scaler, 1:1post-scaler */
    T0CON0 = 0x90;
    T0CON1 = 0x54;

} /* end UserAppInitialize() */

/*!----------------------------------------------------------------------------------------------------------------------
 fn void TimeXus(u16 Input)
 Timer0 counts to Input
 
 Requires:
 - Each tick must be configured to 1 us
 - u16 Input is the value in microseconds timing from 1 us to 65535 us.
 
 Promises:
 - Pre-load TMR0H:L to clock out period
 - TMRI0F cleared
 - Timer0 enabled
 */
void TimeXus(u16 Input)
{
    //Disable the timer
    T0CON0 &= 0x7F;
    
    
    u16 Timer = 0xFFFF - Input;
    
    /* Pre-loading TMR0L and TMR0H based on Input*/
    TMR0H = (u8)((Timer & 0xFF00) >> 8); //Shifts the bitmasked values to LSB;
    TMR0L = (u8)((Timer) & 0x00FF); //Bitmasks the 8 LSB;
    
    
    PIR3 &= 0x7F; //Clears TMRI0F
    
    T0CON0 |= 0x80; //Repeats timer by turning on again
} /* Exit TimeXus */


/*!----------------------------------------------------------------------------------------------------------------------
@fn void UserAppRun(void)

@brief Application code that runs once per system loop

Requires:
- 

Promises:
- 

*/
void UserAppRun(void)
{
    static u16 u16Delay = 0x0000;
    
    /*To access the indices in our Pattern array*/
    static int intLedIndex = 0;
    
    u8 au8Pattern[6] = {0x01,0x02,0x04,0x08,0x10,0x20};
    
    /*Counting 1ms every tick*/
    u16Delay++; 
    
    /*One u16Delay reaches 500 (or 0x01F4), pattern moves to next phase*/
    if(u16Delay == 500)
    {
        /*Reset Delay*/
        u16Delay = 0x0000;
        u8 u8TempVar = LATA;
        
        /*Bit Mask to clear 6 LSB's*/
        u8TempVar &= 0x80;
        
        
        /*Display Next Pattern*/
        u8TempVar = 0x00 | au8Pattern[intLedIndex];
        LATA = u8TempVar;
        intLedIndex++;
        
        
        /*Reset Pattern onces Pattern reaches end*/
        if(intLedIndex == 6)
        {
            intLedIndex = 0;
        }
    }
    
} /* end UserAppRun */



/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/





/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
