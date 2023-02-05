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
 * @file      driver_uvis25.c
 * @brief     driver uvis25 source file
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

#include "driver_uvis25.h"

/**
 * @brief chip register definition
 */
#define CHIP_NAME                 "STMicroelectronics UVIS25"        /**< chip name */
#define MANUFACTURER_NAME         "STMicroelectronics"               /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        1.7f                               /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        3.6f                               /**< chip max supply voltage */
#define MAX_CURRENT               0.01f                              /**< chip max current */
#define TEMPERATURE_MIN           -20.0f                             /**< chip min operating temperature */
#define TEMPERATURE_MAX           85.0f                              /**< chip max operating temperature */
#define DRIVER_VERSION            2000                               /**< driver version */

/**
 * @brief chip register definition
 */
#define UVIS25_REG_WHO_AM_I          0x0F        /**< who am i register */
#define UVIS25_REG_CTRL_REG1         0x20        /**< control 1 register */
#define UVIS25_REG_CTRL_REG2         0x21        /**< control 2 register */
#define UVIS25_REG_CTRL_REG3         0x22        /**< control 3 register */
#define UVIS25_REG_INT_CFG           0x23        /**< interrupt configure register */
#define UVIS25_REG_INT_SOURCE        0x24        /**< interrupt source register */
#define UVIS25_REG_THS_UV            0x25        /**< threshold uv register */
#define UVIS25_REG_STATUS_REG        0x27        /**< status register */
#define UVIS25_REG_UV_OUT_REG        0x28        /**< uv output register */

/**
 * @brief iic address definition
 */
#define UVIS25_IIC_ADDRESS           0x8E        /**< iic device address */

/**
 * @brief      iic or spi interface read bytes
 * @param[in]  *handle points to a uvis25 handle structure
 * @param[in]  reg is the iic register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
static uint8_t a_uvis25_iic_spi_read(uvis25_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle->iic_spi == UVIS25_INTERFACE_IIC)                           /* iic interface */
    {
        if (handle->iic_read(UVIS25_IIC_ADDRESS, reg, buf, len) != 0)      /* read register */
        {
            return 1;                                                      /* return error */
        }
        else
        {
            return 0;                                                      /* success return 0 */
        }
    }
    else                                                                   /* spi interface */
    {
        if (len > 1)                                                       /* if length > 1 */
        {
            reg |= 1 << 6;                                                 /* set read more than 1 byte */
        }
        reg |= 1 << 7;                                                     /* set read mode */
        
        if (handle->spi_read(reg, buf, len) != 0)                          /* read register */
        {
            return 1;                                                      /* return error */
        }
        else
        {
            return 0;                                                      /* success return 0 */
        }
    }
}

/**
 * @brief     iic or spi interface write bytes
 * @param[in] *handle points to a uvis25 handle structure
 * @param[in] reg is the iic register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
static uint8_t a_uvis25_iic_spi_write(uvis25_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle->iic_spi == UVIS25_INTERFACE_IIC)                            /* iic interface */
    {
        if (handle->iic_write(UVIS25_IIC_ADDRESS, reg, buf, len) != 0)      /* write register */
        {
            return 1;                                                       /* return error */
        }
        else
        {
            return 0;                                                       /* success return 0 */
        }
    }
    else                                                                    /* spi interface */
    {
        if (len > 1)                                                        /* if length > 1 */
        {
            reg |= 1 << 6;                                                  /* set write more than 1 byte */
        }
        
        if (handle->spi_write(reg, buf, len) != 0)                          /* write register */
        {
            return 1;                                                       /* return error */
        }
        else
        {
            return 0;                                                       /* success return 0 */
        }
    }
}

/**
 * @brief     set the chip interface
 * @param[in] *handle points to a uvis25 handle structure
 * @param[in] interface is the chip interface
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t uvis25_set_interface(uvis25_handle_t *handle, uvis25_interface_t interface) 
{
    if (handle == NULL)                          /* check handle */
    {
        return 2;                                /* return error */
    }
    
    handle->iic_spi = (uint8_t)interface;        /* set interface */
    
    return 0;                                    /* success return 0 */
}

/**
 * @brief      get the chip interface
 * @param[in]  *handle points to a uvis25 handle structure
 * @param[out] *interface points to a chip interface
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t uvis25_get_interface(uvis25_handle_t *handle, uvis25_interface_t *interface) 
{
    if (handle == NULL)                                        /* check handle */
    {
        return 2;                                              /* return error */
    }
    
    *interface = (uvis25_interface_t)(handle->iic_spi);        /* get interface */
    
    return 0;                                                  /* success return 0 */
}

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
uint8_t uvis25_set_block_data_update(uvis25_handle_t *handle, uvis25_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }   
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }  
    
    res = a_uvis25_iic_spi_read(handle, UVIS25_REG_CTRL_REG1, (uint8_t *)&prev, 1);         /* read ctrl reg1 */
    if (res != 0)                                                                           /* check result */
    {
        handle->debug_print("uvis25: read register failed.\n");                             /* read register failed */
       
        return 1;                                                                           /* return error */
    }
    prev &= ~(1 << 1);                                                                      /* clear enable bit */
    prev |= enable << 1;                                                                    /* set enable */
    
    return a_uvis25_iic_spi_write(handle, UVIS25_REG_CTRL_REG1, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t uvis25_get_block_data_update(uvis25_handle_t *handle, uvis25_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }   
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }  
    
    res = a_uvis25_iic_spi_read(handle, UVIS25_REG_CTRL_REG1, (uint8_t *)&prev, 1);       /* read reg */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("uvis25: read register failed.\n");                           /* read register failed */
       
        return 1;                                                                         /* return error */
    }
    *enable = (uvis25_bool_t)((prev >> 1) & 0x01);                                        /* get bool */
    
    return 0;                                                                             /* success return 0 */
}

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
uint8_t uvis25_set_boot(uvis25_handle_t *handle, uvis25_boot_mode_t mode)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }   
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }  
    
    res = a_uvis25_iic_spi_read(handle, UVIS25_REG_CTRL_REG2, (uint8_t *)&prev, 1);         /* read ctrl reg2 */
    if (res != 0)                                                                           /* check result */
    {
        handle->debug_print("uvis25: read register failed.\n");                             /* read register failed */
       
        return 1;                                                                           /* return error */
    }
    prev &= ~(1 << 7);                                                                      /* clear boot mode bit */
    prev |= mode << 7;                                                                      /* set boot mode */
    
    return a_uvis25_iic_spi_write(handle, UVIS25_REG_CTRL_REG2, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t uvis25_get_boot(uvis25_handle_t *handle, uvis25_boot_mode_t *mode)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }   
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }  
    
    res = a_uvis25_iic_spi_read(handle, UVIS25_REG_CTRL_REG2, (uint8_t *)&prev, 1);       /* read ctrl reg2 */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("uvis25: read register failed.\n");                           /* read register failed */
       
        return 1;                                                                         /* return error */
    }
    *mode = (uvis25_boot_mode_t)((prev >> 7) & 0x01);                                     /* get mode */
    
    return 0;                                                                             /* success return 0 */
}

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
uint8_t uvis25_set_iic(uvis25_handle_t *handle, uvis25_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }   
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }  
    
    res = a_uvis25_iic_spi_read(handle, UVIS25_REG_CTRL_REG2, (uint8_t *)&prev, 1);         /* read ctrl reg2 */
    if (res != 0)                                                                           /* check result */
    {
        handle->debug_print("uvis25: read register failed.\n");                             /* read register failed */
       
        return 1;                                                                           /* return error */
    }
    prev &= ~(1 << 4);                                                                      /* clear iic enable bit */
    prev |= (!enable) << 4;                                                                 /* set iic enable */
    
    return a_uvis25_iic_spi_write(handle, UVIS25_REG_CTRL_REG2, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t uvis25_get_iic(uvis25_handle_t *handle, uvis25_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }   
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }  
    
    res = a_uvis25_iic_spi_read(handle, UVIS25_REG_CTRL_REG2, (uint8_t *)&prev, 1);       /* read ctrl reg2 */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("uvis25: read register failed.\n");                           /* read register failed */
       
        return 1;                                                                         /* return error */
    }
    *enable = (uvis25_bool_t)!((prev >> 4) & 0x01);                                       /* get bool */
    
    return 0;                                                                             /* success return 0 */
}

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
uint8_t uvis25_set_spi_wire(uvis25_handle_t *handle, uvis25_spi_wire_t wire)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }   
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }  
    
    res = a_uvis25_iic_spi_read(handle, UVIS25_REG_CTRL_REG2, (uint8_t *)&prev, 1);         /* read ctrl reg2 */
    if (res != 0)                                                                           /* check result */
    {
        handle->debug_print("uvis25: read register failed.\n");                             /* read register failed */
       
        return 1;                                                                           /* return error */
    }
    prev &= ~(1 << 3);                                                                      /* clear wire bit */
    prev |= wire << 3;                                                                      /* set wire */
    
    return a_uvis25_iic_spi_write(handle, UVIS25_REG_CTRL_REG2, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t uvis25_get_spi_wire(uvis25_handle_t *handle, uvis25_spi_wire_t *wire)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }   
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }  
    
    res = a_uvis25_iic_spi_read(handle, UVIS25_REG_CTRL_REG2, (uint8_t *)&prev, 1);       /* read ctrl reg2 */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("uvis25: read register failed.\n");                           /* read failed */
       
        return 1;                                                                         /* return error */
    }
    *wire = (uvis25_spi_wire_t)((prev>>3) & 0x01);                                        /* get bool */
    
    return 0;                                                                             /* success return 0 */
}

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
uint8_t uvis25_set_interrupt_active_level(uvis25_handle_t *handle, uvis25_interrupt_active_level_t level)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }   
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }  
    
    res = a_uvis25_iic_spi_read(handle, UVIS25_REG_CTRL_REG3, (uint8_t *)&prev, 1);         /* read ctrl reg3 */
    if (res != 0)                                                                           /* check result */
    {
        handle->debug_print("uvis25: read register failed.\n");                             /* read failed */
       
        return 1;                                                                           /* return error */
    }
    prev &= ~(1 << 7);                                                                      /* clear level bit */
    prev |= level << 7;                                                                     /* set level */
    
    return a_uvis25_iic_spi_write(handle, UVIS25_REG_CTRL_REG3, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t uvis25_get_interrupt_active_level(uvis25_handle_t *handle, uvis25_interrupt_active_level_t *level)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }   
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }  
    
    res = a_uvis25_iic_spi_read(handle, UVIS25_REG_CTRL_REG3, (uint8_t *)&prev, 1);       /* read ctrl reg3 */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("uvis25: read register failed.\n");                           /* read failed */
       
        return 1;                                                                         /* return error */
    }
    *level = (uvis25_interrupt_active_level_t)((prev >> 7) & 0x01);                       /* get bool */
    
    return 0;                                                                             /* success return 0 */
}

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
uint8_t uvis25_set_interrupt_pin_type(uvis25_handle_t *handle, uvis25_interrupt_pin_type_t pin_type)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }   
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }  
    
    res = a_uvis25_iic_spi_read(handle, UVIS25_REG_CTRL_REG3, (uint8_t *)&prev, 1);         /* read ctrl reg3 */
    if (res != 0)                                                                           /* check result */
    {
        handle->debug_print("uvis25: read register failed.\n");                             /* read failed */
       
        return 1;                                                                           /* return error */
    }
    prev &= ~(1 << 6);                                                                      /* clear pin type bit */
    prev |= pin_type << 6;                                                                  /* set pin type */
    
    return a_uvis25_iic_spi_write(handle, UVIS25_REG_CTRL_REG3, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t uvis25_get_interrupt_pin_type(uvis25_handle_t *handle, uvis25_interrupt_pin_type_t *pin_type)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }   
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }  
    
    res = a_uvis25_iic_spi_read(handle, UVIS25_REG_CTRL_REG3, (uint8_t *)&prev, 1);       /* read ctrl reg3 */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("uvis25: read register failed.\n");                           /* read failed */
       
        return 1;                                                                         /* return error */
    }
    *pin_type = (uvis25_interrupt_pin_type_t)((prev >> 6) & 0x01);                        /* get bool */
    
    return 0;                                                                             /* success return 0 */
}

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
uint8_t uvis25_set_interrupt_type(uvis25_handle_t *handle, uvis25_interrupt_type_t type)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }   
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }  
    
    res = a_uvis25_iic_spi_read(handle, UVIS25_REG_CTRL_REG3, (uint8_t *)&prev, 1);         /* read ctrl reg3 */
    if (res != 0)                                                                           /* check result */
    {
        handle->debug_print("uvis25: read register failed.\n");                             /* read failed */
       
        return 1;                                                                           /* return error */
    }
    prev &= ~(0x03 << 0);                                                                   /* clear interrupt type bits */
    prev |= type << 0;                                                                      /* set interrupt type */
    
    return a_uvis25_iic_spi_write(handle, UVIS25_REG_CTRL_REG3, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t uvis25_get_interrupt_type(uvis25_handle_t *handle, uvis25_interrupt_type_t *type)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }   
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }  
    
    res = a_uvis25_iic_spi_read(handle, UVIS25_REG_CTRL_REG3, (uint8_t *)&prev, 1);       /* read ctrl reg3 */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("uvis25: read register failed.\n");                           /* read failed */
       
        return 1;                                                                         /* return error */
    }
    *type = (uvis25_interrupt_type_t)((prev >> 0) & 0x03);                                /* get interrupt type */
    
    return 0;                                                                             /* success return 0 */
}

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
uint8_t uvis25_set_interrupt(uvis25_handle_t *handle, uvis25_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }  
    
    res = a_uvis25_iic_spi_read(handle, UVIS25_REG_INT_CFG, (uint8_t *)&prev, 1);         /* read int cfg */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("uvis25: read register failed.\n");                           /* read failed */
       
        return 1;                                                                         /* return error */
    }
    prev &= ~(0x01 << 3);                                                                 /* clear interrupt enable bit */
    prev |= enable << 3;                                                                  /* set interrupt enable */
    
    return a_uvis25_iic_spi_write(handle, UVIS25_REG_INT_CFG, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t uvis25_get_interrupt(uvis25_handle_t *handle, uvis25_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;

    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }   
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }  
    
    res = a_uvis25_iic_spi_read(handle, UVIS25_REG_INT_CFG, (uint8_t *)&prev, 1);       /* read int cfg */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("uvis25: read register failed.\n");                         /* read failed */
       
        return 1;                                                                       /* return error */
    }
    *enable = (uvis25_bool_t)((prev >> 3) & 0x01);                                      /* get value */
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t uvis25_set_latch_interrupt(uvis25_handle_t *handle, uvis25_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }   
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }  
    
    res = a_uvis25_iic_spi_read(handle, UVIS25_REG_INT_CFG, (uint8_t *)&prev, 1);         /* read int cfg */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("uvis25: read register failed.\n");                           /* read failed */
       
        return 1;                                                                         /* return error */
    }
    prev &= ~(0x01 << 2);                                                                 /* clear latch interrupt enable bit */
    prev |= enable << 2;                                                                  /* set latch interrupt enable */
    
    return a_uvis25_iic_spi_write(handle, UVIS25_REG_INT_CFG, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t uvis25_get_latch_interrupt(uvis25_handle_t *handle, uvis25_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }   
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }  
    
    res = a_uvis25_iic_spi_read(handle, UVIS25_REG_INT_CFG, (uint8_t *)&prev, 1);       /* read int cfg */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("uvis25: read register failed.\n");                         /* read failed */
       
        return 1;                                                                       /* return error */
    }
    *enable = (uvis25_bool_t)((prev >> 2) & 0x01);                                      /* get value */
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t uvis25_set_interrupt_low_threshold(uvis25_handle_t *handle, uvis25_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }   
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }  
    
    res = a_uvis25_iic_spi_read(handle, UVIS25_REG_INT_CFG, (uint8_t *)&prev, 1);         /* read int cfg */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("uvis25: read register failed.\n");                           /* read failed */
       
        return 1;                                                                         /* return error */
    }
    prev &= ~(0x01 << 1);                                                                 /* clear interrupt low threshold bit */
    prev |= enable << 1;                                                                  /* set interrupt low threshold */
    
    return a_uvis25_iic_spi_write(handle, UVIS25_REG_INT_CFG, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t uvis25_get_interrupt_low_threshold(uvis25_handle_t *handle, uvis25_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }   
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }  
    
    res = a_uvis25_iic_spi_read(handle, UVIS25_REG_INT_CFG, (uint8_t *)&prev, 1);       /* read int cfg */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("uvis25: read register failed.\n");                         /* read failed */
       
        return 1;                                                                       /* return error */
    }
    *enable = (uvis25_bool_t)((prev >> 1) & 0x01);                                      /* get value */
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t uvis25_set_interrupt_high_threshold(uvis25_handle_t *handle, uvis25_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }   
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }  
    
    res = a_uvis25_iic_spi_read(handle, UVIS25_REG_INT_CFG, (uint8_t *)&prev, 1);         /* read int cfg */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("uvis25: read register failed.\n");                           /* read failed */
       
        return 1;                                                                         /* return error */
    }
    prev &= ~(0x01 << 0);                                                                 /* clear interrupt high threshold enable bit */
    prev |= enable << 0;                                                                  /* set interrupt high threshold enable */
    
    return a_uvis25_iic_spi_write(handle, UVIS25_REG_INT_CFG, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t uvis25_get_interrupt_high_threshold(uvis25_handle_t *handle, uvis25_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }   
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }  
    
    res = a_uvis25_iic_spi_read(handle, UVIS25_REG_INT_CFG, (uint8_t *)&prev, 1);       /* read int cfg */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("uvis25: read register failed.\n");                         /* read failed */
       
        return 1;                                                                       /* return error */
    }
    *enable = (uvis25_bool_t)((prev >> 0) & 0x01);                                      /* get value */
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t uvis25_set_threshold(uvis25_handle_t *handle, uint8_t threshold)
{
    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }   
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }  
    
    return a_uvis25_iic_spi_write(handle, UVIS25_REG_THS_UV, (uint8_t *)&threshold, 1);       /* write threshold */
}

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
uint8_t uvis25_get_threshold(uvis25_handle_t *handle, uint8_t *threshold)
{
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }   
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }  
    
    return a_uvis25_iic_spi_read(handle, UVIS25_REG_THS_UV, (uint8_t *)threshold, 1);       /* read threshold */
}

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
uint8_t uvis25_irq_handler(uvis25_handle_t *handle)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                    /* check handle */
    {
        return 2;                                                                          /* return error */
    }   
    if (handle->inited != 1)                                                               /* check handle initialization */
    {
        return 3;                                                                          /* return error */
    }
    
    res = a_uvis25_iic_spi_read(handle, UVIS25_REG_INT_SOURCE, (uint8_t *)&prev, 1);       /* read int source */
    if (res != 0)                                                                          /* check result */
    {
        handle->debug_print("uvis25: read register failed.\n");                            /* read failed */
       
        return 1;                                                                          /* return error */
    }
    if ((prev & (1 < 2)) != 0)                                                             /* active */
    {
        if (handle->receive_callback != NULL)                                              /* check the callback */
        {
            handle->receive_callback(UVIS25_INTERRUPT_ACTIVE);                             /* run callback */
        }
    }
    if ((prev & (1 << 1)) != 0)                                                            /* interrupt lower */
    {
        if (handle->receive_callback != NULL)                                              /* check the callback */
        {
            handle->receive_callback(UVIS25_INTERRUPT_LOWER);                              /* run callback */
        }
    }
    if ((prev & (1 << 0)) != 0)                                                            /* interrupt higher */
    {
        if (handle->receive_callback != NULL)                                              /* check the callback */
        {
            handle->receive_callback(UVIS25_INTERRUPT_HIGHER);                             /* run callback */
        }
    }    
    
    return 0;                                                                              /* success return 0 */
}

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
uint8_t uvis25_threshold_convert_to_register(uvis25_handle_t *handle, float uv, uint8_t *reg)
{
    if (handle == NULL)                   /* check handle */
    {
        return 2;                         /* return error */
    }
    if (handle->inited != 1)              /* check handle initialization */
    {
        return 3;                         /* return error */
    }
    
    *reg = (uint8_t )(uv * 16.0f);        /* convert real data to register data */
    
    return 0;                             /* success return 0 */
}

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
uint8_t uvis25_threshold_convert_to_data(uvis25_handle_t *handle, uint8_t reg, float *uv)
{
    if (handle == NULL)             /* check handle */
    {
        return 2;                   /* return error */
    }
    if (handle->inited != 1)        /* check handle initialization */
    {
        return 3;                   /* return error */
    }
    
    *uv = (float)reg / 16.0f;       /* convert raw data to real data */
    
    return 0;                       /* success return 0 */
}

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
uint8_t uvis25_init(uvis25_handle_t *handle)
{
    uint8_t id;
  
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->debug_print == NULL)                                                 /* check debug_print */
    {
        return 3;                                                                    /* return error */
    }
    if (handle->iic_init == NULL)                                                    /* check iic_init */
    {
        handle->debug_print("uvis25: iic_init is null.\n");                          /* iic_init is null */
       
        return 3;                                                                    /* return error */
    }
    if (handle->iic_deinit == NULL)                                                  /* check iic_deinit */
    {
        handle->debug_print("uvis25: iic_deinit is null.\n");                        /* iic_deinit is null */
       
        return 3;                                                                    /* return error */
    }
    if (handle->iic_read == NULL)                                                    /* check iic_read */
    {
        handle->debug_print("uvis25: iic_read is null.\n");                          /* iic_read is null */
       
        return 3;                                                                    /* return error */
    }
    if (handle->iic_write == NULL)                                                   /* check iic_write */
    {
        handle->debug_print("uvis25: iic_write is null.\n");                         /* iic_write is null */
        
        return 3;                                                                    /* return error */
    }
    if (handle->spi_init == NULL)                                                    /* check spi_init */
    {
        handle->debug_print("uvis25: spi_init is null.\n");                          /* spi_init is null */
       
        return 3;                                                                    /* return error */
    }
    if (handle->spi_deinit == NULL)                                                  /* check spi_deinit */
    {
        handle->debug_print("uvis25: spi_deinit is null.\n");                        /* spi_deinit is null */
       
        return 3;                                                                    /* return error */
    }
    if (handle->spi_read == NULL)                                                    /* check spi_read */
    {
        handle->debug_print("uvis25: spi_read is null.\n");                          /* spi_read is null */
       
        return 3;                                                                    /* return error */
    }
    if (handle->spi_write == NULL)                                                   /* check spi_write */
    {
        handle->debug_print("uvis25: spi_write is null.\n");                         /* spi_write is null */
       
        return 3;                                                                    /* return error */
    }
    if (handle->delay_ms == NULL)                                                    /* check delay_ms */
    {
        handle->debug_print("uvis25: delay_ms is null.\n");                          /* delay_ms is null */
       
        return 3;                                                                    /* return error */
    }
    
    if (handle->iic_spi == UVIS25_INTERFACE_IIC)                                     /* iic interface */
    {
        if (handle->iic_init() != 0)                                                 /* iic init */
        {
            handle->debug_print("uvis25: iic init failed.\n");                       /* iic init failed */
            
            return 1;                                                                /* return error */
        }
    }
    else                                                                             /* spi interface */
    {
        if (handle->spi_init() != 0)                                                 /* spi init */
        {
            handle->debug_print("uvis25: spi init failed.\n");                       /* spi init failed */
           
            return 1;                                                                /* return error */
        }
    }
    if (a_uvis25_iic_spi_read(handle, UVIS25_REG_WHO_AM_I, (uint8_t *)&id, 1) != 0)  /* read id */
    {
        handle->debug_print("uvis25: read id failed.\n");                            /* read id failed */
        if (handle->iic_spi == UVIS25_INTERFACE_IIC)                                 /* id iic interface */
        {
            (void)handle->iic_deinit();                                              /* iic deinit */
        }
        else
        {
            (void)handle->spi_deinit();                                              /* spi deinit */
        }
        
        return 1;                                                                    /* return error */
    }
    if (id != 0xCA)                                                                  /* check id */
    {
        handle->debug_print("uvis25: id is invalid.\n");                             /* id is invalid */
        if (handle->iic_spi == UVIS25_INTERFACE_IIC)                                 /* if iic interface */
        {
            (void)handle->iic_deinit();                                              /* iic deinit */
        }
        else
        {
            (void)handle->spi_deinit();                                              /* spi deinit */
        }
        
        return 1;                                                                    /* return error */
    }
    handle->inited = 1;                                                              /* flag finish initialization */
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t uvis25_deinit(uvis25_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                    /* check handle */
    {
        return 2;                                                                          /* return error */
    }   
    if (handle->inited != 1)                                                               /* check handle initialization */
    {
        return 3;                                                                          /* return error */
    }  
    
    res = a_uvis25_iic_spi_read(handle, UVIS25_REG_CTRL_REG2, (uint8_t *)&prev, 1);        /* read ctrl reg2 */
    if (res != 0)                                                                          /* check result */
    {
        handle->debug_print("uvis25: read register failed.\n");                            /* read failed */
       
        return 1;                                                                          /* return error */
    }
    prev &= ~(0x01 << 7);                                                                  /* clear reboot bit */
    prev |= 1 << 7;                                                                        /* set reboot */
    res = a_uvis25_iic_spi_write(handle, UVIS25_REG_CTRL_REG2, (uint8_t *)&prev, 1);       /* write config */
    if (res != 0)                                                                          /* check result */
    {
        handle->debug_print("uvis25: write register failed.\n");                           /* write ctrl reg2 failed */
       
        return 1;                                                                          /* return error */
    }
    if (handle->iic_spi == UVIS25_INTERFACE_IIC)                                           /* iic interface */
    {
        res = handle->iic_deinit();                                                        /* iic deinit */
        if (res != 0)                                                                      /* check result */
        {
            handle->debug_print("uvis25: iic deinit failed.\n");                           /* iic deinit failed */
           
            return 1;                                                                      /* return error */
        }
    }
    else                                                                                   /* spi interface */
    {
        res = handle->spi_deinit();                                                        /* spi deinit */
        if (res != 0)                                                                      /* check result */
        {
            handle->debug_print("uvis25: spi deinit failed.\n");                           /* spi deinit */
           
            return 1;                                                                      /* return error */
        }
    }
    handle->inited = 0;                                                                    /* flag close */
    
    return 0;                                                                              /* success return 0 */
}

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
uint8_t uvis25_single_read(uvis25_handle_t *handle, uint8_t *raw, float *uv)
{
    uint8_t res;
    uint8_t prev;
    uint8_t timeout;
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }   
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }  

    res = a_uvis25_iic_spi_read(handle, UVIS25_REG_CTRL_REG1, (uint8_t *)&prev, 1);              /* read ctrl reg1 */
    if (res != 0)                                                                                /* check result */
    {
        handle->debug_print("uvis25: read register failed.\n");                                  /* read failed */
       
        return 1;                                                                                /* return error */
    }
    prev &= ~(1 << 0);                                                                           /* disable continuous reading */
    res = a_uvis25_iic_spi_write(handle, UVIS25_REG_CTRL_REG1, (uint8_t *)&prev, 1);             /* write ctrl reg1 */
    if (res != 0)                                                                                /* check result */
    {
        handle->debug_print("uvis25: write register failed.\n");                                 /* write failed */
       
        return 1;                                                                                /* return error */
    }
    res = a_uvis25_iic_spi_read(handle, UVIS25_REG_CTRL_REG2, (uint8_t *)&prev, 1);              /* read ctrl reg2 */
    if (res != 0)                                                                                /* check result */
    {
        handle->debug_print("uvis25: read register failed.\n");                                  /* read ctrl reg2 failed */
       
        return 1;                                                                                /* return error */
    }
    prev &= ~(1 << 0);                                                                           /* clear one shot bit */
    prev |= 1 << 0;                                                                              /* set one shot */
    res = a_uvis25_iic_spi_write(handle, UVIS25_REG_CTRL_REG2, (uint8_t *)&prev, 1);             /* write reg2 */
    if (res != 0)                                                                                /* check result */
    {
        handle->debug_print("uvis25: write register failed.\n");                                 /* write ctrl reg2 failed */
       
        return 1;                                                                                /* return error */
    }
    timeout = 0;                                                                                 /* reset timeout times */
    
    while (1)                                                                                    /* loop */
    {
        res = a_uvis25_iic_spi_read(handle, UVIS25_REG_STATUS_REG, (uint8_t *)&prev, 1);         /* read status */
        if (res != 0)                                                                            /* check result */
        {
            handle->debug_print("uvis25: read register failed.\n");                              /* read failed */
           
            return 1;                                                                            /* return error */
        }
        if ((prev & 0x01) != 0)                                                                  /* if data ready */
        {
            res = a_uvis25_iic_spi_read(handle, UVIS25_REG_UV_OUT_REG, (uint8_t *)raw, 1);       /* read uv out */
            if (res != 0)                                                                        /* check result */
            {
                handle->debug_print("uvis25: read failed.\n");                                   /* read failed */
               
                return 1;                                                                        /* return error */
            }  
            *uv = (float)(*raw) / 16.0f;                                                         /* convert raw */
        
            return 0;                                                                            /* success return 0 */
        }
        else if (timeout < 100)                                                                  /* if not timeout */
        {
            handle->delay_ms(100);                                                               /* delay 100 ms */
            timeout++;                                                                           /* retry times++ */
            
            continue;                                                                            /* continue */
        }
        else
        {
            handle->debug_print("uvis25: data is not available.\n");                             /* data is not available */
               
            return 1;                                                                            /* return error */
        }
    }
}

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
uint8_t uvis25_start_continuous_read(uvis25_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }   
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }  

    res = a_uvis25_iic_spi_read(handle, UVIS25_REG_CTRL_REG1, (uint8_t *)&prev, 1);              /* read ctrl reg1 */
    if (res != 0)                                                                                /* check result */
    {
        handle->debug_print("uvis25: read register failed.\n");                                  /* read failed */
       
        return 1;                                                                                /* return error */
    }
    prev &= ~(1 << 0);                                                                           /* clear continuous reading */
    prev |= 1 << 0;                                                                              /* enable continuous reading */
    res = a_uvis25_iic_spi_write(handle, UVIS25_REG_CTRL_REG1, (uint8_t *)&prev, 1);             /* write ctrl reg1 */
    if (res != 0)                                                                                /* check result */
    {
        handle->debug_print("uvis25: write register failed.\n");                                 /* write failed */
       
        return 1;                                                                                /* return error */
    }
    res = a_uvis25_iic_spi_read(handle, UVIS25_REG_CTRL_REG2, (uint8_t *)&prev, 1);              /* read ctrl reg2 */
    if (res != 0)                                                                                /* check result */
    {
        handle->debug_print("uvis25: read register failed.\n");                                  /* read ctrl reg2 failed */
       
        return 1;                                                                                /* return error */
    }
    prev &= ~(1 << 0);                                                                           /* clear one shot bit */
    res = a_uvis25_iic_spi_write(handle, UVIS25_REG_CTRL_REG2, (uint8_t *)&prev, 1);             /* write reg2 */
    if (res != 0)                                                                                /* check result */
    {
        handle->debug_print("uvis25: write register failed.\n");                                 /* write ctrl reg2 failed */
       
        return 1;                                                                                /* return error */
    }
    
    return 0;                                                                                    /* success return 0 */
}

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
uint8_t uvis25_stop_continuous_read(uvis25_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }   
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }  

    res = a_uvis25_iic_spi_read(handle, UVIS25_REG_CTRL_REG1, (uint8_t *)&prev, 1);              /* read ctrl reg1 */
    if (res != 0)                                                                                /* check result */
    {
        handle->debug_print("uvis25: read register failed.\n");                                  /* read failed */
       
        return 1;                                                                                /* return error */
    }
    prev &= ~(1 << 0);                                                                           /* clear continuous reading */
    res = a_uvis25_iic_spi_write(handle, UVIS25_REG_CTRL_REG1, (uint8_t *)&prev, 1);             /* write ctrl reg1 */
    if (res != 0)                                                                                /* check result */
    {
        handle->debug_print("uvis25: write register failed.\n");                                 /* write failed */
       
        return 1;                                                                                /* return error */
    }
    res = a_uvis25_iic_spi_read(handle, UVIS25_REG_CTRL_REG2, (uint8_t *)&prev, 1);              /* read ctrl reg2 */
    if (res != 0)                                                                                /* check result */
    {
        handle->debug_print("uvis25: read register failed.\n");                                  /* read ctrl reg2 failed */
       
        return 1;                                                                                /* return error */
    }
    prev &= ~(1 << 0);                                                                           /* clear one shot bit */
    res = a_uvis25_iic_spi_write(handle, UVIS25_REG_CTRL_REG2, (uint8_t *)&prev, 1);             /* write reg2 */
    if (res != 0)                                                                                /* check result */
    {
        handle->debug_print("uvis25: write register failed.\n");                                 /* write ctrl reg2 failed */
       
        return 1;                                                                                /* return error */
    }
    
    return 0;                                                                                    /* success return 0 */
}

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
uint8_t uvis25_continuous_read(uvis25_handle_t *handle, uint8_t *raw, float *uv)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }   
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }  

    res = a_uvis25_iic_spi_read(handle, UVIS25_REG_STATUS_REG, (uint8_t *)&prev, 1);             /* read status failed */
    if (res != 0)                                                                                /* check result */
    {
        handle->debug_print("uvis25: read register failed.\n");                                  /* read failed */
       
        return 1;                                                                                /* return error */
    }
    if ((prev & 0x01) != 0)                                                                      /* check status */
    {
        res = a_uvis25_iic_spi_read(handle, UVIS25_REG_UV_OUT_REG, (uint8_t *)raw, 1);           /* read uv out reg */
        if (res != 0)                                                                            /* check result */
        {
            handle->debug_print("uvis25: read failed.\n");                                       /* read uv out failed */
           
            return 1;                                                                            /* return error */
        }  
        *uv = (float)(*raw) / 16.0f;                                                             /* convert raw data */
    
        return 0;                                                                                /* success return 0 */
    }
    else
    {
        handle->debug_print("uvis25: data is not available.\n");                                 /* data is not available */
           
        return 1;                                                                                /* return error */
    }
}

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
uint8_t uvis25_set_reg(uvis25_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                         /* check handle */
    {
        return 2;                                               /* return error */
    }
    if (handle->inited != 1)                                    /* check handle initialization */
    {
        return 3;                                               /* return error */
    }
    
    return a_uvis25_iic_spi_write(handle, reg, buf, len);       /* write data */
}

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
uint8_t uvis25_get_reg(uvis25_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                        /* check handle */
    {
        return 2;                                              /* return error */
    }
    if (handle->inited != 1)                                   /* check handle initialization */
    {
        return 3;                                              /* return error */
    }
    
    return a_uvis25_iic_spi_read(handle, reg, buf, len);       /* read data */
}

/**
 * @brief      get chip's information
 * @param[out] *info points to a uvis25 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t uvis25_info(uvis25_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(uvis25_info_t));                         /* initialize uvis25 info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "IIC SPI", 8);                         /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver version */
    
    return 0;                                                       /* success return 0 */
}
