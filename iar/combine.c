/*!
 *****************************************************************************
   @file:    hello_world.c

   @brief:   Data Exchange Hello World No Operating System Example

   @details: Example program demonstrating basic use of the Data Exchange
             profile to send a string from the EM9304 to a host.
  -----------------------------------------------------------------------------

Copyright (c) 2016 Analog Devices, Inc.

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
  - Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  - Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
  - Modified versions of the software must be conspicuously marked as such.
  - This software is licensed solely and exclusively for use with processors
    manufactured by or for Analog Devices, Inc.
  - This software may not be combined or merged with other code in any manner
    that would cause the software to become subject to terms and conditions
    which differ from those listed here.
  - Neither the name of Analog Devices, Inc. nor the names of its
    contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.
  - The use of this software may or may not infringe the patent rights of one
    or more patent holders.  This license does not release you from the
    requirement that you obtain separate licenses from these patent holders
    to use this software.

THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF CLAIMS OF INTELLECTUAL
PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*****************************************************************************/
/************************************************************************************ADC FUNCTIONS*/
#include "stdio.h"
/* ADC example include */
#include "adc_channel_read.h"
/* Managed drivers and/or services include */
#include <adi_adc.h>
#include "common.h"
#include "drivers/pwr/adi_pwr.h"
#include <assert.h>
#include <stdio.h>
#include <drivers/general/adi_drivers_general.h>
/*=============  D A T A  =============*/

/* ADC Handle */
ADI_ALIGNED_PRAGMA(4)
ADI_ADC_HANDLE hDevice ADI_ALIGNED_ATTRIBUTE(4);

/* Memory Required for adc driver */
ADI_ALIGNED_PRAGMA(4)
static uint8_t DeviceMemory[ADI_ADC_MEMORY_SIZE] ADI_ALIGNED_ATTRIBUTE(4);

/* ADC Data Buffer */
ADI_ALIGNED_PRAGMA(4)
static uint16_t ADC_DataBuffer[ADC_NUM_SAMPLES] ADI_ALIGNED_ATTRIBUTE(4) = {0};

/*=============  E X T E R N A L    F U N C T I O N S  =============*/
extern int32_t adi_initpinmux(void);


/*=============  L O C A L    F U N C T I O N S  =============*/

static void usleep(uint32_t usec);
static void WriteSamplesToFile (void);
/************************************************************************************ADC FUNCTIONS*/

#include "hello_world.h"


extern int32_t adi_initpinmux(void);


static void ApplicationCallback(void * pCBParam, uint32_t Event, void * pArg);
static void SetAdvertisingMode (void);


//static const char         sGoodbyeWorld[] = "Hijo";
static ADI_DATA_PACKET    eDataPacket;
static bool               gbConnected;
static ADI_BLER_CONN_INFO sConnInfo;
static ADI_BLE_GAP_MODE   geMode;


#define PERIPHERAL_ADV_MODE      ((ADI_BLE_GAP_MODE)(ADI_BLE_GAP_MODE_CONNECTABLE | ADI_BLE_GAP_MODE_DISCOVERABLE))


/*!
 * @brief      Main
 *
 * @details    Application entry point.
 *
 * @param [in] argc : Number of arguments (unused)
 *
 * @param [in] argv : Arguments (unused)
 *             
 */
// C program for implementation of ftoa()
#include<stdio.h>
#include<math.h>
 
// reverses a string 'str' of length 'len'
void reverse(char *str, int len)
{
    int i=0, j=len-1, temp;
    while (i<j)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++; j--;
    }
}
 
 // Converts a given integer x to string str[].  d is the number
 // of digits required in output. If d is more than the number
 // of digits in x, then 0s are added at the beginning.
int intToStr(int x, char str[], int d)
{
    int i = 0;
    while (x)
    {
        str[i++] = (x%10) + '0';
        x = x/10;
    }
 
    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';
 
    reverse(str, i);
    str[i] = '\0';
    return i;
}
 
// Converts a floating point number to string.
void ftoa(float n, char *res, int afterpoint)
{
    // Extract integer part
    int ipart = (int)n;
 
    // Extract floating part
    float fpart = n - (float)ipart;
 
    // convert integer part to string
    int i = intToStr(ipart, res, 0);
 
    // check for display option after point
    if (afterpoint != 0)
    {
        res[i] = '.';  // add dot
 
        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter is needed
        // to handle cases like 233.007
        fpart = fpart * pow(10, afterpoint);
 
        intToStr((int)fpart, res + i + 1, afterpoint);
    }
}
void ADC_SampleData(void)
{
    ADI_ADC_RESULT  eResult = ADI_ADC_SUCCESS;
    ADI_ADC_BUFFER Buffer;
    bool bCalibrationDone = false;

    /* Open the ADC device */
    eResult = adi_adc_Open(ADC_DEV_NUM, DeviceMemory, sizeof(DeviceMemory), &hDevice);
    DEBUG_RESULT("Failed to open ADC device",eResult, ADI_ADC_SUCCESS);

    /* Power up ADC */
    eResult = adi_adc_PowerUp (hDevice, true);
    DEBUG_RESULT("Failed to power up ADC", eResult, ADI_ADC_SUCCESS);

    /* Set ADC reference */
    eResult = adi_adc_SetVrefSource (hDevice, ADI_ADC_VREF_SRC_INT_2_50_V);
    DEBUG_RESULT("Failed to set ADC reference", eResult, ADI_ADC_SUCCESS);

    /* Enable ADC sub system */
    eResult = adi_adc_EnableADCSubSystem (hDevice, true);
    DEBUG_RESULT("Failed to enable ADC sub system", eResult, ADI_ADC_SUCCESS);

    /* Wait for 5.0ms */
     usleep (5000);

    /* Start calibration */
    eResult = adi_adc_StartCalibration (hDevice);
    DEBUG_RESULT("Failed to start calibration", eResult, ADI_ADC_SUCCESS);

    /* Wait until calibration is done */
    while (!bCalibrationDone)
    {
        eResult = adi_adc_IsCalibrationDone (hDevice, &bCalibrationDone);
        DEBUG_RESULT("Failed to get the calibration status", eResult, ADI_ADC_SUCCESS);
    }

    /* Set the delay time */
    eResult = adi_adc_SetDelayTime ( hDevice, 10);
    DEBUG_RESULT("Failed to set the Delay time ", eResult, ADI_ADC_SUCCESS);

    /* Set the acquisition time. (Application need to change it based on the impedence) */
    eResult = adi_adc_SetAcquisitionTime ( hDevice, 10000000);
    DEBUG_RESULT("Failed to set the acquisition time ", eResult, ADI_ADC_SUCCESS);

    /* Populate the buffer structure */
    Buffer.nBuffSize = sizeof(ADC_DataBuffer);
    Buffer.nChannels = ADI_ADC_CHANNEL_0;
    Buffer.nNumConversionPasses = ADC_NUM_SAMPLES;
    Buffer.pDataBuffer = ADC_DataBuffer;

    /* Submit the buffer to the driver */
    eResult = adi_adc_SubmitBuffer (hDevice, &Buffer);
    DEBUG_RESULT("Failed to submit buffer ", eResult, ADI_ADC_SUCCESS);

    /* Enable the ADC */
    eResult = adi_adc_Enable (hDevice, true);
    DEBUG_RESULT("Failed to enable the ADC for sampling ", eResult, ADI_ADC_SUCCESS);

    ADI_ADC_BUFFER* pAdcBuffer;
    eResult = adi_adc_GetBuffer (hDevice, &pAdcBuffer);
    DEBUG_RESULT("Failed to Get Buffer ", eResult, ADI_ADC_SUCCESS);

    /* Disable the ADC */
    eResult = adi_adc_Enable (hDevice, false);
    DEBUG_RESULT("Failed to disable ADC for sampling ", eResult, ADI_ADC_SUCCESS);

    /* Close the ADC */
    eResult = adi_adc_Close (hDevice);
    DEBUG_RESULT("Failed to close ADC", eResult, ADI_ADC_SUCCESS);
}

/* Write the samples to file */
static void WriteSamplesToFile (void)
{
#ifdef WRITE_SAMPLES_TO_FILE
    int x;
    FILE* fp;

    fp = fopen("./ADC_Out.dat", "w");
    if (fp == NULL) {
        common_Fail("Failed open file\n");
        return;
    }
    for (x = 0; x < ADC_NUM_SAMPLES; x++) {
        fprintf(fp,"%d\n", ADC_DataBuffer[x]);
    }

    fclose(fp);
#endif /* WRITE_SAMPLES_TO_FILE */
}

/* Approximately wait for minimum 1 usec */
static void usleep(uint32_t usec)
{
    volatile int y = 0;
    while (y++ < usec) {
        volatile int x = 0;
        while (x < 16) {
        x++;
        }
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
  adi_initpinmux();
  ADC_SampleData();
  
    char sHelloWorld[8];
    char sGoodbyeWorld[8];
    float n = 233.007;
    ftoa(n, sHelloWorld, 4);
    n = 3.14151;
    ftoa(n, sGoodbyeWorld, 4);
    
  
    ADI_BLER_RESULT eResult;
    ADI_PWR_RESULT  ePwr;
    uint8_t         nStringCounter = 0u;
    uint8_t *       aDataExchangeName = (unsigned char *) "ADI_BLE_HELLOWORLD";

    /* Explicitly disable the watchdog timer */ 
    *pREG_WDT0_CTL = 0x0u;

    /* Pinmux */
    //adi_initpinmux();

    /* Initialize clocks */
    ePwr = adi_pwr_Init(); 
    DEBUG_RESULT("Error initializing the power service.\r\n", ePwr, ADI_PWR_SUCCESS);

    ePwr = adi_pwr_SetClockDivider(ADI_CLOCK_HCLK, 1u);     
    DEBUG_RESULT("Error configuring the core clock.\r\n", ePwr, ADI_PWR_SUCCESS);

    ePwr = adi_pwr_SetClockDivider(ADI_CLOCK_PCLK, 1u);
    DEBUG_RESULT("Error configuring the peripheral clock.\r\n", ePwr, ADI_PWR_SUCCESS);

    //common_Init();

    DEBUG_MESSAGE("Starting DataExchange HelloWorld Example.\r\n");

    /* Initialize radio and framework layer */
    eResult = adi_ble_Init(ApplicationCallback, NULL);
    DEBUG_RESULT("Error initializing the radio.\r\n", eResult, ADI_BLER_SUCCESS);
    
    /* Configure radio */
    eResult = adi_radio_RegisterDevice(ADI_BLE_ROLE_PERIPHERAL);
    DEBUG_RESULT("Error registering the radio.\r\n", eResult, ADI_BLER_SUCCESS);
    
    eResult = adi_radio_SetLocalBluetoothDevName(aDataExchangeName, strlen((const char *) aDataExchangeName), 0u, 0u);
    DEBUG_RESULT("Error setting local device name.\r\n", eResult, ADI_BLER_SUCCESS);

    SetAdvertisingMode();        
    
    /* Initialize data exchange profile */
    eResult = adi_radio_Register_DataExchangeServer();
    DEBUG_RESULT("Error registering data exchange server.\r\n", eResult, ADI_BLER_SUCCESS);
    
    /* Initialize the data packet header and sensor type as these will not change throughout the example*/
    eDataPacket.nPacketHeader = ADI_SET_HEADER(ADI_SENSOR_PACKET, SENSOR_ID);
    eDataPacket.eSensorType = ADI_PRINTSTRING_TYPE;

    /* Now enter infinite loop waiting for connection and then data exchange events */
    DEBUG_MESSAGE("Waiting for connection. Initiate connection on central device please.\r\n");

    /* WHILE(forever) */
    while(1u)
    {
      //START SEQUENCE//////////////////////////////////////////////////////
        n = 5151;
        ftoa(n, sHelloWorld, 4);
        /* Dispatch events for two seconds - they will arrive in the application callback */
        eResult = adi_ble_DispatchEvents(200u);
        DEBUG_RESULT("Error dispatching events to the callback.\r\n", eResult, ADI_BLER_SUCCESS);

        /* If connected, send data */
        if (gbConnected == true)
        {
            adi_ble_GetConnectionInfo(&sConnInfo);

            //if (nStringCounter == 0u)
            //{
                ASSERT(strlen(sHelloWorld) <= ADI_MAX_STRING_SIZE);

                /* Set the size of the string */
                eDataPacket.eStringData.nStringSize = strlen(sHelloWorld);
                /* Set the string */
                memcpy(eDataPacket.eStringData.aStringData, sHelloWorld, strlen(sHelloWorld));
                nStringCounter = 1u;
            //}
            //else
            //{
            //	ASSERT(strlen(sGoodbyeWorld) <= ADI_MAX_STRING_SIZE);

            //	/* Set the size of the string */
            //	eDataPacket.eStringData.nStringSize = strlen(sGoodbyeWorld);
            	/* Set the string */
            //    memcpy(eDataPacket.eStringData.aStringData, sGoodbyeWorld, strlen(sGoodbyeWorld));
            //    nStringCounter = 0u;
            //}
            
            eResult = adi_radio_DE_SendData(sConnInfo.nConnHandle, DATAEXCHANGE_PACKET_SIZE, (uint8_t *) &eDataPacket);
            if (eResult != ADI_BLER_SUCCESS) {
                DEBUG_MESSAGE("Error sending the data.\r\n");
            }

        }
        /* If disconnected, make sure we are in the right mode */
        else
        {
            if (geMode != PERIPHERAL_ADV_MODE)
            {
                SetAdvertisingMode();           
            }
        }
      //////////////////////////////////////////////////////////////////////
      for(int i = 0 ; i < 100 ; i++){
        n = ADC_DataBuffer[i];
        //n = 10;
        ftoa(n, sHelloWorld, 4);
        /* Dispatch events for two seconds - they will arrive in the application callback */
        eResult = adi_ble_DispatchEvents(200u);
        DEBUG_RESULT("Error dispatching events to the callback.\r\n", eResult, ADI_BLER_SUCCESS);

        /* If connected, send data */
        if (gbConnected == true)
        {
            adi_ble_GetConnectionInfo(&sConnInfo);

            //if (nStringCounter == 0u)
            //{
                ASSERT(strlen(sHelloWorld) <= ADI_MAX_STRING_SIZE);

                /* Set the size of the string */
                eDataPacket.eStringData.nStringSize = strlen(sHelloWorld);
                /* Set the string */
                memcpy(eDataPacket.eStringData.aStringData, sHelloWorld, strlen(sHelloWorld));
                nStringCounter = 1u;
            //}
            //else
            //{
            //	ASSERT(strlen(sGoodbyeWorld) <= ADI_MAX_STRING_SIZE);

            //	/* Set the size of the string */
            //	eDataPacket.eStringData.nStringSize = strlen(sGoodbyeWorld);
            	/* Set the string */
            //    memcpy(eDataPacket.eStringData.aStringData, sGoodbyeWorld, strlen(sGoodbyeWorld));
            //    nStringCounter = 0u;
            //}
            
            eResult = adi_radio_DE_SendData(sConnInfo.nConnHandle, DATAEXCHANGE_PACKET_SIZE, (uint8_t *) &eDataPacket);
            if (eResult != ADI_BLER_SUCCESS) {
                DEBUG_MESSAGE("Error sending the data.\r\n");
            }

        }
        /* If disconnected, make sure we are in the right mode */
        else
        {
            if (geMode != PERIPHERAL_ADV_MODE)
            {
                SetAdvertisingMode();           
            }
        }
      }
      
      //STOP SEQUENCE//////////////////////////////////////////////////////
        n = 6565;
        ftoa(n, sHelloWorld, 4);
        /* Dispatch events for two seconds - they will arrive in the application callback */
        eResult = adi_ble_DispatchEvents(200u);
        DEBUG_RESULT("Error dispatching events to the callback.\r\n", eResult, ADI_BLER_SUCCESS);

        /* If connected, send data */
        if (gbConnected == true)
        {
            adi_ble_GetConnectionInfo(&sConnInfo);

            //if (nStringCounter == 0u)
            //{
                ASSERT(strlen(sHelloWorld) <= ADI_MAX_STRING_SIZE);

                /* Set the size of the string */
                eDataPacket.eStringData.nStringSize = strlen(sHelloWorld);
                /* Set the string */
                memcpy(eDataPacket.eStringData.aStringData, sHelloWorld, strlen(sHelloWorld));
                nStringCounter = 1u;
            //}
            //else
            //{
            //	ASSERT(strlen(sGoodbyeWorld) <= ADI_MAX_STRING_SIZE);

            //	/* Set the size of the string */
            //	eDataPacket.eStringData.nStringSize = strlen(sGoodbyeWorld);
            	/* Set the string */
            //    memcpy(eDataPacket.eStringData.aStringData, sGoodbyeWorld, strlen(sGoodbyeWorld));
            //    nStringCounter = 0u;
            //}
            
            eResult = adi_radio_DE_SendData(sConnInfo.nConnHandle, DATAEXCHANGE_PACKET_SIZE, (uint8_t *) &eDataPacket);
            if (eResult != ADI_BLER_SUCCESS) {
                DEBUG_MESSAGE("Error sending the data.\r\n");
            }

        }
        /* If disconnected, make sure we are in the right mode */
        else
        {
            if (geMode != PERIPHERAL_ADV_MODE)
            {
                SetAdvertisingMode();           
            }
        }
      //////////////////////////////////////////////////////////////////////
        
    } /* ENDWHILE */
}


/*!
 * @brief      Application Callback
 *
 * @details    Called by the framework layer (adi_ble_noos.c) when an event occurs that the application did NOT
 *             explicity expect by calling #adi_ble_WaitForEventWithTimeout.
 *
 * @param [in] pCBParam : Callback parameter (unused)
 *
 * @param [in] Event : Event of type #ADI_BLER_EVENT.
 *
 * @param [in] pArg : Callback argument (unused)
 * 
 * @note       The application should NOT call other radio functions (adi_ble_radio.c) from this callback that issue 
 *             a command to the radio. The application may call radio functions that simply extract data from the 
 *             companion module, these are located below #adi_ble_GetEvent in adi_ble_radio.c. Ideally this callback 
 *             should just be used for flags and semaphores.
 */
static void ApplicationCallback(void * pCBParam, uint32_t Event, void * pArg)
{
    switch(Event)
    {
        case GAP_EVENT_CONNECTED:
            DEBUG_MESSAGE("Connected!\r\n");
            gbConnected = true;
            break;

        case GAP_EVENT_DISCONNECTED:
            DEBUG_MESSAGE("Disconnected!\r\n");
            geMode      = ADI_BLE_GAP_MODE_NOTCONNECTABLE;
            gbConnected = false;
            break;

        case DATA_EXCHANGE_RX_EVENT:
            DEBUG_MESSAGE("Data received!\r\n");
            break;

        case DATA_EXCHANGE_TX_COMPLETE:
            DEBUG_MESSAGE("Data sent!\r\n");
            break;

        case GAP_EVENT_MODE_CHANGE:
            DEBUG_MESSAGE("GAP mode changed.\r\n");
            break;

        case GAP_EVENT_CONNECTION_UPDATED:
            DEBUG_MESSAGE("Connection interval updated.\r\n");
            break;

        case BLE_RADIO_ERROR_READING:
            /* If you want to enable this print statement, please be aware that the first
             * packet sent from the radio on startup will cause this error. It is a known bug
             * and will not have any adverse effects on the application.
             *
             *DEBUG_MESSAGE("Failed to read a packet from the radio.\r\n");
             *
             */
            break;

        case BLE_RADIO_ERROR_PARSING:
            DEBUG_MESSAGE("Failed to parse a packet from the radio.\r\n");
            break;      
            
        case BLE_RADIO_ERROR_PROCESSING:
            DEBUG_MESSAGE("Failed to process a packet from the radio.\r\n");
            break;                                

        default:
            DEBUG_MESSAGE("Unexpected event received.\r\n");
            break;
    }
}


/*!
 * @brief      Set Advertising Mode
 *
 * @details    Helper function to avoid repeated code in main.
 *             
 */
static void SetAdvertisingMode(void)
{
    ADI_BLER_RESULT eResult;

    eResult = adi_radio_SetMode(PERIPHERAL_ADV_MODE, 0u, 0u);
    DEBUG_RESULT("Error setting the mode.\r\n", eResult, ADI_BLER_SUCCESS);

    eResult = adi_ble_WaitForEventWithTimeout(GAP_EVENT_MODE_CHANGE, 5000u);
    DEBUG_RESULT("Error waiting for GAP_EVENT_MODE_CHANGE.\r\n", eResult, ADI_BLER_SUCCESS);     

    eResult = adi_radio_GetMode(&geMode);
    DEBUG_RESULT("Error getting the mode.\r\n", eResult, ADI_BLER_SUCCESS);  

    if (geMode != PERIPHERAL_ADV_MODE)
    {
        DEBUG_MESSAGE("Error in SetAdvertisingMode.\r\n");
    } 
}
