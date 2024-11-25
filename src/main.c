/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes
#include <stdio.h>
//#include <FreeRTOS.h>
//#include "../../u8g2-master/csrc/u8g2.h"



// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

SemaphoreHandle_t ADC0_SEMAPHORE;
SemaphoreHandle_t ADC3_SEMAPHORE;

// ############# ADC ########################################
uint8_t message_ADC[64];      // CAN message to send ADC data
__COHERENT uint16_t ADC[64];  // ADC raw data
uint16_t ADC_Filtered_0 = 0;
uint16_t ADC_Filtered_3 = 0;

__COHERENT uint16_t adc[10][10];  // just to test dma
bool adc_flag[64];                // flag to indicate that the ADC data was updated

void ADCHS_CH0_Callback(ADCHS_CHANNEL_NUM channel, uintptr_t context) {
    ADC[channel] = ADCHS_ChannelResultGet(channel);
    adc_flag[channel] = 1;
}

void ADCHS_CH3_Callback(ADCHS_CHANNEL_NUM channel, uintptr_t context) {
    ADC[channel] = ADCHS_ChannelResultGet(channel);
    adc_flag[channel] = 1;
}

int main(void) {
    /* Initialize all modules */
    SYS_Initialize(NULL);
    
    ADCHS_CallbackRegister(ADCHS_CH0, ADCHS_CH0_Callback, (uintptr_t)NULL);  // APPS1
    ADCHS_CallbackRegister(ADCHS_CH3, ADCHS_CH3_Callback, (uintptr_t)NULL);  // APPS2

    vSemaphoreCreateBinary(ADC0_SEMAPHORE);
    vSemaphoreCreateBinary(ADC3_SEMAPHORE);


    while (true) {
        SYS_Tasks();
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE);
}


/*******************************************************************************
 End of File
 */

