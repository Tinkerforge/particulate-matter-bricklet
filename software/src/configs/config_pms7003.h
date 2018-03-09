/* particulate-matter-bricklet
 * Copyright (C) 2018 Olaf Lüke <olaf@tinkerforge.com>
 *
 * config_pms7003.h: Config for PMS7003 sensor
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

#ifndef CONFIG_PMS7003_H
#define CONFIG_PMS7003_H

#include "xmc_common.h"
#include "xmc_gpio.h"

#define PMS7003_SET_PIN             P2_10
#define PMS7003_RESET_PIN           P2_11

#define PMS7003_USIC_CHANNEL        USIC1_CH1
#define PMS7003_USIC                XMC_UART1_CH1

#define PMS7003_RX_PIN              P2_12
#define PMS7003_RX_INPUT            XMC_USIC_CH_INPUT_DX0
#define PMS7003_RX_SOURCE           0b010 // DX0C

#define PMS7003_TX_PIN              P2_13
#define PMS7003_TX_PIN_AF           (XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT7 | P2_13_AF_U1C1_DOUT0)

#define PMS7003_SERVICE_REQUEST_RX  2  // receive
#define PMS7003_SERVICE_REQUEST_TX  3  // transfer

#define PMS7003_IRQ_RX              11
#define PMS7003_IRQ_RX_PRIORITY     0
#define PMS7003_IRQCTRL_RX          XMC_SCU_IRQCTRL_USIC1_SR2_IRQ11

#define PMS7003_IRQ_TX              12
#define PMS7003_IRQ_TX_PRIORITY     1
#define PMS7003_IRQCTRL_TX          XMC_SCU_IRQCTRL_USIC1_SR3_IRQ12

#endif
