[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver UVIS25

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/uvis25/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

UVIS25は、太陽光からの紫外線放射指数（UVI）の正確な測定を提供できるデジタルUV指数センサーです。センシング素子とミックスドシグナルASICを搭載し、I2CおよびSPIインターフェースを介してUVインデックスデータを提供します。UVインデックス測定の最高の精度を実現するために、専用のテクノロジーが開発されました。 UVIS25を使用すると、UVインデックスを計算するための専用のアルゴリズムや、顧客の製造ラインでの特定のキャリブレーションを必要とせずに、UVインデックスを測定できます。デバイスは、しきい値の超過に基づいて、または新しいデータセットが生成されたときに割り込みイベントを生成するように構成できます。イベントは、レジスターと専用ピンで利用できます。 UVIS25は、フルモールドLGAパッケージ（LGA）で提供されます。 -20°Cから+85°Cまでの温度範囲で動作することが保証されています。パッケージは透明で、外部の太陽放射が検知要素に到達できるようになっています。

LibDriver UVIS25は、LibDriverによって起動されたUVIS25の全機能ドライバーであり、UVインデックスの連続読み取り、UVインデックスの単一読み取り、UVインデックスしきい値の中断などの機能を提供します。 LibDriverはMISRAに準拠しています。

### 目次

  - [説明](#説明)
  - [インストール](#インストール)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example shot](#example-shot)
    - [example interrupt](#example-interrupt)
  - [ドキュメント](#ドキュメント)
  - [貢献](#貢献)
  - [著作権](#著作権)
  - [連絡して](#連絡して)

### 説明

/ srcディレクトリには、LibDriver UVIS25のソースファイルが含まれています。

/ interfaceディレクトリには、LibDriver UVIS25用のプラットフォームに依存しないIIC, SPIバステンプレートが含まれています。

/ testディレクトリには、チップの必要な機能を簡単にテストできるLibDriver UVIS25ドライバーテストプログラムが含まれています。

/ exampleディレクトリには、LibDriver UVIS25プログラミング例が含まれています。

/ docディレクトリには、LibDriver UVIS25オフラインドキュメントが含まれています。

/ datasheetディレクトリには、UVIS25データシートが含まれています。

/ projectディレクトリには、一般的に使用されるLinuxおよびマイクロコントローラー開発ボードのプロジェクトサンプルが含まれています。 すべてのプロジェクトは、デバッグ方法としてシェルスクリプトを使用しています。詳細については、各プロジェクトのREADME.mdを参照してください。

/ misraはLibDriver misraコードスキャン結果を含む。

### インストール

/ interfaceディレクトリにあるプラットフォームに依存しないIIC, SPIバステンプレートを参照して、指定したプラットフォームのIIC, SPIバスドライバを完成させます。

/src ディレクトリ、プラットフォームのインターフェイス ドライバー、および独自のドライバーをプロジェクトに追加します。デフォルトのサンプル ドライバーを使用する場合は、/example ディレクトリをプロジェクトに追加します。

### 使用

/example ディレクトリ内のサンプルを参照して、独自のドライバーを完成させることができます。 デフォルトのプログラミング例を使用したい場合の使用方法は次のとおりです。

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

```c
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

### ドキュメント

オンラインドキュメント: [https://www.libdriver.com/docs/uvis25/index.html](https://www.libdriver.com/docs/uvis25/index.html)。

オフラインドキュメント: /doc/html/index.html。

### 貢献

CONTRIBUTING.mdを参照してください。

### 著作権

著作権（c）2015-今 LibDriver 全著作権所有

MITライセンス（MIT）

このソフトウェアおよび関連するドキュメントファイル（「ソフトウェア」）のコピーを取得した人は、無制限の使用、複製、変更、組み込み、公開、配布、サブライセンスを含む、ソフトウェアを処分する権利を制限なく付与されます。ソフトウェアのライセンスおよび/またはコピーの販売、および上記のようにソフトウェアが配布された人の権利のサブライセンスは、次の条件に従うものとします。

上記の著作権表示およびこの許可通知は、このソフトウェアのすべてのコピーまたは実体に含まれるものとします。

このソフトウェアは「現状有姿」で提供され、商品性、特定目的への適合性、および非侵害の保証を含むがこれらに限定されない、明示または黙示を問わず、いかなる種類の保証もありません。 いかなる場合も、作者または著作権所有者は、契約、不法行為、またはその他の方法で、本ソフトウェアおよび本ソフトウェアの使用またはその他の廃棄に起因または関連して、請求、損害、またはその他の責任を負わないものとします。

### 連絡して

お問い合わせくださいlishifenging@outlook.com。