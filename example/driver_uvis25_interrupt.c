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
 * @file      driver_uvis25_interrupt.c
 * @brief     driver uvis25 interrupt source file
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

#include "driver_uvis25_interrupt.h"

static uvis25_handle_t gs_handle;        /**< uvis25 handle */

/**
 * @brief  interrupt example irq handler
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t uvis25_interrupt_irq_handler(void)
{
    if (uvis25_irq_handler(&gs_handle) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

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
                              void (*callback)(uint8_t type))
{
    uint8_t res;
    uint8_t reg;

    /* link functions */
    DRIVER_UVIS25_LINK_INIT(&gs_handle, uvis25_handle_t);
    DRIVER_UVIS25_LINK_IIC_INIT(&gs_handle, uvis25_interface_iic_init);
    DRIVER_UVIS25_LINK_IIC_DEINIT(&gs_handle, uvis25_interface_iic_deinit);
    DRIVER_UVIS25_LINK_IIC_READ(&gs_handle, uvis25_interface_iic_read);
    DRIVER_UVIS25_LINK_IIC_WRITE(&gs_handle, uvis25_interface_iic_write);
    DRIVER_UVIS25_LINK_SPI_INIT(&gs_handle, uvis25_interface_spi_init);
    DRIVER_UVIS25_LINK_SPI_DEINIT(&gs_handle, uvis25_interface_spi_deinit);
    DRIVER_UVIS25_LINK_SPI_READ(&gs_handle, uvis25_interface_spi_read);
    DRIVER_UVIS25_LINK_SPI_WRITE(&gs_handle, uvis25_interface_spi_write);
    DRIVER_UVIS25_LINK_DELAY_MS(&gs_handle, uvis25_interface_delay_ms);
    DRIVER_UVIS25_LINK_DEBUG_PRINT(&gs_handle, uvis25_interface_debug_print);
    DRIVER_UVIS25_LINK_RECEIVE_CALLBACK(&gs_handle, callback);

    /* set interface */
    res = uvis25_set_interface(&gs_handle, interface);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: set interface failed.\n");

        return 1;
    }

    /* uvis25 init */
    res = uvis25_init(&gs_handle);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: init failed.\n");

        return 1;
    }

    /* set default iic */
    res = uvis25_set_iic(&gs_handle, UVIS25_INTERRUPT_DEFAULT_IIC);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: set iic failed.\n");
        (void)uvis25_deinit(&gs_handle);

        return 1;
    }

    /* set default spi wire */
    res = uvis25_set_spi_wire(&gs_handle, UVIS25_INTERRUPT_DEFAULT_SPI_WIRE);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: set spi wire failed.\n");
        (void)uvis25_deinit(&gs_handle);

        return 1;
    }

    /* set default boot */
    res = uvis25_set_boot(&gs_handle, UVIS25_INTERRUPT_DEFAULT_BOOT_MODE);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: set boot failed.\n");
        (void)uvis25_deinit(&gs_handle);

        return 1;
    }

    /* set default block data update */
    res = uvis25_set_block_data_update(&gs_handle, UVIS25_INTERRUPT_DEFAULT_BLOCK_DATA_UPDATE);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: set block data update failed.\n");
        (void)uvis25_deinit(&gs_handle);

        return 1;
    }

    /* set default interrupt active level */
    res = uvis25_set_interrupt_active_level(&gs_handle, UVIS25_INTERRUPT_DEFAULT_INTERRUPT_ACTIVE_LEVEL);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: set interrupt active level failed.\n");
        (void)uvis25_deinit(&gs_handle);

        return 1;
    }

    /* set default interrupt pin type */
    res = uvis25_set_interrupt_pin_type(&gs_handle, UVIS25_INTERRUPT_DEFAULT_INTERRUPT_PIN_TYPE);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: set interrupt pin type failed.\n");
        (void)uvis25_deinit(&gs_handle);

        return 1;
    }

    /* set default interrupt mode type */
    res = uvis25_set_interrupt_type(&gs_handle, mode);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: set interrupt type failed.\n");
        (void)uvis25_deinit(&gs_handle);

        return 1;
    }

    /* set default interrupt */
    res = uvis25_set_interrupt(&gs_handle, UVIS25_INTERRUPT_DEFAULT_INTERRUPT);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: set interrupt failed.\n");
        (void)uvis25_deinit(&gs_handle);

        return 1;
    }

    /* set default latch interrupt */
    res = uvis25_set_latch_interrupt(&gs_handle, UVIS25_INTERRUPT_DEFAULT_LATCH_INTERRUPT);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: set latch interrupt failed.\n");
        (void)uvis25_deinit(&gs_handle);

        return 1;
    }

    /* set default interrupt low threshold */
    res = uvis25_set_interrupt_low_threshold(&gs_handle, UVIS25_INTERRUPT_DEFAULT_LOW_THRESHOLD_INTERRUPT);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: set interrupt low threshold failed.\n");
        (void)uvis25_deinit(&gs_handle);

        return 1;
    }

    /* set default interrupt high threshold */
    res = uvis25_set_interrupt_high_threshold(&gs_handle, UVIS25_INTERRUPT_DEFAULT_HIGH_THRESHOLD_INTERRUPT);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: set interrupt high threshold failed.\n");
        (void)uvis25_deinit(&gs_handle);

        return 1;
    }

    /* convert threshold to register */
    res = uvis25_threshold_convert_to_register(&gs_handle, uv_threshold, (uint8_t *)&reg);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: threshold convert to register failed.\n");
        (void)uvis25_deinit(&gs_handle);

        return 1;
    }

    /* set threshold */
    res = uvis25_set_threshold(&gs_handle, reg);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: set threshold failed.\n");
        (void)uvis25_deinit(&gs_handle);

        return 1;
    }

    /* start continuous read */
    res = uvis25_start_continuous_read(&gs_handle);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: start continuous read failed.\n");
        (void)uvis25_deinit(&gs_handle);

        return 1;
    }

    return 0;
}

/**
 * @brief      interrupt example read
 * @param[out] *uv points to a uv index buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t uvis25_interrupt_read(float *uv)
{
    uint8_t raw;

    /* continuous read */
    if (uvis25_continuous_read(&gs_handle, (uint8_t *)&raw, uv) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief  basic interrupt deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t uvis25_interrupt_deinit(void)
{
    uint8_t res;

    /* stop continuous read */
    res = uvis25_stop_continuous_read(&gs_handle);
    if (res != 0)
    {
        return 1;
    }

    /* close uvis25 */
    if (uvis25_deinit(&gs_handle) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
