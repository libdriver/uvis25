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
 * @file      driver_uvis25_register_test.c
 * @brief     driver uvis25 register test source file
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

#include "driver_uvis25_register_test.h"
#include <stdlib.h>

static uvis25_handle_t gs_handle;        /**< uvis25 handle */

/**
 * @brief     register test
 * @param[in] interface is the chip interface
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t uvis25_register_test(uvis25_interface_t interface)
{
    uint8_t res, in, in_check, reg;
    float uv, uv_check;
    uvis25_bool_t bool_check;
    uvis25_interface_t interface_test;
    uvis25_spi_wire_t wire;
    uvis25_interrupt_active_level_t level;
    uvis25_interrupt_pin_type_t pin;
    uvis25_interrupt_type_t type;
    uvis25_boot_mode_t mode;
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
    
    /* start register test */
    uvis25_interface_debug_print("uvis25: start register test.\n");
    
    /* uvis25_set_interface/uvis25_get_interface test */
    uvis25_interface_debug_print("uvis25: uvis25_set_interface/uvis25_get_interface test.\n");
    
    /* set iic */
    res = uvis25_set_interface(&gs_handle, UVIS25_INTERFACE_IIC);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: set interface failed.\n");
       
        return 1;
    }
    uvis25_interface_debug_print("uvis25: set interface iic.\n");
    res = uvis25_get_interface(&gs_handle, &interface_test);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: get interface failed.\n");
       
        return 1;
    }
    uvis25_interface_debug_print("uvis25: check interface %s.\n", interface_test==UVIS25_INTERFACE_IIC?"ok":"error");
    
    /* set spi */
    res = uvis25_set_interface(&gs_handle, UVIS25_INTERFACE_SPI);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: set interface failed.\n");
       
        return 1;
    }
    uvis25_interface_debug_print("uvis25: set interface spi.\n");
    res = uvis25_get_interface(&gs_handle, &interface_test);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: get interface failed.\n");
       
        return 1;
    }
    uvis25_interface_debug_print("uvis25: check interface %s.\n", interface_test==UVIS25_INTERFACE_SPI?"ok":"error");
    
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
    
    /* uvis25_set_block_data_update/uvis25_get_block_data_update test */
    uvis25_interface_debug_print("uvis25: uvis25_set_block_data_update/uvis25_get_block_data_update test.\n");
    
    /* set true */
    res = uvis25_set_block_data_update(&gs_handle, UVIS25_BOOL_TRUE);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: set block data update failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    uvis25_interface_debug_print("uvis25: set block data update true.\n");
    res = uvis25_get_block_data_update(&gs_handle, &bool_check);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: get block data update failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    uvis25_interface_debug_print("uvis25: check block data update %s.\n", bool_check==UVIS25_BOOL_TRUE?"ok":"error");
    
    /* set false */
    res = uvis25_set_block_data_update(&gs_handle, UVIS25_BOOL_FALSE);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: set block data update failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    uvis25_interface_debug_print("uvis25: set block data update false.\n");
    res = uvis25_get_block_data_update(&gs_handle, &bool_check);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: get block data update failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    uvis25_interface_debug_print("uvis25: check block data update %s.\n", bool_check==UVIS25_BOOL_FALSE?"ok":"error");
    
    /* if spi interface */
    if (interface == UVIS25_INTERFACE_SPI)
    {
        /* uvis25_set_iic/uvis25_get_iic test */
        uvis25_interface_debug_print("uvis25: uvis25_set_iic/uvis25_get_iic test.\n");
    
        /* set true */
        res = uvis25_set_iic(&gs_handle, UVIS25_BOOL_TRUE);
        if (res != 0)
        {
            uvis25_interface_debug_print("uvis25: set iic failed.\n");
            (void)uvis25_deinit(&gs_handle);
            
            return 1;
        }
        uvis25_interface_debug_print("uvis25: set iic true.\n");
        res = uvis25_get_iic(&gs_handle, &bool_check);
        if (res != 0)
        {
            uvis25_interface_debug_print("uvis25: get iic failed.\n");
            (void)uvis25_deinit(&gs_handle);
            
            return 1;
        }
        uvis25_interface_debug_print("uvis25: check iic %s.\n", bool_check==UVIS25_BOOL_TRUE?"ok":"error");

        /* set false */
        res = uvis25_set_iic(&gs_handle, UVIS25_BOOL_FALSE);
        if (res != 0)
        {
            uvis25_interface_debug_print("uvis25: set iic failed.\n");
            (void)uvis25_deinit(&gs_handle);
            
            return 1;
        }
        uvis25_interface_debug_print("uvis25: set iic false.\n");
        res = uvis25_get_iic(&gs_handle, &bool_check);
        if (res != 0)
        {
            uvis25_interface_debug_print("uvis25: get iic failed.\n");
            (void)uvis25_deinit(&gs_handle);
            
            return 1;
        }
        uvis25_interface_debug_print("uvis25: check iic %s.\n", bool_check==UVIS25_BOOL_FALSE?"ok":"error");
    }
    else
    {
        /* uvis25_set_spi_wire/uvis25_get_spi_wire test */
        uvis25_interface_debug_print("uvis25: uvis25_set_spi_wire/uvis25_get_spi_wire test.\n");

        /* set true */
        res = uvis25_set_spi_wire(&gs_handle, UVIS25_SPI_WIRE_4);
        if (res != 0)
        {
            uvis25_interface_debug_print("uvis25: set spi wire failed.\n");
            (void)uvis25_deinit(&gs_handle);
            
            return 1;
        }
        uvis25_interface_debug_print("uvis25: set spi wire 4.\n");
        res = uvis25_get_spi_wire(&gs_handle, &wire);
        if (res != 0)
        {
            uvis25_interface_debug_print("uvis25: get spi wire 4 failed.\n");
            (void)uvis25_deinit(&gs_handle);
            
            return 1;
        }
        uvis25_interface_debug_print("uvis25: check spi wire %s.\n", wire==UVIS25_SPI_WIRE_4?"ok":"error");

        /* set false */
        res = uvis25_set_spi_wire(&gs_handle, UVIS25_SPI_WIRE_3);
        if (res != 0)
        {
            uvis25_interface_debug_print("uvis25: set spi wire failed.\n");
            (void)uvis25_deinit(&gs_handle);
            
            return 1;
        }
        uvis25_interface_debug_print("uvis25: set spi wire 3.\n");
        res = uvis25_get_spi_wire(&gs_handle, &wire);
        if (res != 0)
        {
            uvis25_interface_debug_print("uvis25: get spi wire 3 failed.\n");
            (void)uvis25_deinit(&gs_handle);
            
            return 1;
        }
        uvis25_interface_debug_print("uvis25: check spi wire %s.\n", wire==UVIS25_SPI_WIRE_3?"ok":"error");
    }

    /* uvis25_set_interrupt_active_level/uvis25_get_interrupt_active_level test */
    uvis25_interface_debug_print("uvis25: uvis25_set_interrupt_active_level/uvis25_get_interrupt_active_level test.\n");

    /* set high */
    res = uvis25_set_interrupt_active_level(&gs_handle, UVIS25_INTERRUPT_ACTIVE_LEVEL_HIGHER);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: set interrupt active level failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    uvis25_interface_debug_print("uvis25: set interrupt active level high.\n");
    res = uvis25_get_interrupt_active_level(&gs_handle, &level);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: get interrupt active level failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    uvis25_interface_debug_print("uvis25: check interrupt active level %s.\n", level==UVIS25_INTERRUPT_ACTIVE_LEVEL_HIGHER?"ok":"error");

    /* set low */
    res = uvis25_set_interrupt_active_level(&gs_handle, UVIS25_INTERRUPT_ACTIVE_LEVEL_LOWER);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: set interrupt active level failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    uvis25_interface_debug_print("uvis25: set interrupt active level lower.\n");
    res = uvis25_get_interrupt_active_level(&gs_handle, &level);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: get interrupt active level failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    uvis25_interface_debug_print("uvis25: check interrupt active level %s.\n", level==UVIS25_INTERRUPT_ACTIVE_LEVEL_LOWER?"ok":"error");

    /* uvis25_set_interrupt_pin_type/uvis25_get_interrupt_pin_type test */
    uvis25_interface_debug_print("uvis25: uvis25_set_interrupt_pin_type/uvis25_get_interrupt_pin_type test.\n");

    /* set push-pull */
    res = uvis25_set_interrupt_pin_type(&gs_handle, UVIS25_INTERRUPT_PIN_TYPE_PUSH_PULL);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: set interrupt pin type failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    uvis25_interface_debug_print("uvis25: set interrupt pin type push pull.\n");
    res = uvis25_get_interrupt_pin_type(&gs_handle, &pin);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: get interrupt pin type.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    uvis25_interface_debug_print("uvis25: check interrupt pin type %s.\n", pin==UVIS25_INTERRUPT_PIN_TYPE_PUSH_PULL?"ok":"error");
    
    /* set open drain */
    res = uvis25_set_interrupt_pin_type(&gs_handle, UVIS25_INTERRUPT_PIN_TYPE_OPEN_DRAIN);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: set interrupt pin type failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    uvis25_interface_debug_print("uvis25: set interrupt pin type open drain.\n");
    res = uvis25_get_interrupt_pin_type(&gs_handle, &pin);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: get interrupt pin type.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    uvis25_interface_debug_print("uvis25: check interrupt pin type %s.\n", pin==UVIS25_INTERRUPT_PIN_TYPE_OPEN_DRAIN?"ok":"error");
    
    /* uvis25_set_interrupt_type/uvis25_get_interrupt_type test */
    uvis25_interface_debug_print("uvis25: uvis25_set_interrupt_type/uvis25_get_interrupt_type test.\n");
    
    /* set data ready */
    res = uvis25_set_interrupt_type(&gs_handle, UVIS25_INTERRUPT_TYPE_DATA_READY);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: set interrupt type failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    uvis25_interface_debug_print("uvis25: set interrupt type data ready.\n");
    res = uvis25_get_interrupt_type(&gs_handle, &type);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: get interrupt type.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    uvis25_interface_debug_print("uvis25: check interrupt type %s.\n", type==UVIS25_INTERRUPT_TYPE_DATA_READY?"ok":"error");
    
    /* set uv index high */
    res = uvis25_set_interrupt_type(&gs_handle, UVIS25_INTERRUPT_TYPE_UV_INDEX_HIGH);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: set interrupt type failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    uvis25_interface_debug_print("uvis25: set interrupt type uv index high.\n");
    res = uvis25_get_interrupt_type(&gs_handle, &type);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: get interrupt type.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    uvis25_interface_debug_print("uvis25: check interrupt type %s.\n", type==UVIS25_INTERRUPT_TYPE_UV_INDEX_HIGH?"ok":"error");
    
    /* set uv index low */
    res = uvis25_set_interrupt_type(&gs_handle, UVIS25_INTERRUPT_TYPE_UV_INDEX_LOW);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: set interrupt type failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    uvis25_interface_debug_print("uvis25: set interrupt type uv index low.\n");
    res = uvis25_get_interrupt_type(&gs_handle, &type);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: get interrupt type failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    uvis25_interface_debug_print("uvis25: check interrupt type %s.\n", type==UVIS25_INTERRUPT_TYPE_UV_INDEX_LOW?"ok":"error");
    
    /* set uv index high or low */
    res = uvis25_set_interrupt_type(&gs_handle, UVIS25_INTERRUPT_TYPE_UV_INDEX_HIGH_LOW);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: set interrupt type failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    uvis25_interface_debug_print("uvis25: set interrupt type uv index high or low.\n");
    res = uvis25_get_interrupt_type(&gs_handle, &type);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: get interrupt type failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    uvis25_interface_debug_print("uvis25: check interrupt type %s.\n", type==UVIS25_INTERRUPT_TYPE_UV_INDEX_HIGH_LOW?"ok":"error");
    
    /* uvis25_set_interrupt/uvis25_get_interrupt test */
    uvis25_interface_debug_print("uvis25: uvis25_set_interrupt/uvis25_get_interrupt test.\n");
    
    /* set ture */
    res = uvis25_set_interrupt(&gs_handle, UVIS25_BOOL_TRUE);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: set interrupt failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    uvis25_interface_debug_print("uvis25: set interrupt true.\n");
    res = uvis25_get_interrupt(&gs_handle, &bool_check);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: get interrupt failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    uvis25_interface_debug_print("uvis25: check interrupt %s.\n", bool_check==UVIS25_BOOL_TRUE?"ok":"error");
    
    /* set false */
    res = uvis25_set_interrupt(&gs_handle, UVIS25_BOOL_FALSE);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: set interrupt failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    uvis25_interface_debug_print("uvis25: set interrupt false.\n");
    res = uvis25_get_interrupt(&gs_handle, &bool_check);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: get interrupt failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    uvis25_interface_debug_print("uvis25: check interrupt %s.\n", bool_check==UVIS25_BOOL_FALSE?"ok":"error");
    
    /* uvis25_set_latch_interrupt/uvis25_get_latch_interrupt test */
    uvis25_interface_debug_print("uvis25: uvis25_set_latch_interrupt/uvis25_get_latch_interrupt test.\n");
    
    /* set ture */
    res = uvis25_set_latch_interrupt(&gs_handle, UVIS25_BOOL_TRUE);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: set latch interrupt failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    uvis25_interface_debug_print("uvis25: set latch interrupt true.\n");
    res = uvis25_get_latch_interrupt(&gs_handle, &bool_check);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: get latch interrupt failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    uvis25_interface_debug_print("uvis25: check latch interrupt %s.\n", bool_check==UVIS25_BOOL_TRUE?"ok":"error");
    
    /* set false */
    res = uvis25_set_latch_interrupt(&gs_handle, UVIS25_BOOL_FALSE);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: set latch interrupt failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    uvis25_interface_debug_print("uvis25: set latch interrupt false.\n");
    res = uvis25_get_latch_interrupt(&gs_handle, &bool_check);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: get latch interrupt failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    uvis25_interface_debug_print("uvis25: check latch interrupt %s.\n", bool_check==UVIS25_BOOL_FALSE?"ok":"error");
    
    /* uvis25_set_interrupt_low_threshold/uvis25_get_interrupt_low_threshold test */
    uvis25_interface_debug_print("uvis25: uvis25_set_interrupt_low_threshold/uvis25_get_interrupt_low_threshold test.\n");
    
    /* set ture */
    res = uvis25_set_interrupt_low_threshold(&gs_handle, UVIS25_BOOL_TRUE);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: set interrupt low threshold failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    uvis25_interface_debug_print("uvis25: set interrupt low threshold true.\n");
    res = uvis25_get_interrupt_low_threshold(&gs_handle, &bool_check);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: get interrupt low threshold failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    uvis25_interface_debug_print("uvis25: check interrupt low threshold %s.\n", bool_check==UVIS25_BOOL_TRUE?"ok":"error");
    
    /* set false */
    res = uvis25_set_interrupt_low_threshold(&gs_handle, UVIS25_BOOL_FALSE);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: set interrupt low threshold failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    uvis25_interface_debug_print("uvis25: set interrupt low threshold false.\n");
    res = uvis25_get_interrupt_low_threshold(&gs_handle, &bool_check);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: get interrupt low threshold failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    uvis25_interface_debug_print("uvis25: check interrupt low threshold %s.\n", bool_check==UVIS25_BOOL_FALSE?"ok":"error");
    
    /* uvis25_set_interrupt_high_threshold/uvis25_get_interrupt_high_threshold test */
    uvis25_interface_debug_print("uvis25: uvis25_set_interrupt_high_threshold/uvis25_get_interrupt_high_threshold test.\n");
    
    /* set ture */
    res = uvis25_set_interrupt_high_threshold(&gs_handle, UVIS25_BOOL_TRUE);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: set interrupt high threshold failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    uvis25_interface_debug_print("uvis25: set interrupt high threshold true.\n");
    res = uvis25_get_interrupt_high_threshold(&gs_handle, &bool_check);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: get interrupt high threshold failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    uvis25_interface_debug_print("uvis25: check interrupt high threshold %s.\n", bool_check==UVIS25_BOOL_TRUE?"ok":"error");
    
    /* set false */
    res = uvis25_set_interrupt_high_threshold(&gs_handle, UVIS25_BOOL_FALSE);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: set interrupt high threshold failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    uvis25_interface_debug_print("uvis25: set interrupt high threshold false.\n");
    res = uvis25_get_interrupt_high_threshold(&gs_handle, &bool_check);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: get interrupt high threshold failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    uvis25_interface_debug_print("uvis25: check interrupt high threshold %s.\n", bool_check==UVIS25_BOOL_FALSE?"ok":"error");
    
    /* uvis25_set_threshold/uvis25_get_threshold test */
    uvis25_interface_debug_print("uvis25: uvis25_set_threshold/uvis25_get_threshold test.\n");
    in = rand()%256;
    res = uvis25_set_threshold(&gs_handle, in);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: set threshold failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    uvis25_interface_debug_print("uvis25: set threshold %d.\n", in);
    res = uvis25_get_threshold(&gs_handle, (uint8_t *)&in_check);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: get threshold failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    uvis25_interface_debug_print("uvis25: check threshold %s.\n", in==in_check?"ok":"error");
    
    /* uvis25_set_boot/uvis25_get_boot test */
    uvis25_interface_debug_print("uvis25: uvis25_set_boot/uvis25_get_boot test.\n");
    
    /* set normal */
    res = uvis25_set_boot(&gs_handle, UVIS25_BOOT_NORMAL_MODE);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: set boot failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    uvis25_interface_debug_print("uvis25: set boot mode normal.\n");
    res = uvis25_get_boot(&gs_handle, &mode);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: get boot mode failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    uvis25_interface_debug_print("uvis25: check boot mode %s.\n", mode==UVIS25_BOOT_NORMAL_MODE?"ok":"error");
    
    /* uvis25_threshold_convert_to_register/uvis25_threshold_convert_to_data test */
    uvis25_interface_debug_print("uvis25: uvis25_threshold_convert_to_register/uvis25_threshold_convert_to_data test.\n");
    uv = (float)(rand()%1000)/100.0f + 6.0f;
    res = uvis25_threshold_convert_to_register(&gs_handle, uv, (uint8_t *)&reg);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: threshold convert to register failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    uvis25_interface_debug_print("uvis25: threshold convert to register.\n");
    uvis25_interface_debug_print("uvis25: uv is %.04f.\n", uv);
    uvis25_interface_debug_print("uvis25: register is 0x%02X.\n", reg);
    res = uvis25_threshold_convert_to_data(&gs_handle, reg, (float *)&uv_check);
    if (res != 0)
    {
        uvis25_interface_debug_print("uvis25: threshold convert to data failed.\n");
        (void)uvis25_deinit(&gs_handle);
        
        return 1;
    }
    uvis25_interface_debug_print("uvis25: threshold convert to data.\n");
    uvis25_interface_debug_print("uvis25: register is 0x%02X.\n", reg);
    uvis25_interface_debug_print("uvis25: uv is %.04f.\n", uv_check);
    
    /* finish register test */
    uvis25_interface_debug_print("uvis25: finish register test.\n");
    (void)uvis25_deinit(&gs_handle);
    
    return 0;
}
