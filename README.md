[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver UVIS25

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/uvis25/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

The UVIS25 is a digital UV index sensor able to provide an accurate measurement of the ultraviolet radiation index (UVI) from sunlight. It includes a sensing element and a mixed signal ASIC to provide the UV index data through I2C and SPI interfaces.A dedicated technology has been developed to achieve the greatest accuracy for UV index measurements. The UVIS25 allows the measurement of the UV index, without the need for a dedicated algorithm to calculate the UV index and without specific calibrations at the customer manufacturing line. The device can be configured to generate interrupt events based on a threshold crossing or when a new set of data is generated. The event is available in a register as well as on a dedicated pin. The UVIS25 is available in a full-mold LGA package (LGA). It is guaranteed to operate over a temperature range extending from -20 °C to +85 °C. The package is transparent to allow external solar radiation to reach the sensing element.

LibDriver UVIS25 is the full function driver of UVIS25 launched by LibDriver.It provides continuous reading of UV index, single reading of UV index, interruption of UV index threshold and other functions. LibDriver is MISRA compliant.

### Table of Contents

  - [Instruction](#Instruction)
  - [Install](#Install)
  - [Usage](#Usage)
    - [example basic](#example-basic)
    - [example shot](#example-shot)
    - [example interrupt](#example-interrupt)
  - [Document](#Document)
  - [Contributing](#Contributing)
  - [License](#License)
  - [Contact Us](#Contact-Us)

### Instruction

/src includes LibDriver UVIS25 source files.

/interface includes LibDriver UVIS25 IIC and SPI platform independent template.

/test includes LibDriver UVIS25 driver test code and this code can test the chip necessary function simply.

/example includes LibDriver UVIS25 sample code.

/doc includes LibDriver UVIS25 offline document.

/datasheet includes UVIS25 datasheet.

/project includes the common Linux and MCU development board sample code. All projects use the shell script to debug the driver and the detail instruction can be found in each project's README.md.

/misra includes the LibDriver MISRA code scanning results.

### Install

Reference /interface IIC and SPI platform independent template and finish your platform IIC and SPI driver.

Add the /src directory, the interface driver for your platform, and your own drivers to your project, if you want to use the default example drivers, add the /example directory to your project.

### Usage

You can refer to the examples in the /example directory to complete your own driver. If you want to use the default programming examples, here's how to use them.

#### example basic

```C
#include "driver_uvis25_basic.h"

uint8_t res;
uint32_t i;
float uv;

res = uvis25_basic_init(UVIS25_INTERFACE_IIC);
if (res != 0)
{
    return 1;
}

...

for (i = 0; i < 3; i++)
{
    uvis25_interface_delay_ms(2000);
    res = uvis25_basic_read((float *)&uv);
    if (res != 0)
    {
        (void)uvis25_basic_deinit();

        return 1;
    }
    uvis25_interface_debug_print("uvis25: uv is %0.4f.\n", uv);
    
    ...
    
}

...

(void)uvis25_basic_deinit();

return 0;
```

#### example shot

```C
#include "driver_uvis25_shot.h"

uint8_t res;
uint8_t i;
float uv;

res = uvis25_shot_init(UVIS25_INTERFACE_IIC);
if (res != 0)
{
    return 1;
}

...

for (i=0; i<times; i++)
{
    uvis25_interface_delay_ms(2000);
    res = uvis25_shot_read((float *)&uv);
    if (res != 0)
    {
        (void)uvis25_shot_deinit();

        return 1;
    }
    uvis25_interface_debug_print("uvis25: uv is %0.4f.\n", uv);
    
    ...
    
}

...

(void)uvis25_shot_deinit();

return 0;
```

#### example interrupt

```C
#include "driver_uvis25_interrupt.h"

uint8_t res;
uint8_t i;
float uv;
uint8_t g_flag;

void gpio_irq(void)
{
    g_flag = 1;
}

res = gpio_interrupt_init();
if (res != 0)
{
    return 1;
}
res = uvis25_interrupt_init(UVIS25_INTERFACE_IIC, UVIS25_INTERRUPT_TYPE_UV_INDEX_HIGH_LOW, 1.2f);
if (res != 0)
{
    (void)gpio_interrupt_deinit();

    return 1;
}

...

g_flag = 0;
for (i = 0; i < 3; i++)
{
    uvis25_interface_delay_ms(2000);
    res = uvis25_interrupt_read((float *)&uv);
    if (res != 0)
    {
        (void)uvis25_interrupt_deinit();
        (void)gpio_interrupt_deinit();

        return 1;
    }
    uvis25_interface_debug_print("uvis25: uv is %0.4f.\n", uv);
    
    ....
    
    if (g_flag != 0)
    {
        uvis25_interface_debug_print("uvis25: find interrupt.\n");

        break;
    }
    
    ...
    
}

...

(void)uvis25_interrupt_deinit();
(void)gpio_interrupt_deinit();;

return 0;
```

### Document

Online documents: [https://www.libdriver.com/docs/uvis25/index.html](https://www.libdriver.com/docs/uvis25/index.html).

Offline documents: /doc/html/index.html.

### Contributing

Please refer to CONTRIBUTING.md.

### License

Copyright (c) 2015 - present LibDriver All rights reserved



The MIT License (MIT) 



Permission is hereby granted, free of charge, to any person obtaining a copy

of this software and associated documentation files (the "Software"), to deal

in the Software without restriction, including without limitation the rights

to use, copy, modify, merge, publish, distribute, sublicense, and/or sell

copies of the Software, and to permit persons to whom the Software is

furnished to do so, subject to the following conditions: 



The above copyright notice and this permission notice shall be included in all

copies or substantial portions of the Software. 



THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR

IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,

FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE

AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER

LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,

OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE

SOFTWARE. 

### Contact Us

Please send an e-mail to lishifenging@outlook.com.