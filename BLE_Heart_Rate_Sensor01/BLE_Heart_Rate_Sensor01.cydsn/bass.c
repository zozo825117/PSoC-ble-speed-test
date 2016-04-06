/*******************************************************************************
* File Name: bas.c
*
* Version: 1.0
*
* Description:
*  This file contains BAS callback handler function.
*
* Hardware Dependency:
*  CY8CKIT-042 BLE
* 
********************************************************************************
* Copyright 2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "common.h"
#include "bass.h"

#if (BAS_SIMULATE_ENABLE != 0)
uint16 batterySimulation = 0u;
#endif /* (BAS_SIMULATE_ENABLE != 0) */

#if (BAS_MEASURE_ENABLE != 0)
uint16 batteryMeasure = 0u;
#endif /* (BAS_MEASURE_ENABLE != 0) */


/*******************************************************************************
* Function Name: BasCallBack()
********************************************************************************
*
* Summary:
*   This is an event callback function to receive service specific events from 
*   Battery Service.
*
* Parameters:
*  event - the event code
*  *eventParam - the event parameters
*
* Return:
*  None.
*
*******************************************************************************/
void BasCallBack(uint32 event, void *eventParam)
{
    uint8 locServiceIndex;
    
    locServiceIndex = ((CYBLE_BAS_CHAR_VALUE_T *)eventParam)->serviceIndex;
    printf("BAS event: %lx, ", event);
    
    switch(event)
    {
        case CYBLE_EVT_BASS_NOTIFICATION_ENABLED:
            printf("EVT_BASS_NOTIFICATION_ENABLED: %x \r\n", locServiceIndex);
        #if (BAS_SIMULATE_ENABLE != 0)
            if(BAS_SERVICE_SIMULATE == locServiceIndex)
            {
                batterySimulation = ENABLED;
            }
        #endif /*  (BAS_SIMULATE_ENABLE != 0) */        
        #if (BAS_MEASURE_ENABLE != 0)
            if(BAS_SERVICE_MEASURE == locServiceIndex)
            {
                batteryMeasure = ENABLED;
            }
        #endif /*  (BAS_MEASURE_ENABLE != 0) */     
            break;
        case CYBLE_EVT_BASS_NOTIFICATION_DISABLED:
            printf("EVT_BASS_NOTIFICATION_DISABLED: %x \r\n", locServiceIndex);
        #if (BAS_SIMULATE_ENABLE != 0)
            if(BAS_SERVICE_SIMULATE == locServiceIndex)
            {
                batterySimulation = DISABLED;
            }
        #endif /*  (BAS_SIMULATE_ENABLE != 0) */        
        #if (BAS_MEASURE_ENABLE != 0)
            if(BAS_SERVICE_MEASURE == locServiceIndex)
            {
                batteryMeasure = DISABLED;
            }
        #endif /*  (BAS_MEASURE_ENABLE != 0) */     
            break;
        case CYBLE_EVT_BASC_NOTIFICATION:
            break;
        case CYBLE_EVT_BASC_READ_CHAR_RESPONSE:
            break;
        case CYBLE_EVT_BASC_READ_DESCR_RESPONSE:
            break;
        case CYBLE_EVT_BASC_WRITE_DESCR_RESPONSE:
            break;
		default:
            printf("Not supported event\r\n");
			break;
    }
}


#if (BAS_MEASURE_ENABLE != 0)
    

/*******************************************************************************
* Function Name: MeasureBattery()
********************************************************************************
*
* Summary:
*   This function measures the battery voltage and sends it to the client.
*
*******************************************************************************/
void MeasureBattery(void)
{
	int16 adcResult;
    int32 mvolts;
	uint32 sarControlReg;
    uint8 batteryLevel;
    CYBLE_API_RESULT_T apiResult;
    
    static uint32 batteryTimer = BATTERY_TIMEOUT;
    
    if(--batteryTimer == 0u) 
    {
        batteryTimer = BATTERY_TIMEOUT;
        
    	/* Set the reference to VBG and enable reference bypass */
    	sarControlReg = ADC_SAR_CTRL_REG & ~ADC_VREF_MASK;
    	ADC_SAR_CTRL_REG = sarControlReg | ADC_VREF_INTERNAL1024BYPASSED;
    	
    	/* 25 ms delay for reference capacitor to charge */
    	CyDelay(25);             
    	
    	/* Set the reference to VDD and disable reference bypass */
    	sarControlReg = ADC_SAR_CTRL_REG & ~ADC_VREF_MASK;
    	ADC_SAR_CTRL_REG = sarControlReg | ADC_VREF_VDDA;

    	/* Perform a measurement. Store this value in Vref. */
    	CyDelay(1);
    	ADC_StartConvert();
    	ADC_IsEndConversion(ADC_WAIT_FOR_RESULT);

        adcResult = ADC_GetResult16(ADC_BATTERY_CHANNEL);
    	/* Calculate input voltage by using ratio of ADC counts from reference
    	*  and ADC Full Scale counts. 
        */
    	mvolts = (1024 * 2048) / adcResult;
        
        /* Convert battery level voltage to percentage using linear approximation
        *  divided to two sections according to typical performance of 
        *  CR2033 battery specification:
        *  3V - 100%
        *  2.8V - 29%
        *  2.0V - 0%
        */
        if(mvolts < MEASURE_BATTERY_MIN)
        {
            batteryLevel = 0;
        }
        else if(mvolts < MEASURE_BATTERY_MID)
        {
            batteryLevel = (mvolts - MEASURE_BATTERY_MIN) * MEASURE_BATTERY_MID_PERCENT / 
                           (MEASURE_BATTERY_MID - MEASURE_BATTERY_MIN); 
        }
        else if(mvolts < MEASURE_BATTERY_MAX)
        {
            batteryLevel = MEASURE_BATTERY_MID_PERCENT +
                           (mvolts - MEASURE_BATTERY_MID) * (100 - MEASURE_BATTERY_MID_PERCENT) / 
                           (MEASURE_BATTERY_MAX - MEASURE_BATTERY_MID); 
        }
        else
        {
            batteryLevel = CYBLE_BAS_MAX_BATTERY_LEVEL_VALUE;
        }
    #if (BAS_MEASURE_LP_LED != 0u)
        if(batteryLevel < LOW_BATTERY_LIMIT)
        {
            LowPower_LED_Write(LED_ON);
        }
        else
        {
            LowPower_LED_Write(LED_OFF);
        }
    #endif /* (BAS_MEASURE_LP_LED != 0u) */
        /* Update Battery Level characteristic value */
        apiResult = CyBle_BassSendNotification(cyBle_connHandle, BAS_SERVICE_MEASURE, CYBLE_BAS_BATTERY_LEVEL, 
                        sizeof(batteryLevel), &batteryLevel);
        if(apiResult != CYBLE_ERROR_OK)
        {
            printf("BassSendNotification API Error: %x \r\n", apiResult);
            batteryMeasure = DISABLED;
        }
        else
        {
            printf("MeasureBatteryLevelUpdate: %d \r\n",batteryLevel);
        }
    }
}

#endif /*  (BAS_MEASURE_ENABLE != 0) */

#if (BAS_SIMULATE_ENABLE != 0)


/*******************************************************************************
* Function Name: SimulateBattery()
********************************************************************************
*
* Summary:
*   The custom function to simulate Battery Voltage.
*
*******************************************************************************/
void SimulateBattery(void)
{
    static uint32 batteryTimer = BATTERY_TIMEOUT;
    static uint8 batteryLevel = SIM_BATTERY_MIN;
    CYBLE_API_RESULT_T apiResult;
    
    if((CyBle_GattGetBusStatus() == CYBLE_STACK_STATE_FREE) && (--batteryTimer == 0u))
    {
        batteryTimer = BATTERY_TIMEOUT;
        
        /* Battery Level simulation */
        if(batterySimulation == ENABLED)
        {
            batteryLevel += SIM_BATTERY_INCREMENT;
            if(batteryLevel > SIM_BATTERY_MAX)
            {
                batteryLevel = SIM_BATTERY_MIN; 
            }
        }
        /* Update Battery Level characteristic value */
         apiResult = CyBle_BassSendNotification(cyBle_connHandle, BAS_SERVICE_SIMULATE, CYBLE_BAS_BATTERY_LEVEL, 
                            sizeof(batteryLevel), &batteryLevel);
        
        if(apiResult != CYBLE_ERROR_OK)
        {
            printf("BassSendNotification API Error: %x \r\n", apiResult);
            batterySimulation = DISABLED;
        }
        else
        {
            printf("SimulBatteryLevelUpdate: %d \r\n",batteryLevel);
        }
    }
}

#endif  /* (BAS_SIMULATE_ENABLE != 0) */




/* [] END OF FILE */
