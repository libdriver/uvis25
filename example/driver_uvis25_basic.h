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
 * @file      driver_uvis25_basic.h
 * @brief     driver uvis25 basic header file
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

#ifndef DRIVER_UVIS25_BASIC_H
#define DRIVER_UVIS25_BASIC_H

#include "driver_uvis25_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup uvis25_example_driver uvis25 example driver function
 * @brief    uvis25 example driver modules
 * @ingroup  uvis25_driver
 * @{
 */

/**
 * @brief uvis25 basic example default definition
 */
#define UVIS25_BASIC_DEFAULT_IIC                        UVIS25_BOOL_TRUE               /**< enable iic */
#define UVIS25_BASIC_DEFAULT_SPI_WIRE                   UVIS25_SPI_WIRE_4              /**< 4 wire spi */
#define UVIS25_BASIC_DEFAULT_BOOT_MODE                  UVIS25_BOOT_NORMAL_MODE        /**< normal boot mode */
#define UVIS25_BASIC_DEFAULT_BLOCK_DATA_UPDATE          UVIS25_BOOL_FALSE              /**< disable block data update */

/**
 * @brief     basic example init
 * @param[in] interface is the chip interface
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t uvis25_basic_init(uvis25_interface_t interface);

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t uvis25_basic_deinit(void);

/**
 * @brief      basic example read
 * @param[out] *uv points to a uv index buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t uvis25_basic_read(float *uv);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
