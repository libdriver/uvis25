/**
 * Copyright (C) LibDriver 2015-2021 All rights reserved
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
 * @file      main.c
 * @brief     main source file
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

#include "driver_uvis25_basic.h"
#include "driver_uvis25_shot.h"
#include "driver_uvis25_interrupt.h"
#include "driver_uvis25_register_test.h"
#include "driver_uvis25_read_test.h"
#include "driver_uvis25_interrupt_test.h"
#include <gpio.h>
#include <stdlib.h>

uint8_t g_flag;                            /**< interrupt flag */
uint8_t (*g_gpio_irq)(void) = NULL;        /**< gpio irq function address */

/**
 * @brief     uvis25 full function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *             - 0 success
 *             - 1 run failed
 *             - 5 param is invalid
 * @note      none
 */
uint8_t uvis25(uint8_t argc, char **argv)
{
    if (argc == 1)
    {
        goto help;
    }
    else if (argc == 2)
    {
        if (strcmp("-i", argv[1]) == 0)
        {
            uvis25_info_t info;
            
            /* print uvis25 info */
            uvis25_info(&info);
            uvis25_interface_debug_print("uvis25: chip is %s.\n", info.chip_name);
            uvis25_interface_debug_print("uvis25: manufacturer is %s.\n", info.manufacturer_name);
            uvis25_interface_debug_print("uvis25: interface is %s.\n", info.interface);
            uvis25_interface_debug_print("uvis25: driver version is %d.%d.\n", info.driver_version/1000, (info.driver_version%1000)/100);
            uvis25_interface_debug_print("uvis25: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
            uvis25_interface_debug_print("uvis25: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
            uvis25_interface_debug_print("uvis25: max current is %0.2fmA.\n", info.max_current_ma);
            uvis25_interface_debug_print("uvis25: max temperature is %0.1fC.\n", info.temperature_max);
            uvis25_interface_debug_print("uvis25: min temperature is %0.1fC.\n", info.temperature_min);
            
            return 0;
        }
        else if (strcmp("-p", argv[1]) == 0)
        {
            /* print pin connection */
            uvis25_interface_debug_print("uvis25: SPI interface SCK connected to GPIO11(BCM).\n");
            uvis25_interface_debug_print("uvis25: SPI interface MISO connected to GPIO9(BCM).\n");
            uvis25_interface_debug_print("uvis25: SPI interface MOSI connected to GPIO10(BCM).\n");
            uvis25_interface_debug_print("uvis25: SPI interface CS connected to GPIO8(BCM).\n");
            uvis25_interface_debug_print("uvis25: IIC interface SCL connected to GPIO3(BCM).\n");
            uvis25_interface_debug_print("uvis25: IIC interface SDA connected to GPIO2(BCM).\n");
            uvis25_interface_debug_print("uvis25: INT connected to GPIO17(BCM).\n");
            
            return 0;
        }
        else if (strcmp("-h", argv[1]) == 0)
        {
            /* show uvis25 help */
            
            help:
            
            uvis25_interface_debug_print("uvis25 -i\n\tshow uvis25 chip and driver information.\n");
            uvis25_interface_debug_print("uvis25 -h\n\tshow uvis25 help.\n");
            uvis25_interface_debug_print("uvis25 -p\n\tshow uvis25 pin connections of the current board.\n");
            uvis25_interface_debug_print("uvis25 -t reg (-iic | -spi)\n\trun uvis25 register test.\n");
            uvis25_interface_debug_print("uvis25 -t read <times> (-iic | -spi)\n\trun uvis25 read test.times means test times.\n");
            uvis25_interface_debug_print("uvis25 -t int <times> (-iic | -spi) -th <threshold>\n\trun uvis25 interrupt test."
                                         "times means test times.threshold means the interrupt threshold.\n");
            uvis25_interface_debug_print("uvis25 -c read <times> (-iic | -spi)\n\trun uvis25 read function.times means test times.\n");
            uvis25_interface_debug_print("uvis25 -c shot <times> (-iic | -spi)\n\trun uvis25 shot function.times means test times.\n");
            uvis25_interface_debug_print("uvis25 -c int <times> (-iic | -spi) -m <mode> -th <threshold>\n\trun uvis25 interrupt function."
                                         "times means test times.");
            uvis25_interface_debug_print("mode means interrupt mode and it can be \"READY\",\"HIGH\",\"LOW\" or \"HIGH|LOW\"."
                                         "threshold means the interrupt threshold.\n");
            
            return 0;
        }
        else
        {
            return 5;
        }
    }
    else if (argc == 4)
    {
        /* run test */
        if (strcmp("-t", argv[1]) == 0)
        {
            /* reg test */
            if (strcmp("reg", argv[2]) == 0)
            {
                volatile uint8_t res;
                uvis25_interface_t interface;
                
                if (strcmp("-iic", argv[3]) == 0)
                {
                    interface = UVIS25_INTERFACE_IIC;
                }
                else if (strcmp("-spi", argv[3]) == 0)
                {
                    interface = UVIS25_INTERFACE_SPI;
                }
                else
                {
                    uvis25_interface_debug_print("uvis25: interface is invalid.\n");
                    
                    return 5;
                }
                res = uvis25_register_test(interface);
                if (res)
                {
                    return 1;
                }
                
                return 0;
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* param is invalid */
        else
        {
            return 5;
        }
    }
    else if (argc == 5)
    {
        /* run test */
        if (strcmp("-t", argv[1]) == 0)
        {
            /* read test */
            if (strcmp("read", argv[2]) == 0)
            {
                volatile uint8_t res;
                volatile uint32_t times;
                uvis25_interface_t interface;
                
                if (strcmp("-iic", argv[4]) == 0)
                {
                    interface = UVIS25_INTERFACE_IIC;
                }
                else if (strcmp("-spi", argv[4]) == 0)
                {
                    interface = UVIS25_INTERFACE_SPI;
                }
                else
                {
                    uvis25_interface_debug_print("uvis25: interface is invalid.\n");
                    
                    return 5;
                }
                times = atoi(argv[3]);
                res = uvis25_read_test(interface, times);
                if (res)
                {
                    return 1;
                }
                
                return 0;
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* run function */
        if (strcmp("-c", argv[1]) == 0)
        {
            /* read function */
            if (strcmp("read", argv[2]) == 0)
            {
                volatile uint8_t res;
                volatile uint32_t i, times;
                volatile float uv;
                uvis25_interface_t interface;
                
                if (strcmp("-iic", argv[4]) == 0)
                {
                    interface = UVIS25_INTERFACE_IIC;
                }
                else if (strcmp("-spi", argv[4]) == 0)
                {
                    interface = UVIS25_INTERFACE_SPI;
                }
                else
                {
                    uvis25_interface_debug_print("uvis25: interface is invalid.\n");
                    
                    return 5;
                }
                times = atoi(argv[3]);
                res = uvis25_basic_init(interface);
                if (res)
                {
                    return 1;
                }
                for (i=0; i<times; i++)
                {
                    uvis25_interface_delay_ms(2000);
                    res = uvis25_basic_read((float *)&uv);
                    if (res)
                    {
                        uvis25_basic_deinit();
                        
                        return 1;
                    }
                    uvis25_interface_debug_print("uvis25: %d/%d.\n", i+1, times);
                    uvis25_interface_debug_print("uvis25: uv is %0.4f.\n", uv);
                }
                uvis25_basic_deinit();
                
                return 0;
            }
            /* shot function */
            else if (strcmp("shot", argv[2]) == 0)
            {
                volatile uint8_t res;
                volatile uint32_t i, times;
                volatile float uv;
                uvis25_interface_t interface;
                
                if (strcmp("-iic", argv[4]) == 0)
                {
                    interface = UVIS25_INTERFACE_IIC;
                }
                else if (strcmp("-spi", argv[4]) == 0)
                {
                    interface = UVIS25_INTERFACE_SPI;
                }
                else
                {
                    uvis25_interface_debug_print("uvis25: interface is invalid.\n");
                    
                    return 5;
                }
                times = atoi(argv[3]);
                res = uvis25_shot_init(interface);
                if (res)
                {
                    return 1;
                }
                for (i=0; i<times; i++)
                {
                    uvis25_interface_delay_ms(2000);
                    res = uvis25_shot_read((float *)&uv);
                    if (res)
                    {
                        uvis25_shot_deinit();
                        
                        return 1;
                    }
                    uvis25_interface_debug_print("uvis25: %d/%d.\n", i+1, times);
                    uvis25_interface_debug_print("uvis25: uv is %0.4f.\n", uv);
                }
                uvis25_shot_deinit();
                
                return 0;
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* param is invalid */
        else
        {
            return 5;
        }
    }
    else if (argc == 7)
    {
        /* run test */
        if (strcmp("-t", argv[1]) == 0)
        {
            /* int test */
            if (strcmp("int", argv[2]) == 0)
            {
                volatile uint8_t res;
                volatile uint32_t times;
                uvis25_interface_t interface;
                
                if (strcmp("-iic", argv[4]) == 0)
                {
                    interface = UVIS25_INTERFACE_IIC;
                }
                else if (strcmp("-spi", argv[4]) == 0)
                {
                    interface = UVIS25_INTERFACE_SPI;
                }
                else
                {
                    uvis25_interface_debug_print("uvis25: interface is invalid.\n");
                    
                    return 5;
                }
                if (strcmp("-th", argv[5]) != 0)
                {
                    return 5;
                }
                times = atoi(argv[3]);
                g_gpio_irq = uvis25_interface_test_irq_handler;
                res = gpio_interrupt_init();
                if (res)
                {
                    return 1;
                }
                res = uvis25_interrupt_test(interface, atof(argv[6]), times);
                if (res)
                {
                    gpio_interrupt_deinit();
                    g_gpio_irq = NULL;
                    
                    return 1;
                }
                gpio_interrupt_deinit();
                g_gpio_irq = NULL;
                
                return 0;
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* param is invalid */
        else
        {
            return 5;
        }
    }
    else if (argc == 9)
    {
        /* run function */
        if (strcmp("-c", argv[1]) == 0)
        {
            /* int function */
            if (strcmp("int", argv[2]) == 0)
            {
                volatile uint8_t res;
                volatile uint32_t i, times;
                volatile float uv;
                uvis25_interface_t interface;
                uvis25_interrupt_type_t mode;
                
                if (strcmp("-iic", argv[4]) == 0)
                {
                    interface = UVIS25_INTERFACE_IIC;
                }
                else if (strcmp("-spi", argv[4]) == 0)
                {
                    interface = UVIS25_INTERFACE_SPI;
                }
                else
                {
                    uvis25_interface_debug_print("uvis25: interface is invalid.\n");
                    
                    return 5;
                }
                if (strcmp("-m", argv[5]) != 0)
                {
                    return 5;
                }
                if (strcmp("READY", argv[6]) == 0)
                {
                    mode = UVIS25_INTERRUPT_TYPE_DATA_READY;
                }
                else if (strcmp("LOW", argv[6]) == 0)
                {
                    mode = UVIS25_INTERRUPT_TYPE_UV_INDEX_LOW;
                }
                else if (strcmp("HIGH", argv[6]) == 0)
                {
                    mode = UVIS25_INTERRUPT_TYPE_UV_INDEX_HIGH;
                }
                else if (strcmp("HIGH|LOW", argv[6]) == 0)
                {
                    mode = UVIS25_INTERRUPT_TYPE_UV_INDEX_HIGH_LOW;
                }
                else
                {
                    uvis25_interface_debug_print("uvis25: mode is invalid.\n");
                    
                    return 5;
                }
                if (strcmp("-th", argv[7]) != 0)
                {
                    return 5;
                }
                times = atoi(argv[3]);
                g_gpio_irq = uvis25_interface_irq_handler;
                res = gpio_interrupt_init();
                if (res)
                {
                    return 1;
                }
                g_flag = 0;
                res = uvis25_interrupt_init(interface, mode, atof(argv[8]));
                if (res)
                {
                    gpio_interrupt_deinit();
                    g_gpio_irq = NULL;
                    
                    return 1;
                }
                for (i=0; i<times; i++)
                {
                    uvis25_interface_delay_ms(2000);
                    res = uvis25_interrupt_read((float *)&uv);
                    if (res)
                    {
                        uvis25_interrupt_deinit();
                        gpio_interrupt_deinit();
                        g_gpio_irq = NULL;
                        
                        return 1;
                    }
                    uvis25_interface_debug_print("uvis25: %d/%d.\n", i+1, times);
                    uvis25_interface_debug_print("uvis25: uv is %0.4f.\n", uv);
                    if (g_flag)
                    {
                        uvis25_interface_debug_print("uvis25: find interrupt.\n");
                        
                        break;
                    }
                }
                uvis25_interrupt_deinit();
                gpio_interrupt_deinit();
                g_gpio_irq = NULL;
                
                return 0;
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* param is invalid */
        else
        {
            return 5;
        }
    }
    /* param is invalid */
    else
    {
        return 5;
    }
}

/**
 * @brief     main function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *             - 0 success
 * @note      none
 */
int main(uint8_t argc, char **argv)
{
    uint8_t res;

    res = uvis25(argc, argv);
    if (res == 0)
    {
        /* run success */
    }
    else if (res == 1)
    {
        uvis25_interface_debug_print("uvis25: run failed.\n");
    }
    else if (res == 5)
    {
        uvis25_interface_debug_print("uvis25: param is invalid.\n");
    }
    else
    {
        uvis25_interface_debug_print("uvis25: unknow status code.\n");
    }

    return 0;
}
