/* particulate-matter-bricklet
 * Copyright (C) 2018 Matthias Bolte <matthias@tinkerforge.com>
 *
 * communication.h: TFP protocol message handling
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdint.h>
#include <stdbool.h>

#include "bricklib2/protocols/tfp/tfp.h"
#include "bricklib2/bootloader/bootloader.h"

// Default functions
BootloaderHandleMessageResponse handle_message(const void *data, void *response);
void communication_tick(void);
void communication_init(void);

// Constants
#define PARTICULATE_MATTER_BOOTLOADER_MODE_BOOTLOADER 0
#define PARTICULATE_MATTER_BOOTLOADER_MODE_FIRMWARE 1
#define PARTICULATE_MATTER_BOOTLOADER_MODE_BOOTLOADER_WAIT_FOR_REBOOT 2
#define PARTICULATE_MATTER_BOOTLOADER_MODE_FIRMWARE_WAIT_FOR_REBOOT 3
#define PARTICULATE_MATTER_BOOTLOADER_MODE_FIRMWARE_WAIT_FOR_ERASE_AND_REBOOT 4

#define PARTICULATE_MATTER_BOOTLOADER_STATUS_OK 0
#define PARTICULATE_MATTER_BOOTLOADER_STATUS_INVALID_MODE 1
#define PARTICULATE_MATTER_BOOTLOADER_STATUS_NO_CHANGE 2
#define PARTICULATE_MATTER_BOOTLOADER_STATUS_ENTRY_FUNCTION_NOT_PRESENT 3
#define PARTICULATE_MATTER_BOOTLOADER_STATUS_DEVICE_IDENTIFIER_INCORRECT 4
#define PARTICULATE_MATTER_BOOTLOADER_STATUS_CRC_MISMATCH 5

#define PARTICULATE_MATTER_STATUS_LED_CONFIG_OFF 0
#define PARTICULATE_MATTER_STATUS_LED_CONFIG_ON 1
#define PARTICULATE_MATTER_STATUS_LED_CONFIG_SHOW_HEARTBEAT 2
#define PARTICULATE_MATTER_STATUS_LED_CONFIG_SHOW_STATUS 3

// Function and callback IDs and structs
#define FID_GET_PM_CONCENTRATION 1
#define FID_GET_PM_COUNT 2
#define FID_SET_ENABLE 3
#define FID_GET_ENABLE 4
#define FID_GET_SENSOR_INFO 5
#define FID_SET_PM_CONCENTRATION_CALLBACK_CONFIGURATION 6
#define FID_GET_PM_CONCENTRATION_CALLBACK_CONFIGURATION 7
#define FID_SET_PM_COUNT_CALLBACK_CONFIGURATION 8
#define FID_GET_PM_COUNT_CALLBACK_CONFIGURATION 9

#define FID_CALLBACK_PM_CONCENTRATION 10
#define FID_CALLBACK_PM_COUNT 11

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetPMConcentration;

typedef struct {
	TFPMessageHeader header;
	uint16_t pm10;
	uint16_t pm25;
	uint16_t pm100;
} __attribute__((__packed__)) GetPMConcentration_Response;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetPMCount;

typedef struct {
	TFPMessageHeader header;
	uint16_t greater03um;
	uint16_t greater05um;
	uint16_t greater10um;
	uint16_t greater25um;
	uint16_t greater50um;
	uint16_t greater100um;
} __attribute__((__packed__)) GetPMCount_Response;

typedef struct {
	TFPMessageHeader header;
	bool enable;
} __attribute__((__packed__)) SetEnable;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetEnable;

typedef struct {
	TFPMessageHeader header;
	bool enable;
} __attribute__((__packed__)) GetEnable_Response;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetSensorInfo;

typedef struct {
	TFPMessageHeader header;
	uint8_t sensor_version;
	uint8_t last_error_code;
	uint8_t framing_error_count;
	uint8_t checksum_error_count;
} __attribute__((__packed__)) GetSensorInfo_Response;

typedef struct {
	TFPMessageHeader header;
	uint32_t period;
	bool value_has_to_change;
} __attribute__((__packed__)) SetPMConcentrationCallbackConfiguration;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetPMConcentrationCallbackConfiguration;

typedef struct {
	TFPMessageHeader header;
	uint32_t period;
	bool value_has_to_change;
} __attribute__((__packed__)) GetPMConcentrationCallbackConfiguration_Response;

typedef struct {
	TFPMessageHeader header;
	uint32_t period;
	bool value_has_to_change;
} __attribute__((__packed__)) SetPMCountCallbackConfiguration;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetPMCountCallbackConfiguration;

typedef struct {
	TFPMessageHeader header;
	uint32_t period;
	bool value_has_to_change;
} __attribute__((__packed__)) GetPMCountCallbackConfiguration_Response;

typedef struct {
	uint16_t pm10;
	uint16_t pm25;
	uint16_t pm100;
} __attribute__((__packed__)) PMConcentrationValue;

typedef struct {
	TFPMessageHeader header;
	PMConcentrationValue value;
} __attribute__((__packed__)) PMConcentration_Callback;

typedef struct {
	uint16_t greater03um;
	uint16_t greater05um;
	uint16_t greater10um;
	uint16_t greater25um;
	uint16_t greater50um;
	uint16_t greater100um;
} __attribute__((__packed__)) PMCountValue;

typedef struct {
	TFPMessageHeader header;
	PMCountValue value;
} __attribute__((__packed__)) PMCount_Callback;

// Function prototypes
BootloaderHandleMessageResponse get_pm_concentration(const GetPMConcentration *data, GetPMConcentration_Response *response);
BootloaderHandleMessageResponse get_pm_count(const GetPMCount *data, GetPMCount_Response *response);
BootloaderHandleMessageResponse set_enable(const SetEnable *data);
BootloaderHandleMessageResponse get_enable(const GetEnable *data, GetEnable_Response *response);
BootloaderHandleMessageResponse get_sensor_info(const GetSensorInfo *data, GetSensorInfo_Response *response);
BootloaderHandleMessageResponse set_pm_concentration_callback_configuration(const SetPMConcentrationCallbackConfiguration *data);
BootloaderHandleMessageResponse get_pm_concentration_callback_configuration(const GetPMConcentrationCallbackConfiguration *data, GetPMConcentrationCallbackConfiguration_Response *response);
BootloaderHandleMessageResponse set_pm_count_callback_configuration(const SetPMCountCallbackConfiguration *data);
BootloaderHandleMessageResponse get_pm_count_callback_configuration(const GetPMCountCallbackConfiguration *data, GetPMCountCallbackConfiguration_Response *response);

// Callbacks
bool handle_pm_concentration_callback(void);
bool handle_pm_count_callback(void);

#define COMMUNICATION_CALLBACK_TICK_WAIT_MS 1
#define COMMUNICATION_CALLBACK_HANDLER_NUM 2
#define COMMUNICATION_CALLBACK_LIST_INIT \
	handle_pm_concentration_callback, \
	handle_pm_count_callback, \

#endif
