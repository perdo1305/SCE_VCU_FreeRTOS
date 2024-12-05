/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    r2d_task.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "r2d_task.h"
#include "definitions.h"
// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the R2D_TASK_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

R2D_TASK_DATA r2d_taskData;

typedef struct {
    bool isR2D;
    bool R2D_last_state;
    bool R2DS_as_played;
} R2D_t;
R2D_t R2D;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/
void SOUND_R2DS(void);

void SOUND_R2DS(void) {
    if (R2D.isR2D) {
        if (!BUZZER_ON) {
            if (!R2D.R2DS_as_played) {
                TMR5_Start();
                GPIO_RF0_pin_Clear();
                R2D.R2DS_as_played = true;
            }
        }
    }
}
// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void R2D_TASK_Initialize ( void )

  Remarks:
    See prototype in r2d_task.h.
 */

void R2D_TASK_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    r2d_taskData.state = R2D_TASK_STATE_INIT;

    

    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}


/******************************************************************************
  Function:
    void R2D_TASK_Tasks ( void )

  Remarks:
    See prototype in r2d_task.h.
 */

void R2D_TASK_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( r2d_taskData.state )
    {
        /* Application's initial state. */
        case R2D_TASK_STATE_INIT:
        {
            bool appInitialized = true;


            if (appInitialized)
            {

                r2d_taskData.state = R2D_TASK_STATE_SERVICE_TASKS;
            }
            break;
        }

        case R2D_TASK_STATE_SERVICE_TASKS:
        {

            break;
        }

        /* TODO: implement your application state machine.*/


        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}


/*******************************************************************************
 End of File
 */
