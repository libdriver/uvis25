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
 * @file      driver_uvis25.h
 * @brief     driver uvis25 header file
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

#ifndef DRIVER_UVIS25_H
#define DRIVER_UVIS25_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup uvis25_driver uvis25 driver function
 * @brief    uvis25 driver modules
 * @{
 */

/**
 * @addtogroup uvis25_base_driver
 * @{
 */

/**
 * @brief uvis25 bool enumeration definition
 */
typedef enum
{
    UVIS25_BOOL_FALSE = 0x00,        /**< disable function */
    UVIS25_BOOL_TRUE  = 0x01,        /**< enable function */
} uvis25_bool_t;

/**
 * @brief uvis25 spi wire enumeration definition
 */
typedef enum
{
    UVIS25_SPI_WIRE_4 = 0x00,        /**< 4 wire */
    UVIS25_SPI_WIRE_3 = 0x01,        /**< 3 wire */
} uvis25_spi_wire_t;

/**
 * @brief uvis25 interface enumeration definition
 */
typedef enum
{
    UVIS25_INTERFACE_IIC = 0x00,        /**< iic interface */
    UVIS25_INTERFACE_SPI = 0x01,        /**< spi interface */
} uvis25_interface_t;

/**
 * @brief uvis25 boot enumeration definition
 */
typedef enum
{
    UVIS25_BOOT_NORMAL_MODE           = 0x00,        /**< normal mode */
    UVIS25_BOOT_REBOOT_MEMORY_CONTENT = 0x01,        /**< reboot memory content */
} uvis25_boot_mode_t;

/**
 * @}
 */

/**
 * @addtogroup uvis25_interrupt_driver
 * @{
 */

/**
 * @brief uvis25 interrupt active level enumeration definition
 */
typedef enum
{
    UVIS25_INTERRUPT_ACTIVE_LEVEL_HIGHER = 0x00,        /**< active higher level */
    UVIS25_INTERRUPT_ACTIVE_LEVEL_LOWER  = 0x01,        /**< active lower level */
} uvis25_interrupt_active_level_t;

/**
 * @brief uvis25 interrupt pin type enumeration definition
 */
typedef enum
{
    UVIS25_INTERRUPT_PIN_TYPE_PUSH_PULL  = 0x00,        /**< push pull type */
    UVIS25_INTERRUPT_PIN_TYPE_OPEN_DRAIN = 0x01,        /**< open drain type */
} uvis25_interrupt_pin_type_t;

/**
 * @brief uvis25 interrupt type enumeration definition
 */
typedef enum
{
    UVIS25_INTERRUPT_TYPE_DATA_READY        = 0x00,        /**< data ready */
    UVIS25_INTERRUPT_TYPE_UV_INDEX_HIGH     = 0x01,        /**< uv index high */
    UVIS25_INTERRUPT_TYPE_UV_INDEX_LOW      = 0x02,        /**< uv index low */
    UVIS25_INTERRUPT_TYPE_UV_INDEX_HIGH_LOW = 0x03,        /**< uv index high or low */
} uvis25_interrupt_type_t;

/**
 * @brief uvis25 interrupt status enumeration definition
 */
typedef enum
{
    UVIS25_INTERRUPT_ACTIVE = 0x00,        /**< active */
    UVIS25_INTERRUPT_HIGHER = 0x01,        /**< higher */
    UVIS25_INTERRUPT_LOWER  = 0x02,        /**< lower */
} uvis25_interrupt_t;

/**
 * @}
 */

/**
 * @addtogroup uvis25_base_driver
 * @{
 */

/**
 * @brief uvis25 handle structure definition
 */
typedef struct uvis25_handle_s
{
    uint8_t (*iic_init)(void);                                                          /**< point to an iic_init function address */
    uint8_t (*iic_deinit)(void);                                                        /**< point to an iic_deinit function address */
    uint8_t (*iic_read)(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);         /**< point to an iic_read function address */
    uint8_t (*iic_write)(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);        /**< point to an iic_write function address */
    uint8_t (*spi_init)(void);                                                          /**< point to a spi_init function address */
    uint8_t (*spi_deinit)(void);                                                        /**< point to a spi_deinit function address */
    uint8_t (*spi_read)(uint8_t reg, uint8_t *buf, uint16_t len);                       /**< point to a spi_read function address */
    uint8_t (*spi_write)(uint8_t reg, uint8_t *buf, uint16_t len);                      /**< point to a spi_write function address */
    void (*receive_callback)(uint8_t type);                                             /**< point to a receive_callback function address */
    void (*delay_ms)(uint32_t ms);                                                      /**< point to a delay_ms function address */
    void (*debug_print)(const char *const fmt, ...);                                    /**< point to a debug_print function address */
    uint8_t inited;                                                                     /**< inited flag */
    uint8_t iic_spi;                                                                    /**< iic spi interface type */
} uvis25_handle_t;

/**
 * @brief uvis25 information structure definition
 */
typedef struct uvis25_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                 /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} uvis25_info_t;

/**
 * @}
 */

/**
 * @defgroup uvis25_link_driver uvis25 link driver function
 * @brief    uvis25 link driver modules
 * @ingroup  uvis25_driver
 * @{
 */

/**
 * @brief     initialize uvis25_handle_t structure
 * @param[in] HANDLE points to a uvis25 handle structure
 * @param[in] STRUCTURE is uvis25_handle_t
 * @note      none
 */
#define DRIVER_UVIS25_LINK_INIT(HANDLE, STRUCTURE)         memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link iic_init function
 * @param[in] HANDLE points to a uvis25 handle structure
 * @param[in] FUC points to an iic_init function address
 * @note      none
 */
#define DRIVER_UVIS25_LINK_IIC_INIT(HANDLE, FUC)          (HANDLE)->iic_init = FUC

/**
 * @brief     link iic_deinit function
 * @param[in] HANDLE points to a uvis25 handle structure
 * @param[in] FUC points to an iic_deinit function address
 * @note      none
 */
#define DRIVER_UVIS25_LINK_IIC_DEINIT(HANDLE, FUC)        (HANDLE)->iic_deinit = FUC

/**
 * @brief     link iic_deinit function
 * @param[in] HANDLE points to a uvis25 handle structure
 * @param[in] FUC points to an iic_read function address
 * @note      none
 */
#define DRIVER_UVIS25_LINK_IIC_READ(HANDLE, FUC)          (HANDLE)->iic_read = FUC

/**
 * @brief     link iic_write function
 * @param[in] HANDLE points to a uvis25 handle structure
 * @param[in] FUC points to an iic_write function address
 * @note      none
 */
#define DRIVER_UVIS25_LINK_IIC_WRITE(HANDLE, FUC)         (HANDLE)->iic_write = FUC

/**
 * @brief     link spi_init function
 * @param[in] HANDLE points to a uvis25 handle structure
 * @param[in] FUC points to a spi_init function address
 * @note      none
 */
#define DRIVER_UVIS25_LINK_SPI_INIT(HANDLE, FUC)          (HANDLE)->spi_init = FUC

/**
 * @brief     link spi_deinit function
 * @param[in] HANDLE points to a uvis25 handle structure
 * @param[in] FUC points to a spi_deinit function address
 * @note      none
 */
#define DRIVER_UVIS25_LINK_SPI_DEINIT(HANDLE, FUC)        (HANDLE)->spi_deinit = FUC

/**
 * @brief     link spi_read function
 * @param[in] HANDLE points to a uvis25 handle structure
 * @param[in] FUC points to a spi_read function address
 * @note      none
 */
#define DRIVER_UVIS25_LINK_SPI_READ(HANDLE, FUC)          (HANDLE)->spi_read = FUC

/**
 * @brief     link spi_write function
 * @param[in] HANDLE points to a uvis25 handle structure
 * @param[in] FUC points to a spi_write function address
 * @note      none
 */
#define DRIVER_UVIS25_LINK_SPI_WRITE(HANDLE, FUC)         (HANDLE)->spi_write = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE points to a uvis25 handle structure
 * @param[in] FUC points to a delay_ms function address
 * @note      none
 */
#define DRIVER_UVIS25_LINK_DELAY_MS(HANDLE, FUC)          (HANDLE)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE points to a uvis25 handle structure
 * @param[in] FUC points to a debug_print function address
 * @note      none
 */
#define DRIVER_UVIS25_LINK_DEBUG_PRINT(HANDLE, FUC)       (HANDLE)->debug_print = FUC

/**
 * @brief     link receive_callback function
 * @param[in] HANDLE points to a uvis25 handle structure
 * @param[in] FUC points to a receive_callback function address
 * @note      none
 */
#define DRIVER_UVIS25_LINK_RECEIVE_CALLBACK(HANDLE, FUC)  (HANDLE)->receive_callback = FUC

/**
 * @}
 */

/**
 * @defgroup uvis25_base_driver uvis25 base driver function
 * @brief    uvis25 base driver modules
 * @ingroup  uvis25_driver
 * @{
 */

/**
 * @brief      get chip's information
 * @param[out] *info points to a uvis25 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t uvis25_info(uvis25_info_t *info);

/**
 * @brief     initialize the chip
 * @param[in] *handle points to a uvis25 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic or spi initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 * @note      none
 */
uint8_t uvis25_init(uvis25_handle_t *handle);

/**
 * @brief     close the chip
 * @param[in] *handle points to a uvis25 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic or spi deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t uvis25_deinit(uvis25_handle_t *handle);

/**
 * @brief      read data once
 * @param[in]  *handle points to a uvis25 handle structure
 * @param[out] *raw points to a raw data buffer
 * @param[out] *uv points to a uv index buffer
 * @return     status code
 *             - 0 success
 *             - 1 single read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t uvis25_single_read(uvis25_handle_t *handle, uint8_t *raw, float *uv);

/**
 * @brief     start reading
 * @param[in] *handle points to a uvis25 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 start continuous read failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t uvis25_start_continuous_read(uvis25_handle_t *handle);

/**
 * @brief     stop reading
 * @param[in] *handle points to a uvis25 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 stop continuous read failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t uvis25_stop_continuous_read(uvis25_handle_t *handle);

/**
 * @brief      read data continuously
 * @param[in]  *handle points to a uvis25 handle structure
 * @param[out] *raw points to a raw data buffer
 * @param[out] *uv points to a uv index buffer
 * @return     status code
 *             - 0 success
 *             - 1 continuous read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t uvis25_continuous_read(uvis25_handle_t *handle, uint8_t *raw, float *uv);

/**
 * @brief     set the chip interface
 * @param[in] *handle points to a uvis25 handle structure
 * @param[in] interface is the chip interface
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t uvis25_set_interface(uvis25_handle_t *handle, uvis25_interface_t interface);

/**
 * @brief      get the chip interface
 * @param[in]  *handle points to a uvis25 handle structure
 * @param[out] *interface points to a chip interface
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t uvis25_get_interface(uvis25_handle_t *handle, uvis25_interface_t *interface);

/**
 * @brief     irq handler
 * @param[in] *handle points to a uvis25 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t uvis25_irq_handler(uvis25_handle_t *handle);

/**
 * @brief     enable or disable blocking data update 
 * @param[in] *handle points to a uvis25 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set block data update failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t uvis25_set_block_data_update(uvis25_handle_t *handle, uvis25_bool_t enable);

/**
 * @brief      get blocking data update status 
 * @param[in]  *handle points to a uvis25 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get block data update failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t uvis25_get_block_data_update(uvis25_handle_t *handle, uvis25_bool_t *enable);

/**
 * @brief     set the boot mode
 * @param[in] *handle points to a uvis25 handle structure
 * @param[in] mode is the boot mode
 * @return    status code
 *            - 0 success
 *            - 1 set boot failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t uvis25_set_boot(uvis25_handle_t *handle, uvis25_boot_mode_t mode);

/**
 * @brief      get the boot mode
 * @param[in]  *handle points to a uvis25 handle structure
 * @param[out] *mode points to a boot mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get boot failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t uvis25_get_boot(uvis25_handle_t *handle, uvis25_boot_mode_t *mode);

/**
 * @brief     enable or disable the chip iic
 * @param[in] *handle points to a uvis25 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set iic failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t uvis25_set_iic(uvis25_handle_t *handle, uvis25_bool_t enable);

/**
 * @brief     enable or disable the chip iic
 * @param[in] *handle points to a uvis25 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set iic failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t uvis25_set_iic(uvis25_handle_t *handle, uvis25_bool_t enable);

/**
 * @brief      get the chip iic status
 * @param[in]  *handle points to a uvis25 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get iic failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t uvis25_get_iic(uvis25_handle_t *handle, uvis25_bool_t *enable);

/**
 * @brief      get the chip iic status
 * @param[in]  *handle points to a uvis25 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get iic failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t uvis25_get_iic(uvis25_handle_t *handle, uvis25_bool_t *enable);

/**
 * @brief     set the spi wire
 * @param[in] *handle points to a uvis25 handle structure
 * @param[in] wire is the spi wire
 * @return    status code
 *            - 0 success
 *            - 1 set spi wire failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t uvis25_set_spi_wire(uvis25_handle_t *handle, uvis25_spi_wire_t wire);

/**
 * @brief      get the spi wire
 * @param[in]  *handle points to a uvis25 handle structure
 * @param[out] *wire points to a spi wire buffer
 * @return     status code
 *             - 0 success
 *             - 1 get spi wire failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t uvis25_get_spi_wire(uvis25_handle_t *handle, uvis25_spi_wire_t *wire);

/**
 * @}
 */

/**
 * @defgroup uvis25_interrupt_driver uvis25 interrupt driver function
 * @brief    uvis25 interrupt driver modules
 * @ingroup  uvis25_driver
 * @{
 */

/**
 * @brief     set the interrupt active level
 * @param[in] *handle points to a uvis25 handle structure
 * @param[in] level is the interrupt active level
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt active level failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t uvis25_set_interrupt_active_level(uvis25_handle_t *handle, uvis25_interrupt_active_level_t level);

/**
 * @brief      get the interrupt active level
 * @param[in]  *handle points to a uvis25 handle structure
 * @param[out] *level points to an interrupt active level buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt active level failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t uvis25_get_interrupt_active_level(uvis25_handle_t *handle, uvis25_interrupt_active_level_t *level);

/**
 * @brief     set the interrupt pin type
 * @param[in] *handle points to a uvis25 handle structure
 * @param[in] pin_type is the interrupt pin type
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt pin type failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t uvis25_set_interrupt_pin_type(uvis25_handle_t *handle, uvis25_interrupt_pin_type_t pin_type);

/**
 * @brief      get the interrupt pin type
 * @param[in]  *handle points to a uvis25 handle structure
 * @param[out] *pin_type points to an interrupt pin type buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt pin type failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t uvis25_get_interrupt_pin_type(uvis25_handle_t *handle, uvis25_interrupt_pin_type_t *pin_type);

/**
 * @brief     set the interrupt type
 * @param[in] *handle points to a uvis25 handle structure
 * @param[in] type is the interrupt type
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt type failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t uvis25_set_interrupt_type(uvis25_handle_t *handle, uvis25_interrupt_type_t type);

/**
 * @brief      get the interrupt type
 * @param[in]  *handle points to a uvis25 handle structure
 * @param[out] *type points to an interrupt type buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt type failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t uvis25_get_interrupt_type(uvis25_handle_t *handle, uvis25_interrupt_type_t *type);

/**
 * @brief     enable or disable the chip interrupt
 * @param[in] *handle points to a uvis25 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t uvis25_set_interrupt(uvis25_handle_t *handle, uvis25_bool_t enable);

/**
 * @brief      get the chip interrupt status
 * @param[in]  *handle points to a uvis25 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t uvis25_get_interrupt(uvis25_handle_t *handle, uvis25_bool_t *enable);

/**
 * @brief     enable or disable latching interrupt
 * @param[in] *handle points to a uvis25 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set latch interrupt failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t uvis25_set_latch_interrupt(uvis25_handle_t *handle, uvis25_bool_t enable);

/**
 * @brief      get the latching interrupt status
 * @param[in]  *handle points to a uvis25 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get latch interrupt failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t uvis25_get_latch_interrupt(uvis25_handle_t *handle, uvis25_bool_t *enable);

/**
 * @brief     enable or disable the low threshold interrupt
 * @param[in] *handle points to a uvis25 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt low threshold failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t uvis25_set_interrupt_low_threshold(uvis25_handle_t *handle, uvis25_bool_t enable);

/**
 * @brief      get the low threshold interrupt status
 * @param[in]  *handle points to a uvis25 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt low threshold failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t uvis25_get_interrupt_low_threshold(uvis25_handle_t *handle, uvis25_bool_t *enable);

/**
 * @brief     enable or disable the high threshold interrupt
 * @param[in] *handle points to a uvis25 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt high threshold failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t uvis25_set_interrupt_high_threshold(uvis25_handle_t *handle, uvis25_bool_t enable);

/**
 * @brief      get the high threshold interrupt status
 * @param[in]  *handle points to a uvis25 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt high threshold failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t uvis25_get_interrupt_high_threshold(uvis25_handle_t *handle, uvis25_bool_t *enable);

/**
 * @brief     set the interrupt threshold
 * @param[in] *handle points to a uvis25 handle structure
 * @param[in] threshold is the interrupt threshold
 * @return    status code
 *            - 0 success
 *            - 1 set threshold failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t uvis25_set_threshold(uvis25_handle_t *handle, uint8_t threshold);

/**
 * @brief      get the interrupt threshold
 * @param[in]  *handle points to a uvis25 handle structure
 * @param[out] *threshold points to an interrupt threshold buffer
 * @return     status code
 *             - 0 success
 *             - 1 get threshold failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t uvis25_get_threshold(uvis25_handle_t *handle, uint8_t *threshold);

/**
 * @brief      convert a uv index to a raw register data
 * @param[in]  *handle points to a uvis25 handle structure
 * @param[in]  uv is the uv index
 * @param[out] *reg points to a raw register data
 * @return     status code
 *             - 0 success
 *             - 1 threshold convert to register failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t uvis25_threshold_convert_to_register(uvis25_handle_t *handle, float uv, uint8_t *reg);

/**
 * @brief      convert a raw register data to a converted uv index
 * @param[in]  *handle points to a uvis25 handle structure
 * @param[in]  reg is the raw register data
 * @param[out] *uv points to a uv index buffer
 * @return     status code
 *             - 0 success
 *             - 1 threshold convert to data failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t uvis25_threshold_convert_to_data(uvis25_handle_t *handle, uint8_t reg, float *uv);

/**
 * @}
 */

/**
 * @defgroup uvis25_extend_driver uvis25 extend driver function
 * @brief    uvis25 extend driver modules
 * @ingroup  uvis25_driver
 * @{
 */

/**
 * @brief     set the chip register
 * @param[in] *handle points to a uvis25 handle structure
 * @param[in] reg is the iic register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data buffer length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t uvis25_set_reg(uvis25_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len);

/**
 * @brief      get the chip register
 * @param[in]  *handle points to a uvis25 handle structure
 * @param[in]  reg is the iic register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the data buffer length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t uvis25_get_reg(uvis25_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
