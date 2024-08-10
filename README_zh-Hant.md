[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver UVIS25

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/uvis25/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

UVIS25是一種數字紫外線指數傳感器，能夠準確測量太陽光紫外線輻射指數。它包括傳感元件和混合信號ASIC並通過IIC和SPI提供紫外指數數據。 UVIS25使用一種專門的技術來實現UV指數的最大精度測量，無需專用算法計算紫外指數，無需在客戶生產線上進行校準。設備可以配置為生成基於閾值交叉或當生成一組新數據時的事件。 UVIS25有一個完整的模具LGA包裝（LGA）。

LibDriver UVIS25是LibDriver推出的UVIS25的全功能驅動，該驅動提供紫外線指數連續讀取、紫外線指數單次讀取，紫外線指數閾值中斷等功能並且它符合MISRA標準。

### 目錄

  - [說明](#說明)
  - [安裝](#安裝)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example shot](#example-shot)
    - [example interrupt](#example-interrupt)
  - [文檔](#文檔)
  - [貢獻](#貢獻)
  - [版權](#版權)
  - [聯繫我們](#聯繫我們)

### 說明

/src目錄包含了LibDriver UVIS25的源文件。

/interface目錄包含了LibDriver UVIS25與平台無關的IIC、SPI總線模板。

/test目錄包含了LibDriver UVIS25驅動測試程序，該程序可以簡單的測試芯片必要功能。

/example目錄包含了LibDriver UVIS25編程範例。

/doc目錄包含了LibDriver UVIS25離線文檔。

/datasheet目錄包含了UVIS25數據手冊。

/project目錄包含了常用Linux與單片機開發板的工程樣例。所有工程均採用shell腳本作為調試方法，詳細內容可參考每個工程裡面的README.md。

/misra目錄包含了LibDriver MISRA程式碼掃描結果。

### 安裝

參考/interface目錄下與平台無關的IIC、SPI總線模板，完成指定平台的IIC、SPI總線驅動。

將/src目錄，您使用平臺的介面驅動和您開發的驅動加入工程，如果您想要使用默認的範例驅動，可以將/example目錄加入您的工程。

### 使用

您可以參考/example目錄下的程式設計範例完成適合您的驅動，如果您想要使用默認的程式設計範例，以下是它們的使用方法。

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

### 文檔

在線文檔: [https://www.libdriver.com/docs/uvis25/index.html](https://www.libdriver.com/docs/uvis25/index.html)。

離線文檔: /doc/html/index.html。

### 貢獻

請參攷CONTRIBUTING.md。

### 版權

版權 (c) 2015 - 現在 LibDriver 版權所有

MIT 許可證（MIT）

特此免費授予任何獲得本軟件副本和相關文檔文件（下稱“軟件”）的人不受限制地處置該軟件的權利，包括不受限制地使用、複製、修改、合併、發布、分發、轉授許可和/或出售該軟件副本，以及再授權被配發了本軟件的人如上的權利，須在下列條件下：

上述版權聲明和本許可聲明應包含在該軟件的所有副本或實質成分中。

本軟件是“如此”提供的，沒有任何形式的明示或暗示的保證，包括但不限於對適銷性、特定用途的適用性和不侵權的保證。在任何情況下，作者或版權持有人都不對任何索賠、損害或其他責任負責，無論這些追責來自合同、侵權或其它行為中，還是產生於、源於或有關於本軟件以及本軟件的使用或其它處置。

### 聯繫我們

請聯繫lishifenging@outlook.com。