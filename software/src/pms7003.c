/* particulate-matter-bricklet
 * Copyright (C) 2018 Olaf Lüke <olaf@tinkerforge.com>
 *
 * pms7003.c: Driver for PMS7003 sensor
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

#include "pms7003.h"

#include "configs/config_pms7003.h"

#include "bricklib2/hal/system_timer/system_timer.h"
#include "bricklib2/utility/ringbuffer.h"
#include "bricklib2/logging/logging.h"

#include "xmc_uart.h"
#include "xmc_scu.h"

// TODO:
//  * Create API
//  * Implement API
//  * API doc: 30 seconds after startup values ok
//  * Add support for on/off
//  * API: get_particles (gt03um, gt05um, gt10um, gt25um, gt50um, gt100um)
//  * API: get_concentration (pm10, pm25, pm100)

#define pms7003_rx_irq_handler  IRQ_Hdlr_11
#define pms7003_tx_irq_handler  IRQ_Hdlr_12

PMS7003 pms7003;

// Set const pointer to rx ringbuffer variables.
// With this the compiler can properly optimize the access!
uint8_t  *const pms7003_ringbuffer_rx_buffer = &(pms7003.buffer_rx[0]);
uint16_t *const pms7003_ringbuffer_rx_end    = &(pms7003.ringbuffer_rx.end);
uint16_t *const pms7003_ringbuffer_rx_start  = &(pms7003.ringbuffer_rx.start);
uint16_t *const pms7003_ringbuffer_rx_size   = &(pms7003.ringbuffer_rx.size);

void __attribute__((optimize("-O3"))) __attribute__ ((section (".ram_code"))) pms7003_rx_irq_handler(void) {
	while(!XMC_USIC_CH_RXFIFO_IsEmpty(PMS7003_USIC)) {
		// Instead of ringbuffer_add() we add the byte to the buffer
		// by hand.
		//
		// We need to save the low watermark calculation overhead.

		uint16_t new_end = *pms7003_ringbuffer_rx_end + 1;

		if(new_end >= *pms7003_ringbuffer_rx_size) {
			new_end = 0;
		}

		if(new_end == *pms7003_ringbuffer_rx_start) {
			// In the case of an overrun we read the byte and throw it away.
			volatile uint8_t __attribute__((unused)) _  = PMS7003_USIC->OUTR;
		} else {
			pms7003_ringbuffer_rx_buffer[*pms7003_ringbuffer_rx_end] = PMS7003_USIC->OUTR;
			*pms7003_ringbuffer_rx_end = new_end;
		}
	}

}

void __attribute__((optimize("-O3"))) __attribute__ ((section (".ram_code"))) pms7003_tx_irq_handler(void) {
	while(!XMC_USIC_CH_TXFIFO_IsFull(PMS7003_USIC)) {
		// TX FIFO is not full, more data can be loaded on the FIFO from the ring buffer.
		uint8_t data;
		if(!ringbuffer_get(&pms7003.ringbuffer_tx, &data)) {
			// No more data to TX from ringbuffer, disable TX interrupt.
			XMC_USIC_CH_TXFIFO_DisableEvent(PMS7003_USIC, XMC_USIC_CH_TXFIFO_EVENT_CONF_STANDARD);

			return;
		}

		PMS7003_USIC->IN[0] = data;
	}
}

void pms7003_print_frame(void) {
	uartbb_printf("Frame (err ch %u, err fr %u):\n\r", pms7003.checksum_error_counter, pms7003.framing_error_counter);
	uartbb_printf("* conc_pm1_0_cf1: %d\n\r", pms7003.frame.conc_pm1_0_cf1);
	uartbb_printf("* conc_pm2_5_cf1: %d\n\r", pms7003.frame.conc_pm2_5_cf1);
	uartbb_printf("* conc_pm10_0_cf1: %d\n\r", pms7003.frame.conc_pm10_0_cf1);
	uartbb_printf("* conc_pm1_0_amb: %d\n\r", pms7003.frame.conc_pm1_0_amb);
	uartbb_printf("* conc_pm2_5_amb: %d\n\r", pms7003.frame.conc_pm2_5_amb);
	uartbb_printf("* conc_pm10_0_amb: %d\n\r", pms7003.frame.conc_pm10_0_amb);
	uartbb_printf("* raw_gt0_3um: %d\n\r", pms7003.frame.raw_gt0_3um);
	uartbb_printf("* raw_gt0_5um: %d\n\r", pms7003.frame.raw_gt0_5um);
	uartbb_printf("* raw_gt1_0um: %d\n\r", pms7003.frame.raw_gt1_0um);
	uartbb_printf("* raw_gt2_5um: %d\n\r", pms7003.frame.raw_gt2_5um);
	uartbb_printf("* raw_gt5_0um: %d\n\r", pms7003.frame.raw_gt5_0um);
	uartbb_printf("* raw_gt10_0um: %d\n\r", pms7003.frame.raw_gt10_0um);
	uartbb_printf("* version: %d\n\r", pms7003.frame.version);
	uartbb_printf("* error_code: %d\n\r", pms7003.frame.error_code);
	uartbb_printf("* checksum: %d\n\r", pms7003.frame.checksum);
	uartbb_printf("\n\r");
}

void pms7003_handle_data(uint8_t data) {
	static uint16_t checksum = 0;
	static uint32_t data_counter = 0;

	uint8_t *frame_buffer = (uint8_t*)&pms7003.frame;
	if(data_counter == 0) {                   // sync first byte (0x42)
		if(data == 0x42) {
			data_counter++;
			checksum += data;
		} else {
			data_counter = 0;
			checksum = 0;
		}
	} else if(data_counter == 1) {            // sync second byte (0x4D)
		if(data == 0x4D) {
			data_counter++;
			checksum += data;
		} else {
			data_counter = 0;
			checksum = 0;
		}
	} else if(data_counter == 2) {            // length (MSB)
		if(data == 0) {
			data_counter++;
			checksum += data;
		} else {
			logw("Unexpected length MSB: %d\n\r", data);
			data_counter = 0;
			checksum = 0;
			pms7003.framing_error_counter++;
		}
	} else if(data_counter == 3) {            // length (LSB)
		if(data == 28) {
			data_counter++;
			checksum += data;
		} else {
			logw("Unexpected length LSB: %d\n\r", data);
			data_counter = 0;
			checksum = 0;
			pms7003.framing_error_counter++;
		}
	} else {                                  // data
		if(data_counter == 28 || data_counter == 29) {
			frame_buffer[data_counter-4] = data;
		} else {
			if(data_counter & 1) {
				frame_buffer[data_counter-1-4] = data;
			} else {
				frame_buffer[data_counter+1-4] = data;
			}
		}
		data_counter++;

		if(data_counter < 30) {
			checksum += data;
		}
	}

	if(data_counter == 32) {
		if(checksum == pms7003.frame.checksum) {
			// can only be valid if sensor is active
			/*modbus_input.pms_valid = modbus_coils.pms_active;
			modbus_input.pms_concentration_pm10 = pms7003_frame.conc_pm1_0_amb;
			modbus_input.pms_concentration_pm25 = pms7003_frame.conc_pm2_5_amb;
			modbus_input.pms_concentration_pm100 = pms7003_frame.conc_pm10_0_amb;
			modbus_input.pms_particles_03um = pms7003_frame.raw_gt0_3um;
			modbus_input.pms_particles_05um = pms7003_frame.raw_gt0_5um;
			modbus_input.pms_particles_10um = pms7003_frame.raw_gt1_0um;
			modbus_input.pms_particles_25um = pms7003_frame.raw_gt2_5um;
			modbus_input.pms_particles_50um = pms7003_frame.raw_gt5_0um;
			modbus_input.pms_particles_100um = pms7003_frame.raw_gt10_0um;

			pms7003_last_valid_time = system_timer_get_ms();*/
			//pms7003_print_frame();
		} else {
			logw("Unexpected: checksum %d != %d\n\r", checksum, pms7003.frame.checksum);
			pms7003.checksum_error_counter++;
		}

		data_counter = 0;
		checksum = 0;
	}
}

void pms7003_init_hardware(void) {
	// TX pin configuration
	const XMC_GPIO_CONFIG_t tx_pin_config = {
		.mode             = PMS7003_TX_PIN_AF,
		.output_level     = XMC_GPIO_OUTPUT_LEVEL_HIGH
	};

	// RX pin configuration
	const XMC_GPIO_CONFIG_t rx_pin_config = {
		.mode             = XMC_GPIO_MODE_INPUT_PULL_UP,
		.input_hysteresis = XMC_GPIO_INPUT_HYSTERESIS_STANDARD
	};

	// Configure  pins
	XMC_GPIO_Init(PMS7003_TX_PIN, &tx_pin_config);
	XMC_GPIO_Init(PMS7003_RX_PIN, &rx_pin_config);

	// Initialize USIC channel in UART master mode
	// USIC channel configuration
	XMC_UART_CH_CONFIG_t config;
	config.oversampling = 16;
	config.frame_length = 8;
	config.baudrate     = 9600;
	config.stop_bits    = 1;
	config.data_bits    = 8;
	config.parity_mode  = XMC_USIC_CH_PARITY_MODE_NONE;
	XMC_UART_CH_Init(PMS7003_USIC, &config);

	// Set input source path
	XMC_UART_CH_SetInputSource(PMS7003_USIC, PMS7003_RX_INPUT, PMS7003_RX_SOURCE);

	// Configure transmit FIFO
	XMC_USIC_CH_TXFIFO_Configure(PMS7003_USIC, 32, XMC_USIC_CH_FIFO_SIZE_32WORDS, 16);

	// Configure receive FIFO
	XMC_USIC_CH_RXFIFO_Configure(PMS7003_USIC, 0, XMC_USIC_CH_FIFO_SIZE_32WORDS, 16);

	// Set service request for tx FIFO transmit interrupt
//	XMC_USIC_CH_TXFIFO_SetInterruptNodePointer(PMS7003_USIC, XMC_USIC_CH_TXFIFO_INTERRUPT_NODE_POINTER_STANDARD, PMS7003_SERVICE_REQUEST_TX);

	// Set service request for rx FIFO receive interrupt
	XMC_USIC_CH_RXFIFO_SetInterruptNodePointer(PMS7003_USIC, XMC_USIC_CH_RXFIFO_INTERRUPT_NODE_POINTER_STANDARD, PMS7003_SERVICE_REQUEST_RX);
	XMC_USIC_CH_RXFIFO_SetInterruptNodePointer(PMS7003_USIC, XMC_USIC_CH_RXFIFO_INTERRUPT_NODE_POINTER_ALTERNATE, PMS7003_SERVICE_REQUEST_RX);

	// Set priority and enable NVIC node for transmit interrupt
//	NVIC_SetPriority((IRQn_Type)PMS7003_IRQ_TX, PMS7003_IRQ_TX_PRIORITY);
//	XMC_SCU_SetInterruptControl(PMS7003_IRQ_TX, PMS7003_IRQCTRL_TX);
//	NVIC_EnableIRQ((IRQn_Type)PMS7003_IRQ_TX);

	// Set priority and enable NVIC node for receive interrupt
	NVIC_SetPriority((IRQn_Type)PMS7003_IRQ_RX, PMS7003_IRQ_RX_PRIORITY);
	XMC_SCU_SetInterruptControl(PMS7003_IRQ_RX, PMS7003_IRQCTRL_RX);
	NVIC_EnableIRQ((IRQn_Type)PMS7003_IRQ_RX);

	// Start UART
	XMC_UART_CH_Start(PMS7003_USIC);

	XMC_USIC_CH_EnableEvent(PMS7003_USIC, XMC_USIC_CH_EVENT_ALTERNATIVE_RECEIVE);
	XMC_USIC_CH_RXFIFO_EnableEvent(PMS7003_USIC, XMC_USIC_CH_RXFIFO_EVENT_CONF_STANDARD | XMC_USIC_CH_RXFIFO_EVENT_CONF_ALTERNATE);
}

void pms7003_init_buffer(void) {
	// Disable interrupts so we can't accidentally
	// receive ringbuffer_adds in between a re-init
	NVIC_DisableIRQ((IRQn_Type)PMS7003_IRQ_TX);
	NVIC_DisableIRQ((IRQn_Type)PMS7003_IRQ_RX);
	__DSB();
	__ISB();

	// Initialize pms7003 buffer
	memset(pms7003.buffer_rx, 0, PMS7003_BUFFER_SIZE);

	ringbuffer_init(&pms7003.ringbuffer_rx, PMS7003_BUFFER_SIZE, pms7003.buffer_rx);
	ringbuffer_init(&pms7003.ringbuffer_tx, PMS7003_BUFFER_SIZE, pms7003.buffer_tx);

	NVIC_EnableIRQ((IRQn_Type)PMS7003_IRQ_TX);
	NVIC_EnableIRQ((IRQn_Type)PMS7003_IRQ_RX);
}

void pms7003_init(void) {
	XMC_GPIO_CONFIG_t pin_config = {
		.mode             = XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
		.output_level     = XMC_GPIO_OUTPUT_LEVEL_HIGH
	};

	XMC_GPIO_Init(PMS7003_SET_PIN, &pin_config);
	XMC_GPIO_Init(PMS7003_RESET_PIN, &pin_config);

	pms7003_init_buffer();
	pms7003_init_hardware();
}

void pms7003_tick(void) {
	while(ringbuffer_get_used(&pms7003.ringbuffer_rx) > 0) {
		uint8_t data = 0;
		ringbuffer_get(&pms7003.ringbuffer_rx, &data);
		pms7003_handle_data(data);
	}
}
