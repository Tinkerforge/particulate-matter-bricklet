/* particulate-matter-bricklet
 * Copyright (C) 2018 Matthias Bolte <matthias@tinkerforge.com>
 *
 * communication.c: TFP protocol message handling
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

#include "communication.h"

#include "bricklib2/utility/communication_callback.h"
#include "bricklib2/hal/system_timer/system_timer.h"
#include "bricklib2/protocols/tfp/tfp.h"

#include "pms7003.h"

static uint32_t pm_concentration_cb_period = 0;
static bool pm_concentration_cb_value_has_to_change = false;

static uint32_t pm_count_cb_period = 0;
static bool pm_count_cb_value_has_to_change = false;

BootloaderHandleMessageResponse handle_message(const void *message, void *response) {
	switch(tfp_get_fid_from_message(message)) {
		case FID_GET_PM_CONCENTRATION: return get_pm_concentration(message, response);
		case FID_GET_PM_COUNT: return get_pm_count(message, response);
		case FID_SET_ENABLE: return set_enable(message);
		case FID_GET_ENABLE: return get_enable(message, response);
		case FID_GET_SENSOR_INFO: return get_sensor_info(message, response);
		case FID_SET_PM_CONCENTRATION_CALLBACK_CONFIGURATION: return set_pm_concentration_callback_configuration(message);
		case FID_GET_PM_CONCENTRATION_CALLBACK_CONFIGURATION: return get_pm_concentration_callback_configuration(message, response);
		case FID_SET_PM_COUNT_CALLBACK_CONFIGURATION: return set_pm_count_callback_configuration(message);
		case FID_GET_PM_COUNT_CALLBACK_CONFIGURATION: return get_pm_count_callback_configuration(message, response);
		default: return HANDLE_MESSAGE_RESPONSE_NOT_SUPPORTED;
	}
}

BootloaderHandleMessageResponse get_pm_concentration(const GetPMConcentration *data, GetPMConcentration_Response *response) {
	response->header.length = sizeof(GetPMConcentration_Response);
	response->pm10          = pms7003.frame_good.conc_pm1_0_amb;
	response->pm25          = pms7003.frame_good.conc_pm2_5_amb;
	response->pm100         = pms7003.frame_good.conc_pm10_0_amb;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse get_pm_count(const GetPMCount *data, GetPMCount_Response *response) {
	response->header.length = sizeof(GetPMCount_Response);
	response->greater03um   = pms7003.frame_good.raw_gt0_3um;
	response->greater05um   = pms7003.frame_good.raw_gt0_5um;
	response->greater10um   = pms7003.frame_good.raw_gt1_0um;
	response->greater25um   = pms7003.frame_good.raw_gt2_5um;
	response->greater50um   = pms7003.frame_good.raw_gt5_0um;
	response->greater100um  = pms7003.frame_good.raw_gt10_0um;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_enable(const SetEnable *data) {
	pms7003.enable = data->enable;

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_enable(const GetEnable *data, GetEnable_Response *response) {
	response->header.length = sizeof(GetEnable_Response);
	response->enable        = pms7003.enable;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse get_sensor_info(const GetSensorInfo *data, GetSensorInfo_Response *response) {
	response->header.length         = sizeof(GetSensorInfo_Response);
	response->sensor_version        = pms7003.frame_good.version;
	response->last_error_code       = pms7003.frame_good.error_code;
	response->framing_error_count   = pms7003.framing_error_counter;
	response->checksum_error_count  = pms7003.checksum_error_counter;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_pm_concentration_callback_configuration(const SetPMConcentrationCallbackConfiguration *data) {
	pm_concentration_cb_period              = data->period;
	pm_concentration_cb_value_has_to_change = data->value_has_to_change;

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_pm_concentration_callback_configuration(const GetPMConcentrationCallbackConfiguration *data, GetPMConcentrationCallbackConfiguration_Response *response) {
	response->header.length       = sizeof(GetPMConcentrationCallbackConfiguration_Response);
	response->period              = pm_concentration_cb_period;
	response->value_has_to_change = pm_concentration_cb_value_has_to_change;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_pm_count_callback_configuration(const SetPMCountCallbackConfiguration *data) {
	pm_count_cb_period              = data->period;
	pm_count_cb_value_has_to_change = data->value_has_to_change;

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_pm_count_callback_configuration(const GetPMCountCallbackConfiguration *data, GetPMCountCallbackConfiguration_Response *response) {
	response->header.length       = sizeof(GetPMCountCallbackConfiguration_Response);
	response->period              = pm_count_cb_period;
	response->value_has_to_change = pm_count_cb_value_has_to_change;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

bool handle_pm_concentration_callback(void) {
	static bool is_buffered = false;
	static PMConcentration_Callback cb;
	static PMConcentrationValue last_value = {0};
	static uint32_t last_time = 0;

	if (!is_buffered) {
		if (pm_concentration_cb_period == 0 ||
		    !system_timer_is_time_elapsed_ms(last_time, pm_concentration_cb_period)) {
			return false;
		}

		tfp_make_default_header(&cb.header, bootloader_get_uid(), sizeof(PMConcentration_Callback), FID_CALLBACK_PM_CONCENTRATION);

		cb.value.pm10  = pms7003.frame_good.conc_pm1_0_amb;
		cb.value.pm25  = pms7003.frame_good.conc_pm2_5_amb;
		cb.value.pm100 = pms7003.frame_good.conc_pm10_0_amb;

		if (pm_concentration_cb_value_has_to_change &&
		    memcmp(&cb.value, &last_value, sizeof(PMConcentrationValue)) == 0) {
			return false;
		}

		memcpy(&last_value, &cb.value, sizeof(PMConcentrationValue));
		last_time = system_timer_get_ms();
	}

	if (bootloader_spitfp_is_send_possible(&bootloader_status.st)) {
		bootloader_spitfp_send_ack_and_message(&bootloader_status, (uint8_t*)&cb, sizeof(PMConcentration_Callback));
		is_buffered = false;
		return true;
	} else {
		is_buffered = true;
	}

	return false;
}

bool handle_pm_count_callback(void) {
	static bool is_buffered = false;
	static PMCount_Callback cb;
	static PMCountValue last_value = {0};
	static uint32_t last_time = 0;

	if (!is_buffered) {
		if (pm_count_cb_period == 0 ||
		    !system_timer_is_time_elapsed_ms(last_time, pm_count_cb_period)) {
			return false;
		}

		tfp_make_default_header(&cb.header, bootloader_get_uid(), sizeof(PMCount_Callback), FID_CALLBACK_PM_COUNT);

		cb.value.greater03um   = pms7003.frame_good.raw_gt0_3um;
		cb.value.greater05um   = pms7003.frame_good.raw_gt0_5um;
		cb.value.greater10um   = pms7003.frame_good.raw_gt1_0um;
		cb.value.greater25um   = pms7003.frame_good.raw_gt2_5um;
		cb.value.greater50um   = pms7003.frame_good.raw_gt5_0um;
		cb.value.greater100um  = pms7003.frame_good.raw_gt10_0um;

		if (pm_count_cb_value_has_to_change &&
		    memcmp(&cb.value, &last_value, sizeof(PMCountValue)) == 0) {
			return false;
		}

		memcpy(&last_value, &cb.value, sizeof(PMCountValue));
		last_time = system_timer_get_ms();
	}

	if (bootloader_spitfp_is_send_possible(&bootloader_status.st)) {
		bootloader_spitfp_send_ack_and_message(&bootloader_status, (uint8_t*)&cb, sizeof(PMCount_Callback));
		is_buffered = false;
		return true;
	} else {
		is_buffered = true;
	}

	return false;
}

void communication_tick(void) {
	communication_callback_tick();
}

void communication_init(void) {
	communication_callback_init();
}
