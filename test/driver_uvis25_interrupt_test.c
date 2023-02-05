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
 * @file      driver_uvis25_interrupt_test.c
 * @brief     driver uvis25 interrupt test source file
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

#include "driver_uvis25_interrupt_test.h"

static uvis25_handle_t gs_handle;        /**< uvis25 handle */

/**
 * @brief  interrupt test irq handler
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t uvis25_interrupt_test_irq_handler(void)
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
 * @brief     interrupt test
 * @param[in] interface is the chip interface
 * @param[in] threshold is the uv threshold
 * @param[in] times is the test times
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t uvis25_interrupt_test(uvis25_interface_t interface, float threshold, uint32_t times)
{
    uint8_t res, reg;
    uint32_t i;
    uvis25_info_t info;
    
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
    DRIVER_UVIS25_LINK_RECEIVE_CALLBACK(&gs_handle, uvis25_interface_receive_callback); 

    /* get information */
    res = uvis25_info(&info);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print chip information */
        uvis25_interface_debug_print("uvis25: chip is %s.\n", info.chip_name);
        uvis25_interface_debug_print("uvis25: manufacturer is %s.\n", info.manufacturer_name);
        uvis25_interface_debug_print("uvis25: interface is %s.\n", info.interface);
        uvis25_interface_debug_print("uvis25: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        uvis25_interface_debug_print("uvis25: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        uvis25_interface_debug_print("uvis25: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        uvis25_interface_debug_print("uvis25: max current is %0.2fmA.\n", info.max_current_ma);
        uvis25_interface_debug_print("uvis25: max temperature is %0.1fC.\n", info.temperature_max);
        uvis25_interface_debug_print("uvis25: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
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
    
    /* start interrupt test */
    uvis25_interface_debug_print("uvis25: start interrupt test.\n");
    res = uvis25_set_iic(&gs_handle, UVIS25_BOOL_TRUE);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: set iic failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set spi wire 4 */
    res = uvis25_set_spi_wire(&gs_handle, UVIS25_SPI_WIRE_4);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: set spi wire failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }  
    
    /* set normal mode */
    res = uvis25_set_boot(&gs_handle, UVIS25_BOOT_NORMAL_MODE);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: set boot failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable block data update */
    res = uvis25_set_block_data_update(&gs_handle, UVIS25_BOOL_FALSE);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: set block data update failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set interrupt active level lower */
    res = uvis25_set_interrupt_active_level(&gs_handle, UVIS25_INTERRUPT_ACTIVE_LEVEL_LOWER);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: set interrupt active level failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set push-pull type */
    res = uvis25_set_interrupt_pin_type(&gs_handle, UVIS25_INTERRUPT_PIN_TYPE_PUSH_PULL);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: set interrupt pin type failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set uv index low */
    res = uvis25_set_interrupt_type(&gs_handle, UVIS25_INTERRUPT_TYPE_UV_INDEX_LOW);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: set interrupt type failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable interrupt */
    res = uvis25_set_interrupt(&gs_handle, UVIS25_BOOL_TRUE);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: set interrupt failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable latch interrupt */
    res = uvis25_set_latch_interrupt(&gs_handle, UVIS25_BOOL_TRUE);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: set latch interrupt failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable interrupt low threshold */
    res = uvis25_set_interrupt_low_threshold(&gs_handle, UVIS25_BOOL_TRUE);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: set interrupt low threshold failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable interrupt high threshold */
    res = uvis25_set_interrupt_high_threshold(&gs_handle, UVIS25_BOOL_TRUE);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: set interrupt high threshold failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    
    /* threshold convert to register */
    res = uvis25_threshold_convert_to_register(&gs_handle,  threshold, (uint8_t *)&reg);
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
    for (i = 0; i < times; i++)
    {
        uint8_t raw;
        float uv;
        
        uvis25_interface_delay_ms(2000);
    
        /* continuous read */
        res = uvis25_continuous_read(&gs_handle, (uint8_t *)&raw, (float *)&uv);
        if (res != 0)
        {
            uvis25_interface_debug_print("uvis25: read failed.\n");
            (void)uvis25_deinit(&gs_handle);
            
            return 1;
        }
        uvis25_interface_debug_print("uvis25: uv %0.4f is %s than threshold.\n", uv, uv>threshold ? "higher": "lower");
    }

    /* stop continuous read */
    res = uvis25_stop_continuous_read(&gs_handle);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: stop continuous read failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }

    /* finish interrupt test */
    uvis25_interface_debug_print("uvis25: finish interrupt test.\n");
    (void)uvis25_deinit(&gs_handle);
    
    return 0;
}
