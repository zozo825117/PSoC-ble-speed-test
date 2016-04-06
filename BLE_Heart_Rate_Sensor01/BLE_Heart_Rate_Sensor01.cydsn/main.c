/*******************************************************************************
* File Name: main.c
*
* Version: 1.0
*
* Description:
*  This project demonstrates the operation of the Heart Rate Profile
*  in Server (Peripheral) role.
*
*******************************************************************************
* Copyright 2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "hrss.h"
#include "bass.h"

volatile uint32 mainTimer = 0;
volatile uint32 SysTickTimer = 0,LastTimer;
uint8 InterruptHpn;

CYBLE_API_RESULT_T apiResult;

uint8 test_buf[]={
	0,1,2,3,4,5,6,7,8,9,
	10,11,12,13,14,15,16,17,18,19,
	20,21,22,23,24,25,26,27,28,29,
	30,31,32,33,34,35,36,37,38,39,
	40,41,42,43,44,45,46,47,48,49,
	50,51,52,53,54,55,56,57,58,59,
	60,61,62,63,64,65,66,67,68,69,
	70,71,72,73,74,75,76,77,78,79,
	80,81,82,83,84,85,86,87,88,89,
	90,91,92,93,94,95,96,97,98,99,

};

uint8 test_ptr=0,send_flag=0;

uint16 connIntv;

#define EN_TT_CCC_HANDLE				        0x002Fu
#define TEST_THOUGHPUT_CCC_HANDLE				0x002Du
#define CCC_DATA_LEN										21u

#define TIMER_DELAY_2_5MS 							2  //
#define TIMER_DELAY_3_75MS 							3  //
#define TIMER_DELAY_5MS 								4  //
#define TIMER_DELAY_6_25MS 							5  //
#define TIMER_DELAY_7_5MS 							6  //
#define TIMER_DELAY_8_75MS 							7  //
#define TIMER_DELAY_2_5S 								2000  //
#define TIMER_DELAY_5S 									4000  //



/* General application callback */
void AppCallBack(uint32 event, void* eventParam)
{
	CYBLE_GATTS_WRITE_REQ_PARAM_T *wrReqParam;
	CYBLE_GATT_HANDLE_VALUE_PAIR_T zozoTThandle;

  printf("GAC event %lu \r\n",event);  
#ifdef DEBUG_OUT    
    DebugOut(event, eventParam);
#endif

    switch(event)
    {
        case CYBLE_EVT_STACK_ON:
        case CYBLE_EVT_GAP_DEVICE_DISCONNECTED:
        case CYBLE_EVT_TIMEOUT:
            heartRateSimulation = DISABLED;
            batterySimulation = DISABLED;
            /* Put the device into discoverable mode so that remote can search it. */
            StartAdvertisement();
            Advertising_LED_Write(LED_ON);
            test_ptr=0;
            break;
            
        case CYBLE_EVT_GAP_DEVICE_CONNECTED:
            Disconnect_LED_Write(LED_OFF);
						connIntv = ((CYBLE_GAP_CONN_PARAM_UPDATED_IN_CONTROLLER_T *)eventParam)->connIntv * 1000* 5u /4u;
            printf("EVT_GAP_DEVICE_CONNECTED: connIntv = %d us \r\n", connIntv);            
					break;

				case CYBLE_EVT_GATTS_WRITE_CMD_REQ:
					  //printf("evt gatts write req \r\n");
						wrReqParam = (CYBLE_GATTS_WRITE_REQ_PARAM_T *) eventParam;
            
						if(wrReqParam->handleValPair.attrHandle == 0x0030u)
							{
								//printf("zozo tt WR rec \r\n");  
								
								zozoTThandle.attrHandle = TEST_THOUGHPUT_CCC_HANDLE;
								zozoTThandle.value.val = test_buf;
								zozoTThandle.value.len = 20;
						
								/* Send notification to client using previously filled structure */
#if 0
								apiResult = CYBLE_ERROR_OK;
								if(CYBLE_GATT_ERR_NONE !=
										CyBle_GattsWriteAttributeValue(&CustomNotificationCCCDhandle, 0u, NULL, CYBLE_GATT_DB_LOCALLY_INITIATED))
								{
										apiResult = CYBLE_ERROR_INVALID_PARAMETER;
								} 			
#endif
#if 1
								apiResult = CyBle_GattsNotification(cyBle_connHandle, &zozoTThandle);
#endif
								//CyBle_ProcessEvents();

								if(apiResult == CYBLE_ERROR_OK)
								{
									//test_1_Write(~ test_1_Read());
									printf("zozo tt ntf TEST_THOUGHPUT_CCC_HANDLE \r\n");
								}
								else
									printf("zozo tt ntf TEST_THOUGHPUT_CCC_HANDLE error: %xd \r\n", apiResult);
								
							}

						
						
					break;

        default:
            break;
    }
}


/*******************************************************************************
* Function Name: Timer_Interrupt
********************************************************************************
*
* Summary:
*  Handles the Interrupt Service Routine for the WDT timer.
*  It is called from common WDT ISR located in BLE component. 
*
*******************************************************************************/
void Timer_Interrupt(void)
{
    if(CySysWdtGetInterruptSource() & WDT_INTERRUPT_SOURCE)
    {
        static uint8 led = LED_OFF;
        
        /* Blink LED to indicate that device advertises */
        if(CYBLE_STATE_ADVERTISING == CyBle_GetState())
        {
            led ^= LED_OFF;
            Advertising_LED_Write(led);
        }
        
        /* Indicate that timer is raised to the main loop */
        mainTimer++;
        
        /* Clears interrupt request  */
        CySysWdtClearInterrupt(WDT_INTERRUPT_SOURCE);
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
				SysTickTimer++;
				
    }
}


/*******************************************************************************
* Function Name: WDT_Start
********************************************************************************
*
* Summary:
*  Configures WDT(counter 2) to trigger an interrupt every second.
*
*******************************************************************************/
void WDT_Start(void)
{
   WdtIsr_StartEx(&Timer_Interrupt);
    /* Unlock the WDT registers for modification */
    CySysWdtUnlock(); 
    /* Setup ISR callback */
    //CyBle_WdtRegisterIsrCallback(&Timer_Interrupt);
    /* Write the mode to generate interrupt on match */
    CySysWdtWriteMode(WDT_COUNTER, CY_SYS_WDT_MODE_INT);
    /* Configure the WDT counter clear on a match setting */
    CySysWdtWriteClearOnMatch(WDT_COUNTER, WDT_COUNTER_ENABLE);
    /* Configure the WDT counter match comparison value */
    CySysWdtWriteMatch(WDT_COUNTER, WDT_1SEC);
    /* Reset WDT counter */
    CySysWdtResetCounters(WDT_COUNTER);
    /* Enable the specified WDT counter */
    CySysWdtEnable(WDT_COUNTER_MASK);
    /* Lock out configuration changes to the Watchdog timer registers */
    CySysWdtLock();    
}
void HandleLeds(void)
{
    static uint32 ledTimer = LED_TIMEOUT;
    static uint8 greenLed = LED_OFF;
    
    /* Blink blue LED to indicate that device advertises */
    if(CyBle_GetState() == CYBLE_STATE_ADVERTISING)
    {
        if(--ledTimer == 0u) 
        {
            ledTimer = LED_TIMEOUT;
            greenLed ^= LED_OFF;
            Advertising_LED_Write(greenLed);
        }
    }
}


int main()
{
		//uint8 i;
    CYBLE_LP_MODE_T lpMode;
		CYBLE_GATT_HANDLE_VALUE_PAIR_T CustomNotificationCCCDhandle;

    CyGlobalIntEnable;
    
    UART_DEB_Start();               /* Start communication component */
    printf("BLE Heart Rate Sensor Example Project \r\n");
    
    Disconnect_LED_Write(LED_OFF);
    Advertising_LED_Write(LED_OFF);

    /* Start CYBLE component and register generic event handler */
    apiResult = CyBle_Start(AppCallBack);
    if(apiResult != CYBLE_ERROR_OK)
    {
        printf("CyBle_Start API Error: %x \r\n", apiResult);
    }

    CyBle_BasRegisterAttrCallback(BasCallBack);
    CyBle_HrsRegisterAttrCallback(HeartRateCallBack);
    HrsInit();
    WDT_Start();
		
		/* Enable the Interrupt component connected to interrupt */
		TC_CC_ISR_StartEx(InterruptHandler);
		
		/* Start the components */
		Timer_Start();



    /***************************************************************************
    * Main polling loop
    ***************************************************************************/
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
                /* Handle advertising led blinking */
                HandleLeds();
            }
        }
        
        
        /***********************************************************************
        * Wait for connection established with Central device
        ***********************************************************************/
        if(CyBle_GetState() == CYBLE_STATE_CONNECTED)
        {
            /*******************************************************************
            *  Periodically measure a battery level and temperature and send 
            *  results to the Client
            *******************************************************************/    
						#if 1

						if( SysTickTimer-LastTimer >= TIMER_DELAY_2_5S && send_flag)
							{
								if(CyBle_GattGetBusStatus() == CYBLE_STACK_STATE_FREE)
									{
										LastTimer = SysTickTimer;

										#if 0
										test_buf[0] = test_ptr;
										if(test_ptr == 2)
											{
												LastTimer = SysTickTimer;
												test_ptr = 0;
											}
										else
											{
												test_ptr++;
											}
										#endif

										CustomNotificationCCCDhandle.attrHandle = EN_TT_CCC_HANDLE;
										CustomNotificationCCCDhandle.value.val = test_buf;
										CustomNotificationCCCDhandle.value.len = 2;
								
										/* Send notification to client using previously filled structure */
#if 0
		                apiResult = CYBLE_ERROR_OK;
		                if(CYBLE_GATT_ERR_NONE !=
		                    CyBle_GattsWriteAttributeValue(&CustomNotificationCCCDhandle, 0u, NULL, CYBLE_GATT_DB_LOCALLY_INITIATED))
		                {
		                    apiResult = CYBLE_ERROR_INVALID_PARAMETER;
		                }				
#endif
#if 1
										apiResult = CyBle_GattsNotification(cyBle_connHandle, &CustomNotificationCCCDhandle);
#endif
		                CyBle_ProcessEvents();

		                if(apiResult == CYBLE_ERROR_OK)
		                {
		                  test_1_Write(~ test_1_Read());
	                    printf("zozo tt ntf 0x002f \r\n");
		                }
	                  else
	                    printf("zozo tt ntf 0x002f error: %xd \r\n", apiResult);
								}
							}
						#endif
            if(mainTimer != 0u)
            {
                mainTimer = 0u;

  							if(heartRateSimulation == ENABLED)
                {
                    SimulateHeartRate();
                    CyBle_ProcessEvents();
										send_flag = 1;
                }              
                if(batterySimulation == ENABLED)
                {
                    SimulateBattery();
                    CyBle_ProcessEvents();
                }
                #if 1
                if(storeBondingData == ENABLED)
                {
                    cystatus retValue;
                    retValue = CyBle_StoreBondingData(0u);
                    printf("Store bonding data, status: %lx \r\n", retValue);
                    storeBondingData = DISABLED;
                }
                #endif
            }
        }
        
        /*******************************************************************
        *  Processes all pending BLE events in the stack
        *******************************************************************/        
        CyBle_ProcessEvents();

    }
}


/* [] END OF FILE */
