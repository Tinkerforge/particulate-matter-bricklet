/* particulate-matter-bricklet
 * Copyright (C) 2018 Olaf Lüke <olaf@tinkerforge.com>
 *
 * pms7003.h: Driver for PMS7003 sensor
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

#ifndef PMS7003_H
#define PMS7003_H

#include <stdint.h>

#include "bricklib2/utility/ringbuffer.h"

#define PMS7003_BUFFER_SIZE 2048

typedef struct {
    uint16_t conc_pm1_0_cf1;
    uint16_t conc_pm2_5_cf1;
    uint16_t conc_pm10_0_cf1;
    uint16_t conc_pm1_0_amb;
    uint16_t conc_pm2_5_amb;
    uint16_t conc_pm10_0_amb;
    uint16_t raw_gt0_3um;
    uint16_t raw_gt0_5um;
    uint16_t raw_gt1_0um;
    uint16_t raw_gt2_5um;
    uint16_t raw_gt5_0um;
    uint16_t raw_gt10_0um;
    uint8_t  version;
    uint8_t  error_code;
    uint16_t checksum;
} __attribute__((__packed__)) PMS7003Frame;

typedef struct {
	uint8_t buffer_rx[PMS7003_BUFFER_SIZE];
	uint8_t buffer_tx[PMS7003_BUFFER_SIZE];
	Ringbuffer ringbuffer_tx;
	Ringbuffer ringbuffer_rx;

	PMS7003Frame frame;
	uint32_t framing_error_counter;
	uint32_t checksum_error_counter;
} PMS7003;

extern PMS7003 pms7003;

void pms7003_init(void);
void pms7003_tick(void);

#endif
