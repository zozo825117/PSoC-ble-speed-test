/*******************************************************************************
* File Name: main.c
*
* Version: 1.0
*
* Description:
*  This project demonstrates the operation of the Heart Rate Profile
*  in Collector (Central) role.
*
* Related Document:
*  HEART RATE PROFILE SPECIFICATION v1.0
*  HEART RATE SERVICE SPECIFICATION v1.0
*
*******************************************************************************
* Copyright 2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "hrsc.h"
#include "basc.h"

CYBLE_API_RESULT_T apiResult;
uint32 TimerCnt1ms=0;
uint8 InterruptHpn;

void AppCallBack(uint32 event, void* eventParam)
{

	CYBLE_GATTC_WRITE_CMD_REQ_T zozoParam;
	uint8 zozoArrage[2];

#ifdef DEBUG_OUT    
    DebugOut(event, eventParam);
#endif
    
    switch(event)
    {
        case CYBLE_EVT_STACK_ON:
        case CYBLE_EVT_GAP_DEVICE_DISCONNECTED:
        case CYBLE_EVT_TIMEOUT:
            StartScan(CYBLE_UUID_HIDS_SERVICE);
            Scanning_LED_Write(LED_ON);
            break;

        case CYBLE_EVT_GAP_DEVICE_CONNECTED:
            //StartDiscovery();
            Disconnect_LED_Write(LED_OFF);

						printf("zozo tt GATTC NTF 0x000f\r\n");
						zozoArrage[0] = 0x01;
						zozoArrage[1] = 0x00;
						zozoParam.attrHandle = 0x001eu;
						zozoParam.value.len = 2;
						zozoParam.value.val = zozoArrage;
						apiResult = CyBle_GattcWriteWithoutResponse(cyBle_connHandle,&zozoParam);
						
						if(apiResult == CYBLE_ERROR_OK)
							{
								printf("zozo test write cmd send\r\n");
							}
						CyBle_ProcessEvents();

						zozoArrage[0] = 0x01;
						zozoArrage[1] = 0x00;
						zozoParam.attrHandle = 0x000fu;
						zozoParam.value.len = 2;
						zozoParam.value.val = zozoArrage;
						apiResult = CyBle_GattcWriteWithoutResponse(cyBle_connHandle,&zozoParam);
						
						if(apiResult == CYBLE_ERROR_OK)
							{
								printf("zozo test write cmd send\r\n");
							}

						CyBle_ProcessEvents();

						CyBle_GattcExchangeMtuReq(cyBle_connHandle,0x200);
						if(apiResult == CYBLE_ERROR_OK)
							{
								printf("zozo test exchg mtu req send\r\n");
							}


            break;

        case CYBLE_EVT_GATTC_DISCOVERY_COMPLETE:
            /* Send request to read the body sensor location char. 
            apiResult = HrscReadBodySensorLocation();

            if(apiResult != CYBLE_ERROR_OK)
            {
                printf("HrscReadBodySensorLocation API Error: %xd \r\n", apiResult);
            }
            else
            {
                printf("Body Sensor Location Read Request is sent \r\n");
            }*/
            break;

        default:
            break;
    }
}

void HandleLeds(void)
{
    static uint32 ledTimer = LED_TIMEOUT;
    static uint8 greenLed = LED_OFF;
    
    /* Blink green LED to indicate that device advertises */
    if(CyBle_GetState() == CYBLE_STATE_SCANNING)
    {
        if(--ledTimer == 0u) 
        {
            ledTimer = LED_TIMEOUT;
            greenLed ^= LED_OFF;
            Scanning_LED_Write(greenLed);
        }
    }
    /* Blink blue LED to indicate that device has received a notification */
    else if(CyBle_GetState() == CYBLE_STATE_CONNECTED)
    {
        if((LED_ON == Notification_LED_Read()) && (--ledTimer == 0u))
        {
            ledTimer = LED_TIMEOUT;
            Notification_LED_Write(LED_OFF);
        }
    }
    else
    {
        /* dummy else block to avoid MISRA warning */
    }
}
/*******************************************************************************
* Define Interrupt service routine and allocate an vector to the Interrupt
********************************************************************************/
CY_ISR(InterruptHandler)
{
    /* Check interrupt source and clear Inerrupt */
    InterruptHpn = Timer_GetInterruptSourceMasked();
   	if (InterruptHpn == Timer_INTR_MASK_CC_MATCH)
    {
        Timer_ClearInterrupt(Timer_INTR_MASK_CC_MATCH);      
        //test_1_Write(~ test_1_Read());
    }
    else
    {
        Timer_ClearInterrupt(Timer_INTR_MASK_TC);
        //test_2_Write(~ test_2_Read());
				TimerCnt1ms++;
				
    }
}

int main()
{
    CYBLE_LP_MODE_T lpMode;

    CyGlobalIntEnable;
    
    UART_DEB_Start();               /* Start communication component */
    printf("BLE Heart Rate Collector Example Project \r\n");
    
    Disconnect_LED_Write(LED_OFF);
    Scanning_LED_Write(LED_OFF);
    Notification_LED_Write(LED_OFF);

    apiResult = CyBle_Start(AppCallBack);
    if(apiResult != CYBLE_ERROR_OK)
    {
        printf("CyBle_Start API Error: %xd \r\n", apiResult);
    }

    CyBle_HrsRegisterAttrCallback(HeartRateCallBack);
    HrsInit();
    CyBle_BasRegisterAttrCallback(BasCallBack);
    
			/* Enable the Interrupt component connected to interrupt */
			TC_CC_ISR_StartEx(InterruptHandler);
		
		/* Start the components */
			Timer_Start();
    while(1)
    {
        if(CyBle_GetState() != CYBLE_STATE_INITIALIZING)
        {
            /* Enter DeepSleep mode between connection intervals */
            lpMode = CyBle_EnterLPM(CYBLE_BLESS_DEEPSLEEP);
            if(lpMode == CYBLE_BLESS_DEEPSLEEP) 
            {
                /* Put the device into the Deep Sleep mode only when all debug information has been sent */
                if(UART_DEB_SpiUartGetTxBufferSize() == 0u)
                {
                    CySysPmDeepSleep();
                }
                else
                {
                    CySysPmSleep();
                }
                
                /* Handle scanning led blinking */
                HandleLeds();
            }
        }
        
        
        /*******************************************************************
        *  Processes all pending BLE events in the stack
        *******************************************************************/        
        CyBle_ProcessEvents();
    }
}

/* [] END OF FILE */
