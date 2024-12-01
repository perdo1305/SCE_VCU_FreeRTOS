/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    voltage_measurement_task.c

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

#include "voltage_measurement_task.h"
#include "definitions.h"

#include "../src/config/default/peripheral/adchs/plib_adchs_common.h"
#include "../src/config/default/peripheral/adchs/plib_adchs.h" 

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
    This structure should be initialized by the VOLTAGE_MEASUREMENT_TASK_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

VOLTAGE_MEASUREMENT_TASK_DATA voltage_measurement_taskData;

xSemaphoreHandle voltageMeasurementSemaphore;

QueueHandle_t  voltageMeasurementQueue;

__COHERENT uint16_t voltageMeasurementValue;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/
void ADCHS_CH8_Callback(ADCHS_CHANNEL_NUM channel, uintptr_t context) {
    static BaseType_t xHigherPriorityTaskWoken;

    xHigherPriorityTaskWoken = pdFALSE;
    
    xSemaphoreGiveFromISR(voltageMeasurementSemaphore, &xHigherPriorityTaskWoken);
    if (xHigherPriorityTaskWoken == pdTRUE) {
        portYIELD();
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void VOLTAGE_MEASUREMENT_TASK_Initialize ( void )

  Remarks:
    See prototype in voltage_measurement_task.h.
 */

void VOLTAGE_MEASUREMENT_TASK_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    voltage_measurement_taskData.state = VOLTAGE_MEASUREMENT_TASK_STATE_INIT;

    ADCHS_CallbackRegister(ADCHS_CH8, ADCHS_CH8_Callback, (uintptr_t)NULL);  // Voltage Measurement
    ADCHS_ChannelResultInterruptEnable(ADCHS_CH8);
    ADCHS_ChannelConversionStart(ADCHS_CH8);

    vSemaphoreCreateBinary(voltageMeasurementSemaphore);
    xSemaphoreTake(voltageMeasurementSemaphore, 0);

    //creating a queue for the voltage measurement
    voltageMeasurementQueue = xQueueCreate(10, sizeof(uint16_t));


    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}


/******************************************************************************
  Function:
    void VOLTAGE_MEASUREMENT_TASK_Tasks ( void )

  Remarks:
    See prototype in voltage_measurement_task.h.
 */

void VOLTAGE_MEASUREMENT_TASK_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( voltage_measurement_taskData.state )
    {
        /* Application's initial state. */
        case VOLTAGE_MEASUREMENT_TASK_STATE_INIT:
        {
            bool appInitialized = true;


            if (appInitialized)
            {

                voltage_measurement_taskData.state = VOLTAGE_MEASUREMENT_TASK_STATE_SERVICE_TASKS;
            }
            break;
        }

        case VOLTAGE_MEASUREMENT_TASK_STATE_SERVICE_TASKS:
        {
          static uint16_t adcResult;
          
          xSemaphoreTake(voltageMeasurementSemaphore, portMAX_DELAY);
          adcResult = ADCHS_ChannelResultGet(ADCHS_CH8);

          voltageMeasurementValue = (uint16_t) ((adcResult * 3300) / 4095);
           
          xQueueSendToBack(voltageMeasurementQueue, &voltageMeasurementValue, 0);
          printf("Voltage Measurement: %d\n", voltageMeasurementValue);

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
