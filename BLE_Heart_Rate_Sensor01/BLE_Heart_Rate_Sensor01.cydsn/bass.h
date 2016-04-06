/*******************************************************************************
* File Name: bas.h
* Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
*
* Description:
*  Contains the function prototypes and constants available to the example
*  project.
*
********************************************************************************
* Copyright 2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include <project.h>


/***************************************
*  Conditional Compilation Parameters
***************************************/
#define BAS_MEASURE_ENABLE          (0)     /* Set to 1 to enable Battery Level measurement using sequencer ADC */
#define BAS_SIMULATE_ENABLE         (1)     /* Set to 1 to enable Battery Level simulation */
#define BAS_MEASURE_LP_LED          (0)     /* Set to 1 to indicate <10% of battery level on LED */


/***************************************
*          Constants
***************************************/

#define BATTERY_TIMEOUT             (20000u)     /* Counts of 10 millisecond */

#define SIM_BATTERY_MIN             (2u)        /* Minimum simulated battery level measurement */
#define SIM_BATTERY_MAX             (20u)       /* Maximum simulated battery level measurement */
#define SIM_BATTERY_INCREMENT       (1u)        /* Value by which the battery level is incremented */                             

#define MEASURE_BATTERY_MAX         (3000)      /* Use 3V as battery voltage starting */
#define MEASURE_BATTERY_MID         (2800)      /* Use 2.8V as a knee point of discharge curve @ 29% */
#define MEASURE_BATTERY_MID_PERCENT (29)        
#define MEASURE_BATTERY_MIN         (2000)      /* Use 2V as a cut-off of battery life */
#define LOW_BATTERY_LIMIT           (10)        /* Low level limit in percent to switch on LED */
    

#define BAS_SERVICE_SIMULATE        (0u)        /* BAS service for simulation */ 
#define BAS_SERVICE_MEASURE         (0u)        /* BAS service for measure actual batteru level */  

#define ADC_VREF_MASK               (0x000000F0Lu)



/***************************************
*       Function Prototypes
***************************************/
void BasCallBack(uint32 event, void *eventParam);
#if (BAS_MEASURE_ENABLE != 0)
void MeasureBattery(void);
#endif /* BAS_MEASURE_ENABLE != 0 */

#if (BAS_SIMULATE_ENABLE != 0)
void SimulateBattery(void);
#endif /* BAS_SIMULATE_ENABLE != 0 */


/***************************************
* External data references
***************************************/
extern uint16 batterySimulation;
extern uint16 batteryMeasure;


/* [] END OF FILE */
