/*
File Name: CYBLE_StackTypes.h

Version: 1.0

Description:
1. This file contains the platform data types, controller exported constants and Generic Error Codes.

Note:

Owner:
KGUP

Related Document:
1. BLE Standard Spec - CoreV4.1, CSS, CSAs, ESR05, ESR06


Copyright (2014) , Cypress Semiconductor Corporation.

This software is owned by Cypress Semiconductor Corporation (Cypress) and is
protected by and subject to worldwide patent protection (United States and
foreign), United States copyright laws and international treaty provisions.
Cypress hereby grants to licensee a personal, non-exclusive, non-transferable
license to copy, use, modify, create derivative works of, and compile the
Cypress Source Code and derivative works for the sole purpose of creating
custom software in support of licensee product to be used only in conjunction
with a Cypress integrated circuit as specified in the applicable agreement.
Any reproduction, modification, translation, compilation, or representation of
this software except as specified above is prohibited without the express
written permission of Cypress.

Disclaimer: CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, WITH
REGARD TO THIS MATERIAL, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
Cypress reserves the right to make changes without further notice to the
materials described herein. Cypress does not assume any liability arising out
of the application or use of any product or circuit described herein. Cypress
does not authorize its products for use as critical components in life-support
systems where a malfunction or failure may reasonably be expected to result in
significant injury to the user. The inclusion of Cypress' product in a life-
support systems application implies that the manufacturer assumes all risk of
such use and in doing so indemnifies Cypress against all charges. Use may be
limited by and subject to the applicable Cypress software license agreement.
*/

#ifndef CY_BLE_CYBLE_STACK_TYPES_H
#define CY_BLE_CYBLE_STACK_TYPES_H






/* Common error codes received as API result */
typedef enum
{
	/*Common error codes*/
	CYBLE_ERROR_OK = 0x00u,
	CYBLE_ERROR_INVALID_PARAMETER,
	CYBLE_ERROR_INVALID_OPERATION,
	CYBLE_ERROR_MEMORY_ALLOCATION_FAILED,
   	CYBLE_ERROR_INSUFFICIENT_RESOURCES,
	CYBLE_ERROR_OOB_NOT_AVAILABLE,
	CYBLE_ERROR_NO_CONNECTION,
	CYBLE_ERROR_NO_DEVICE_ENTITY,
	CYBLE_ERROR_REPEATED_ATTEMPTS,
	CYBLE_ERROR_GAP_ROLE,
	CYBLE_ERROR_TX_POWER_READ,
	CYBLE_ERROR_BT_ON_NOT_COMPLETED,
	CYBLE_ERROR_SEC_FAILED,

	/*L2CAP error codes*/
	CYBLE_ERROR_L2CAP_PSM_WRONG_ENCODING = 0x0Du,
	CYBLE_ERROR_L2CAP_PSM_ALREADY_REGISTERED,
	CYBLE_ERROR_L2CAP_PSM_NOT_REGISTERED,
	CYBLE_ERROR_L2CAP_CONNECTION_ENTITY_NOT_FOUND,
	CYBLE_ERROR_L2CAP_CHANNEL_NOT_FOUND,
	CYBLE_ERROR_L2CAP_NOT_ENOUGH_CREDITS,
	CYBLE_ERROR_L2CAP_PSM_NOT_IN_RANGE,

	/*GATT DB error codes*/
	CYBLE_ERROR_GATT_DB_INVALID_OFFSET = 0x14u,
	CYBLE_ERROR_GATT_DB_NULL_PARAMETER_NOT_ALLOWED,
	CYBLE_ERROR_GATT_DB_UNSUPPORTED_GROUP_TYPE,
	CYBLE_ERROR_GATT_DB_INSUFFICIENT_BUFFER_LEN,
	CYBLE_ERROR_GATT_DB_MORE_MATCHING_RESULT_FOUND,
	CYBLE_ERROR_GATT_DB_NO_MATCHING_RESULT,
	CYBLE_ERROR_GATT_DB_HANDLE_NOT_FOUND,
	CYBLE_ERROR_GATT_DB_HANDLE_NOT_IN_RANGE,
	CYBLE_ERROR_GATT_DB_HANDLE_IN_GROUP_RANGE,
	CYBLE_ERROR_GATT_DB_INVALID_OPERATION,
	CYBLE_ERROR_GATT_DB_UUID_NOT_IN_BT_SPACE,
	CYBLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE,
	CYBLE_ERROR_GATT_DB_INSUFFICIENT_SECURITY,
	CYBLE_ERROR_GATT_DB_INSUFFICIENT_ENC_KEY_SIZE,
	CYBLE_ERROR_GATT_DB_INVALID_INSTANCE,
	CYBLE_ERROR_GATT_DB_INCORRECT_UUID_FRMT,
	CYBLE_ERROR_GATT_DB_UUID_FRMT_UNSUPPORTED,
	CYBLE_ERROR_GATT_DB_TYPE_MISMATCH,
	CYBLE_ERROR_GATT_DB_INSUFFICIENT_ENCRYPTION,
	
	/* New Codes. CDT#188451 */
	CYBLE_ERROR_DEVICE_ALREADY_EXISTS = 0x27u,

    CYBLE_ERROR_MAX = 0xFFu,

    /* Profile level API_RESULT codes will be here */
    CYBLE_INCL_DISC_DONE,
    CYBLE_CHAR_DISC_DONE,
    CYBLE_DESCR_DISC_DONE,
    CYBLE_ERROR_NTF_DISABLED,
    CYBLE_ERROR_IND_DISABLED,
    CYBLE_ERROR_CHAR_IS_NOT_DISCOVERED,
    CYBLE_ERROR_INVALID_STATE

}CYBLE_API_RESULT_T;

typedef enum
{
    CYBLE_BLESS_ACTIVE = 0x01u,
    CYBLE_BLESS_SLEEP,
    CYBLE_BLESS_DEEPSLEEP_ECO_ON,
    CYBLE_BLESS_DEEPSLEEP,
    CYBLE_BLESS_HIBERNATE,
    CYBLE_BLESS_INVALID = 0xFFu
} CYBLE_LP_MODE_T;

#endif /* CY_BLE_CYBLE_STACK_TYPES_H */

/* EOF */
