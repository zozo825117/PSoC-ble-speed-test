/*******************************************************************************
File Name: CYBLE_custom.h
Version 1.20

Description:
 Contains the function prototypes and constants for the Custom Service.

********************************************************************************
Copyright 2014-2015, Cypress Semiconductor Corporation.  All rights reserved.
You may use this file only in accordance with the license, terms, conditions,
disclaimers, and limitations in the end user license agreement accompanying
the software package with which this file was provided.
*******************************************************************************/


#if !defined(CY_BLE_CYBLE_CUSTOM_H)
#define CY_BLE_CYBLE_CUSTOM_H

#include "CYBLE_gatt.h"


/***************************************
##Conditional Compilation Parameters
***************************************/

/* Maximum supported Custom Services */
#define CYBLE_CUSTOMS_SERVICE_COUNT                  (0x00u)
#define CYBLE_CUSTOMC_SERVICE_COUNT                  (0x01u)
#define CYBLE_CUSTOM_SERVICE_CHAR_COUNT              (0x00u)
#define CYBLE_CUSTOM_SERVICE_CHAR_DESCRIPTORS_COUNT  (0x00u)

/* Below are the indexes and handles of the defined Custom Services and their characteristics */


	

/***************************************
##Data Struct Definition
***************************************/

#if(0u != CYBLE_CUSTOMS_SERVICE_COUNT)

/* Contains information about Custom Characteristic structure */
typedef struct
{
    /* Custom Characteristic handle */
    CYBLE_GATT_DB_ATTR_HANDLE_T customServiceCharHandle;
    /* Custom Characteristic Descriptors handles */
    CYBLE_GATT_DB_ATTR_HANDLE_T customServiceCharDescriptors[CYBLE_CUSTOM_SERVICE_CHAR_DESCRIPTORS_COUNT];
} CYBLE_CUSTOMS_INFO_T;

/* Structure with Custom Service attribute handles. */
typedef struct
{
    /* Handle of a Custom Service */
    CYBLE_GATT_DB_ATTR_HANDLE_T customServiceHandle;
    
    /* Information about Custom Characteristics */
    CYBLE_CUSTOMS_INFO_T customServiceInfo[CYBLE_CUSTOM_SERVICE_CHAR_COUNT];
} CYBLE_CUSTOMS_T;


#endif /* (0u != CYBLE_CUSTOMS_SERVICE_COUNT) */

/* DOM-IGNORE-BEGIN */
/* The custom Client functionality is not functional in current version of 
* the component.
*/
#if(0u != CYBLE_CUSTOMC_SERVICE_COUNT)

typedef struct
{
    /* Custom Descriptor handle */
    CYBLE_GATT_DB_ATTR_HANDLE_T descHandle;
	/* Custom Descriptor UUID */
	CYBLE_UUID_T uuid;           
    /* UUID Format - 16-bit (0x01) or 128-bit (0x02) */
	uint8 uuidFormat;
   
} CYBLE_CUSTOMC_DESC_T;

typedef struct
{
    /* Characteristic handle */
    CYBLE_GATT_DB_ATTR_HANDLE_T customServiceCharHandle;
	/* Characteristic end handle */
    CYBLE_GATT_DB_ATTR_HANDLE_T customServiceCharEndHandle;
	/* Characteristic UUID */
	CYBLE_UUID_T uuid;           
    /* UUID Format - 16-bit (0x01) or 128-bit (0x02) */
	uint8 uuidFormat;
	/* Number of descriptors */
    uint8 descCount;
    /* Characteristic Descriptors */
    CYBLE_CUSTOMC_DESC_T * customServiceCharDescriptors;
} CYBLE_CUSTOMC_CHAR_T;

/* Structure with discovered attributes information of Custom Service */
typedef struct
{
    /* Custom Service handle */
    CYBLE_GATT_DB_ATTR_HANDLE_T customServiceHandle;
	/* Service UUID */
	CYBLE_UUID_T uuid;           
    /* UUID Format - 16-bit (0x01) or 128-bit (0x02) */
	uint8 uuidFormat;
	/* Number of characteristics */
    uint8 charCount;
    /* Custom Service Characteristics */
    CYBLE_CUSTOMC_CHAR_T * customServiceInfo;
} CYBLE_CUSTOMC_T;

#endif /* (0u != CYBLE_CUSTOMC_SERVICE_COUNT) */
/* DOM-IGNORE-END */


#if(0u != CYBLE_CUSTOMS_SERVICE_COUNT)

extern const CYBLE_CUSTOMS_T cyBle_customs[CYBLE_CUSTOMS_SERVICE_COUNT];

#endif /* (0u != CYBLE_CUSTOMS_SERVICE_COUNT) */

/* DOM-IGNORE-BEGIN */
#if(0u != CYBLE_CUSTOMC_SERVICE_COUNT)

extern CYBLE_CUSTOMC_T cyBle_customc[CYBLE_CUSTOMC_SERVICE_COUNT];

#endif /* (0u != CYBLE_CUSTOMC_SERVICE_COUNT) */
/* DOM-IGNORE-END */

#endif /* CY_BLE_CYBLE_CUSTOM_H  */


/* [] END OF FILE */
