[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver UVIS25

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/uvis25/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE) 

Der UVIS25 ist ein digitaler UV-Index-Sensor, der eine genaue Messung des UV-Strahlungsindex (UVI) des Sonnenlichts ermöglicht. Es enthält ein Sensorelement und einen Mixed-Signal-ASIC, um die UV-Indexdaten über I2C- und SPI-Schnittstellen bereitzustellen. Es wurde eine spezielle Technologie entwickelt, um die größte Genauigkeit für UV-Indexmessungen zu erreichen. Das UVIS25 ermöglicht die Messung des UV-Index, ohne dass ein dedizierter Algorithmus zur Berechnung des UV-Index erforderlich ist und ohne spezifische Kalibrierungen an der Fertigungslinie des Kunden. Das Gerät kann so konfiguriert werden, dass Interrupt-Ereignisse basierend auf einer Schwellenwertüberschreitung oder wenn ein neuer Datensatz generiert wird, generiert werden. Die Veranstaltung ist sowohl in einem Register als auch auf einem dedizierten Pin verfügbar. Der UVIS25 ist in einem Full-Mold-LGA-Gehäuse (LGA) erhältlich. Der Betrieb über einen Temperaturbereich von -20 °C bis +85 °C ist garantiert. Das Gehäuse ist transparent, damit externe Sonnenstrahlung das Sensorelement erreichen kann.

LibDriver UVIS25 ist der voll funktionsfähige Treiber von UVIS25, der von LibDriver gestartet wurde. Er bietet kontinuierliches Lesen des UV-Index, einmaliges Lesen des UV-Index, Unterbrechung des UV-Index-Schwellenwerts und andere Funktionen. LibDriver ist MISRA-konform.

### Inhaltsverzeichnis

  - [Anweisung](#Anweisung)
  - [Installieren](#Installieren)
  - [Nutzung](#Nutzung)
    - [example basic](#example-basic)
    - [example shot](#example-shot)
    - [example interrupt](#example-interrupt)
  - [Dokument](#Dokument)
  - [Beitrag](#Beitrag)
  - [Lizenz](#Lizenz)
  - [Kontaktieren Sie uns](#Kontaktieren-Sie-uns)

### Anweisung

/src enthält LibDriver UVIS25-Quelldateien.

/interface enthält die plattformunabhängige Vorlage LibDriver UVIS25 IIC, SPI.

/test enthält den Testcode des LibDriver UVIS25-Treibers und dieser Code kann die erforderliche Funktion des Chips einfach testen.

/example enthält LibDriver UVIS25-Beispielcode.

/doc enthält das LibDriver UVIS25-Offlinedokument.

/Datenblatt enthält UVIS25-Datenblatt.

/project enthält den allgemeinen Beispielcode für Linux- und MCU-Entwicklungsboards. Alle Projekte verwenden das Shell-Skript, um den Treiber zu debuggen, und die detaillierten Anweisungen finden Sie in der README.md jedes Projekts.

/misra enthält die Ergebnisse des LibDriver MISRA Code Scans.

### Installieren

Verweisen Sie auf eine plattformunabhängige IIC, SPI-Schnittstellenvorlage und stellen Sie Ihren Plattform-IIC, SPI-Treiber fertig.

Fügen Sie das Verzeichnis /src, den Schnittstellentreiber für Ihre Plattform und Ihre eigenen Treiber zu Ihrem Projekt hinzu. Wenn Sie die Standardbeispieltreiber verwenden möchten, fügen Sie das Verzeichnis /example zu Ihrem Projekt hinzu.

### Nutzung

Sie können auf die Beispiele im Verzeichnis /example zurückgreifen, um Ihren eigenen Treiber zu vervollständigen. Wenn Sie die Standardprogrammierbeispiele verwenden möchten, erfahren Sie hier, wie Sie diese verwenden.

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

### Dokument

Online-Dokumente: [https://www.libdriver.com/docs/uvis25/index.html](https://www.libdriver.com/docs/uvis25/index.html).

Offline-Dokumente: /doc/html/index.html.

### Beitrag

Bitte beachten Sie CONTRIBUTING.md.

### Lizenz

Urheberrechte © (c) 2015 - Gegenwart LibDriver Alle Rechte vorbehalten



Die MIT-Lizenz (MIT)



Hiermit wird jeder Person kostenlos die Erlaubnis erteilt, eine Kopie zu erhalten

dieser Software und zugehörigen Dokumentationsdateien (die „Software“) zu behandeln

in der Software ohne Einschränkung, einschließlich, aber nicht beschränkt auf die Rechte

zu verwenden, zu kopieren, zu modifizieren, zusammenzuführen, zu veröffentlichen, zu verteilen, unterzulizenzieren und/oder zu verkaufen

Kopien der Software und Personen, denen die Software gehört, zu gestatten

dazu eingerichtet werden, unter folgenden Bedingungen:



Der obige Urheberrechtshinweis und dieser Genehmigungshinweis müssen in allen enthalten sein

Kopien oder wesentliche Teile der Software.



DIE SOFTWARE WIRD "WIE BESEHEN" BEREITGESTELLT, OHNE JEGLICHE GEWÄHRLEISTUNG, AUSDRÜCKLICH ODER

STILLSCHWEIGEND, EINSCHLIESSLICH, ABER NICHT BESCHRÄNKT AUF DIE GEWÄHRLEISTUNG DER MARKTGÄNGIGKEIT,

EIGNUNG FÜR EINEN BESTIMMTEN ZWECK UND NICHTVERLETZUNG VON RECHTEN DRITTER. IN KEINEM FALL DARF DAS

AUTOREN ODER URHEBERRECHTSINHABER HAFTEN FÜR JEGLICHE ANSPRÜCHE, SCHÄDEN ODER ANDERE

HAFTUNG, OB AUS VERTRAG, DELIKT ODER ANDERWEITIG, ENTSTEHEND AUS,

AUS ODER IM ZUSAMMENHANG MIT DER SOFTWARE ODER DER VERWENDUNG ODER ANDEREN HANDLUNGEN MIT DER

SOFTWARE.

### Kontaktieren Sie uns

Bitte senden Sie eine E-Mail an lishifenging@outlook.com.