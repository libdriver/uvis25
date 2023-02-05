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
#include <getopt.h>
#include <stdlib.h>

volatile uint8_t g_flag;                   /**< interrupt flag */
uint8_t (*g_gpio_irq)(void) = NULL;        /**< gpio irq function address */

/**
 * @brief     interface receive callback
 * @param[in] type is the interrupt type
 * @note      none
 */
static void a_callback(uint8_t type)
{
    switch (type)
    {
        case UVIS25_INTERRUPT_ACTIVE :
        {
            uvis25_interface_debug_print("uvis25: active interrupt.\n");
            g_flag = 1;

            break;
        }
        case UVIS25_INTERRUPT_HIGHER :
        {
            uvis25_interface_debug_print("uvis25: high threshold interrupt.\n");
            g_flag = 1;

            break;
        }
        case UVIS25_INTERRUPT_LOWER :
        {
            uvis25_interface_debug_print("uvis25: low threshold interrupt.\n");
            g_flag = 1;
            
            break;
        }
        default :
        {
            uvis25_interface_debug_print("uvis25: unknown code.\n");
            
            break;
        }
    }
}

/**
 * @brief     uvis25 full function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 5 param is invalid
 * @note      none
 */
uint8_t uvis25(uint8_t argc, char **argv)
{
    int c;
    int longindex = 0;
    const char short_options[] = "hipe:t:";
    const struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {"interface", required_argument, NULL, 1},
        {"mode", required_argument, NULL, 2},
        {"threshold", required_argument, NULL, 3},
        {"times", required_argument, NULL, 4},
        {NULL, 0, NULL, 0},
    };
    char type[33] = "unknown";
    uint32_t times = 3;
    uvis25_interrupt_type_t mode = UVIS25_INTERRUPT_TYPE_UV_INDEX_HIGH;
    uvis25_interface_t interface = UVIS25_INTERFACE_IIC;
    float threshold = 0.5f;
    
    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }
    
    /* init 0 */
    optind = 0;
    
    /* parse */
    do
    {
        /* parse the args */
        c = getopt_long(argc, argv, short_options, long_options, &longindex);
        
        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "h");
                
                break;
            }
            
            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "i");
                
                break;
            }
            
            /* port */
            case 'p' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "p");
                
                break;
            }
            
            /* example */
            case 'e' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "e_%s", optarg);
                
                break;
            }
            
            /* test */
            case 't' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "t_%s", optarg);
                
                break;
            }
            
            /* interface */
            case 1 :
            {
                /* set the interface */
                if (strcmp("iic", optarg) == 0)
                {
                    interface = UVIS25_INTERFACE_IIC;
                }
                else if (strcmp("spi", optarg) == 0)
                {
                    interface = UVIS25_INTERFACE_SPI;
                }
                else
                {
                    return 5;
                }
                
                break;
            }
            
            /* mode */
            case 2 :
            {
                /* set the mode */
                if (strcmp("READY", optarg) == 0)
                {
                    mode = UVIS25_INTERRUPT_TYPE_DATA_READY;
                }
                else if (strcmp("HIGH", optarg) == 0)
                {
                    mode = UVIS25_INTERRUPT_TYPE_UV_INDEX_HIGH;
                }
                else if (strcmp("LOW", optarg) == 0)
                {
                    mode = UVIS25_INTERRUPT_TYPE_UV_INDEX_LOW;
                }
                else if (strcmp("HIGH-OR-LOW", optarg) == 0)
                {
                    mode = UVIS25_INTERRUPT_TYPE_UV_INDEX_HIGH_LOW;
                }
                else
                {
                    return 5;
                }
                
                break;
            }
            
            /* threshold */
            case 3 :
            {
                threshold = atof(optarg);
                
                break;
            }
            
            /* running times */
            case 4 :
            {
                /* set the times */
                times = atol(optarg);
                
                break;
            } 
            
            /* the end */
            case -1 :
            {
                break;
            }
            
            /* others */
            default :
            {
                return 5;
            }
        }
    } while (c != -1);

    /* run the function */
    if (strcmp("t_reg", type) == 0)
    {
        uint8_t res;
        
        /* run reg test */
        res = uvis25_register_test(interface);
        if (res != 0)
        {
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("t_read", type) == 0)
    {
        uint8_t res;
        
        /* run read test */
        res = uvis25_read_test(interface, times);
        if (res != 0)
        {
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("t_int", type) == 0)
    {
        uint8_t res;
        
        /* set gpio irq */
        g_gpio_irq = uvis25_interrupt_test_irq_handler;
        
        /* gpio init */
        res = gpio_interrupt_init();
        if (res != 0)
        {
            return 1;
        }
        
        /* run interrupt */
        res = uvis25_interrupt_test(interface, threshold, times);
        if (res != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            
            return 1;
        }
        
        /* gpio deinit */
        (void)gpio_interrupt_deinit();
        g_gpio_irq = NULL;
        
        return 0;
    }
    else if (strcmp("e_read", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        float uv;
        
        /* basic init */
        res = uvis25_basic_init(interface);
        if (res != 0)
        {
            return 1;
        }
        
        /* loop */
        for (i = 0; i < times; i++)
        {
            /* delay 2000ms */
            uvis25_interface_delay_ms(2000);
            
            /* read data */
            res = uvis25_basic_read((float *)&uv);
            if (res != 0)
            {
                (void)uvis25_basic_deinit();
                
                return 1;
            }
            
            /* output */
            uvis25_interface_debug_print("uvis25: %d/%d.\n", i + 1, times);
            uvis25_interface_debug_print("uvis25: uv is %0.4f.\n", uv);
        }
        
        /* basic deinit */
        (void)uvis25_basic_deinit();
        
        return 0;
    }
    else if (strcmp("e_shot", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        float uv;
        
        /* shot init */
        res = uvis25_shot_init(interface);
        if (res != 0)
        {
            return 1;
        }
        
        /* loop */
        for (i = 0; i < times; i++)
        {
            /* delay 2000ms */
            uvis25_interface_delay_ms(2000);
            
            /* read data */
            res = uvis25_shot_read((float *)&uv);
            if (res != 0)
            {
                (void)uvis25_shot_deinit();
                
                return 1;
            }
            
            /* output */
            uvis25_interface_debug_print("uvis25: %d/%d.\n", i + 1, times);
            uvis25_interface_debug_print("uvis25: uv is %0.4f.\n", uv);
        }
        
        /* shot deinit */
        (void)uvis25_shot_deinit();
        
        return 0;
    }
    else if (strcmp("e_int", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        float uv;
        
        /* set gpio irq */
        g_gpio_irq = uvis25_interrupt_irq_handler;
        
        /* gpio init */
        res = gpio_interrupt_init();
        if (res != 0)
        {
            return 1;
        }
        
        /* interrupt init */
        g_flag = 0;
        res = uvis25_interrupt_init(interface, mode, threshold, a_callback);
        if (res != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            
            return 1;
        }
        
        /* loop */
        for (i = 0; i < times; i++)
        {
            /* delay 2000ms */
            uvis25_interface_delay_ms(2000);
            
            /* read data */
            res = uvis25_interrupt_read((float *)&uv);
            if (res != 0)
            {
                (void)uvis25_interrupt_deinit();
                (void)gpio_interrupt_deinit();
                g_gpio_irq = NULL;
                
                return 1;
            }
            
            /* output */
            uvis25_interface_debug_print("uvis25: %d/%d.\n", i + 1, times);
            uvis25_interface_debug_print("uvis25: uv is %0.4f.\n", uv);
            if (g_flag != 0)
            {
                uvis25_interface_debug_print("uvis25: find interrupt.\n");
                
                break;
            }
        }
        
        /* deinit */
        (void)uvis25_interrupt_deinit();
        (void)gpio_interrupt_deinit();
        g_gpio_irq = NULL;
        
        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        uvis25_interface_debug_print("Usage:\n");
        uvis25_interface_debug_print("  uvis25 (-i | --information)\n");
        uvis25_interface_debug_print("  uvis25 (-h | --help)\n");
        uvis25_interface_debug_print("  uvis25 (-p | --port)\n");
        uvis25_interface_debug_print("  uvis25 (-t reg | --test=reg) [--interface=<iic | spi>]\n");
        uvis25_interface_debug_print("  uvis25 (-t read | --test=read) [--interface=<iic | spi>] [--times=<num>]\n");
        uvis25_interface_debug_print("  uvis25 (-t int | --test=int) [--interface=<iic | spi>] [--times=<num>] [--threshold=<th>]\n");
        uvis25_interface_debug_print("  uvis25 (-e read | --example=read) [--interface=<iic | spi>] [--times=<num>]\n");
        uvis25_interface_debug_print("  uvis25 (-e shot | --example=shot) [--interface=<iic | spi>] [--times=<num>]\n");
        uvis25_interface_debug_print("  uvis25 (-e int | --example=int) [--interface=<iic | spi>] [--times=<num>] [--threshold=<th>]\n");
        uvis25_interface_debug_print("         [--mode=<READY | HIGH | LOW | HIGH-OR-LOW>]\n");
        uvis25_interface_debug_print("\n");
        uvis25_interface_debug_print("Options:\n");
        uvis25_interface_debug_print("  -e <read | shot | int>, --example=<read | shot | int>\n");
        uvis25_interface_debug_print("                          Run the driver example.\n");
        uvis25_interface_debug_print("  -h, --help              Show the help.\n");
        uvis25_interface_debug_print("  -i, --information       Show the chip information.\n");
        uvis25_interface_debug_print("      --interface=<iic | spi>\n");
        uvis25_interface_debug_print("                          Set the chip interface.([default: iic])\n");
        uvis25_interface_debug_print("      --mode=<READY | HIGH | LOW | HIGH-OR-LOW>\n");
        uvis25_interface_debug_print("                          Set the interrupt mode.([default: HIGH])\n");
        uvis25_interface_debug_print("  -p, --port              Display the pin connections of the current board.\n");
        uvis25_interface_debug_print("  -t <reg | read | int>, --test=<reg | read | int>\n");
        uvis25_interface_debug_print("                          Run the driver test.\n");
        uvis25_interface_debug_print("      --threshold=<th>    Set the interrupt threshold.([default: 0.5])\n");
        uvis25_interface_debug_print("      --times=<num>       Set the running times.([default: 3])\n");
        
        return 0;
    }
    else if (strcmp("i", type) == 0)
    {
        uvis25_info_t info;
        
        /* print uvis25 info */
        uvis25_info(&info);
        uvis25_interface_debug_print("uvis25: chip is %s.\n", info.chip_name);
        uvis25_interface_debug_print("uvis25: manufacturer is %s.\n", info.manufacturer_name);
        uvis25_interface_debug_print("uvis25: interface is %s.\n", info.interface);
        uvis25_interface_debug_print("uvis25: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        uvis25_interface_debug_print("uvis25: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        uvis25_interface_debug_print("uvis25: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        uvis25_interface_debug_print("uvis25: max current is %0.2fmA.\n", info.max_current_ma);
        uvis25_interface_debug_print("uvis25: max temperature is %0.1fC.\n", info.temperature_max);
        uvis25_interface_debug_print("uvis25: min temperature is %0.1fC.\n", info.temperature_min);
        
        return 0;
    }
    else if (strcmp("p", type) == 0)
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
        uvis25_interface_debug_print("uvis25: unknown status code.\n");
    }

    return 0;
}
