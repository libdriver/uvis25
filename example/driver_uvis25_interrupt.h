/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      driver_uvis25_interrupt.h
 * @brief     driver uvis25 interrupt header file
 * @version   2.0.0
 * @author    Shifeng Li
 * @date      2021-03-24
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2021/03/24  <td>2.0      <td>Shifeng Li  <td>format the code
 * <tr><td>2020/12/06  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#ifndef DRIVER_UVIS25_INTERRUPT_H
#define DRIVER_UVIS25_INTERRUPT_H

#include "driver_uvis25_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @addtogroup uvis25_example_driver
 * @{
 */

/**
 * @brief uvis25 interrupt example default definition
 */
#define UVIS25_INTERRUPT_DEFAULT_IIC                        UVIS25_BOOL_TRUE                               /* enable iic */
#define UVIS25_INTERRUPT_DEFAULT_SPI_WIRE                   UVIS25_SPI_WIRE_4                              /* 4 wire spi */
#define UVIS25_INTERRUPT_DEFAULT_BOOT_MODE                  UVIS25_BOOT_NORMAL_MODE                        /* normal boot mode */
#define UVIS25_INTERRUPT_DEFAULT_BLOCK_DATA_UPDATE          UVIS25_BOOL_FALSE                              /* disable block data update */
#define UVIS25_INTERRUPT_DEFAULT_INTERRUPT_ACTIVE_LEVEL     UVIS25_INTERRUPT_ACTIVE_LEVEL_LOWER            /* low level */
#define UVIS25_INTERRUPT_DEFAULT_INTERRUPT_PIN_TYPE         UVIS25_INTERRUPT_PIN_TYPE_PUSH_PULL            /* set push-pull */
#define UVIS25_INTERRUPT_DEFAULT_INTERRUPT                  UVIS25_BOOL_TRUE                               /* enable interrupt */
#define UVIS25_INTERRUPT_DEFAULT_LATCH_INTERRUPT            UVIS25_BOOL_FALSE                              /* disable latch interrupt */
#define UVIS25_INTERRUPT_DEFAULT_HIGH_THRESHOLD_INTERRUPT   UVIS25_BOOL_TRUE                               /* enable high threshold interrupt */
#define UVIS25_INTERRUPT_DEFAULT_LOW_THRESHOLD_INTERRUPT    UVIS25_BOOL_TRUE                               /* enable low threshold interrupt */

/**
 * @brief  interrupt example irq handler
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t uvis25_interrupt_irq_handler(void);

/**
 * @brief     interrupt example init
 * @param[in] interface is the chip interface
 * @param[in] mode is the interrupt mode type
 * @param[in] uv_threshold is the uv threshold
 * @param[in] *callback points to a callback function
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t uvis25_interrupt_init(uvis25_interface_t interface, uvis25_interrupt_type_t mode, float uv_threshold,
                              void (*callback)(uint8_t type));

/**
 * @brief  basic interrupt deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t uvis25_interrupt_deinit(void);

/**
 * @brief      interrupt example read
 * @param[out] *uv points to a uv index buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t uvis25_interrupt_read(float *uv);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
