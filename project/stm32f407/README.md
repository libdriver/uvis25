### 1. Chip

#### 1.1 Chip Info

Chip Name: STM32F407ZGT6.

Extern Oscillator: 8MHz.

UART Pin: TX/RX PA9/PA10.

IIC Pin: SCL/SDA PB8/PB9.

SPI Pin: SCK/MISO/MOSI/CS  PA5/PA6/PA7/PA4.

GPIO Pin: INT PB0.

### 2. Development and Debugging

#### 2.1 Integrated Development Environment

LibDriver provides both Keil and IAR integrated development environment projects.

MDK is the Keil ARM project and your Keil version must be 5 or higher.Keil ARM project needs STMicroelectronics STM32F4 Series Device Family Pack and you can download from https://www.keil.com/dd2/stmicroelectronics/stm32f407zgtx.

EW is the IAR ARM project and your IAR version must be 9 or higher.

#### 2.2 Serial Port Parameter

Baud Rate: 115200.

Data Bits : 8.

Stop Bits: 1.

Parity: None.

Flow Control: None.

#### 2.3 Serial Port Assistant

We use '\n' to wrap lines.If your serial port assistant displays exceptions (e.g. the displayed content does not divide lines), please modify the configuration of your serial port assistant or replace one that supports '\n' parsing.

### 3. UVIS25

#### 3.1 Command Instruction

1. Show uvis25 chip and driver information.

   ```shell
   uvis25 (-i | --information)
   ```

2. Show uvis25 help.

   ```shell
   uvis25 (-h | --help)
   ```

3. Show uvis25 pin connections of the current board.

   ```shell
   uvis25 (-p | --port)
   ```

4. Run uvis25 register test.

   ```shell
   uvis25 (-t reg | --test=reg) [--interface=<iic | spi>]
   ```

5. Run uvis25 read test, num means test times.

   ```shell
   uvis25 (-t read | --test=read) [--interface=<iic | spi>] [--times=<num>]
   ```

6. Run uvis25 interrupt test, num means test times, th means the interrupt threshold.

   ```shell
   uvis25 (-t int | --test=int) [--interface=<iic | spi>] [--times=<num>] [--threshold=<th>]
   ```

7. Run uvis25 read function, num means test times.

   ```shell
   uvis25 (-e read | --example=read) [--interface=<iic | spi>] [--times=<num>]
   ```

8. Run uvis25 shot function, num means test times.

   ```shell
   uvis25 (-e shot | --example=shot) [--interface=<iic | spi>] [--times=<num>]
   ```

9. Run uvis25 interrupt function, num means test times, th means the interrupt threshold.

   ```shell
   uvis25 (-e int | --example=int) [--interface=<iic | spi>] [--times=<num>] [--threshold=<th>] [--mode=<READY | HIGH | LOW | HIGH-OR-LOW>]
   ```

#### 3.2 Command Example

```shell
uvis25 -i

uvis25: chip is STMicroelectronics UVIS25.
uvis25: manufacturer is STMicroelectronics.
uvis25: interface is IIC SPI.
uvis25: driver version is 2.0.
uvis25: min supply voltage is 1.7V.
uvis25: max supply voltage is 3.6V.
uvis25: max current is 0.01mA.
uvis25: max temperature is 85.0C.
uvis25: min temperature is -20.0C.
```

```shell
uvis25 -p

uvis25: SPI interface SCK connected to GPIOA PIN5.
uvis25: SPI interface MISO connected to GPIOA PIN6.
uvis25: SPI interface MOSI connected to GPIOA PIN7.
uvis25: SPI interface CS connected to GPIOA PIN4.
uvis25: IIC interface SCL connected to GPIOB PIN8.
uvis25: IIC interface SDA connected to GPIOB PIN9.
uvis25: INT connected to GPIOB PIN0.
```

```shell
uvis25 -t reg --interface=iic

uvis25: chip is STMicroelectronics UVIS25.
uvis25: manufacturer is STMicroelectronics.
uvis25: interface is IIC SPI.
uvis25: driver version is 2.0.
uvis25: min supply voltage is 1.7V.
uvis25: max supply voltage is 3.6V.
uvis25: max current is 0.01mA.
uvis25: max temperature is 85.0C.
uvis25: min temperature is -20.0C.
uvis25: start register test.
uvis25: uvis25_set_interface/uvis25_get_interface test.
uvis25: set interface iic.
uvis25: check interface ok.
uvis25: set interface spi.
uvis25: check interface ok.
uvis25: uvis25_set_block_data_update/uvis25_get_block_data_update test.
uvis25: set block data update true.
uvis25: check block data update ok.
uvis25: set block data update false.
uvis25: check block data update ok.
uvis25: uvis25_set_spi_wire/uvis25_get_spi_wire test.
uvis25: set spi wire 4.
uvis25: check spi wire ok.
uvis25: set spi wire 3.
uvis25: check spi wire ok.
uvis25: uvis25_set_interrupt_active_level/uvis25_get_interrupt_active_level test.
uvis25: set interrupt active level high.
uvis25: check interrupt active level ok.
uvis25: set interrupt active level lower.
uvis25: check interrupt active level ok.
uvis25: uvis25_set_interrupt_pin_type/uvis25_get_interrupt_pin_type test.
uvis25: set interrupt pin type push pull.
uvis25: check interrupt pin type ok.
uvis25: set interrupt pin type open drain.
uvis25: check interrupt pin type ok.
uvis25: uvis25_set_interrupt_type/uvis25_get_interrupt_type test.
uvis25: set interrupt type data ready.
uvis25: check interrupt type ok.
uvis25: set interrupt type uv index high.
uvis25: check interrupt type ok.
uvis25: set interrupt type uv index low.
uvis25: check interrupt type ok.
uvis25: set interrupt type uv index high or low.
uvis25: check interrupt type ok.
uvis25: uvis25_set_interrupt/uvis25_get_interrupt test.
uvis25: set interrupt true.
uvis25: check interrupt ok.
uvis25: set interrupt false.
uvis25: check interrupt ok.
uvis25: uvis25_set_latch_interrupt/uvis25_get_latch_interrupt test.
uvis25: set latch interrupt true.
uvis25: check latch interrupt ok.
uvis25: set latch interrupt false.
uvis25: check latch interrupt ok.
uvis25: uvis25_set_interrupt_low_threshold/uvis25_get_interrupt_low_threshold test.
uvis25: set interrupt low threshold true.
uvis25: check linterrupt low threshold ok.
uvis25: set interrupt low threshold false.
uvis25: check linterrupt low threshold ok.
uvis25: uvis25_set_interrupt_high_threshold/uvis25_get_interrupt_high_threshold test.
uvis25: set interrupt high threshold true.
uvis25: check linterrupt high threshold ok.
uvis25: set interrupt high threshold false.
uvis25: check interrupt high threshold ok.
uvis25: uvis25_set_threshold/uvis25_get_threshold test.
uvis25: set threshold 73.
uvis25: check threshold ok.
uvis25: uvis25_set_boot/uvis25_get_boot test.
uvis25: set boot mode normal.
uvis25: check boot mode ok.
uvis25: uvis25_threshold_convert_to_register/uvis25_threshold_convert_to_data test.
uvis25: threshold convert to register.
uvis25: uv is 7.6000.
uvis25: register is 0x79.
uvis25: threshold convert to data.
uvis25: register is 0x79.
uvis25: uv is 7.5625.
uvis25: finish register test.
```

```shell
uvis25 -t read --interface=iic --times=3

uvis25: chip is STMicroelectronics UVIS25.
uvis25: manufacturer is STMicroelectronics.
uvis25: interface is IIC SPI.
uvis25: driver version is 2.0.
uvis25: min supply voltage is 1.7V.
uvis25: max supply voltage is 3.6V.
uvis25: max current is 0.01mA.
uvis25: max temperature is 85.0C.
uvis25: min temperature is -20.0C.
uvis25: start read test.
uvis25: set continuous read.
uvis25: uv is 0.0000.
uvis25: uv is 0.0000.
uvis25: uv is 0.0000.
uvis25: set one shot read.
uvis25: uv is 0.0000.
uvis25: uv is 0.0000.
uvis25: uv is 0.0000.
uvis25: finish read test.
```

```shell
uvis25 -t int --interface=iic --times=3 --threshold=0.5

uvis25: chip is STMicroelectronics UVIS25.
uvis25: manufacturer is STMicroelectronics.
uvis25: interface is IIC SPI.
uvis25: driver version is 2.0.
uvis25: min supply voltage is 1.7V.
uvis25: max supply voltage is 3.6V.
uvis25: max current is 0.01mA.
uvis25: max temperature is 85.0C.
uvis25: min temperature is -20.0C.
uvis25: start interrupt test.
uvis25: low threshold interrupt.
uvis25: low threshold interrupt.
uvis25: uv 0.0000 is lower than threshold.
uvis25: low threshold interrupt.
uvis25: low threshold interrupt.
uvis25: uv 0.0000 is lower than threshold.
uvis25: low threshold interrupt.
uvis25: low threshold interrupt.
uvis25: uv 0.0000 is lower than threshold.
uvis25: finish interrupt test.
```

```shell
uvis25 -e read --interface=iic --times=3

uvis25: 1/3.
uvis25: uv is 0.0000.
uvis25: 2/3.
uvis25: uv is 0.0000.
uvis25: 3/3.
uvis25: uv is 0.0000.
```

```shell
uvis25 -e shot --interface=iic --times=3

uvis25: 1/3.
uvis25: uv is 0.0000.
uvis25: 2/3.
uvis25: uv is 0.0000.
uvis25: 3/3.
uvis25: uv is 0.0000.
```

```shell
uvis25 -e int --interface=iic --times=3 --threshold=0.1 --mode=HIGH

uvis25: 1/3.
uvis25: uv is 0.0000.
uvis25: active interrupt.
uvis25: high threshold interrupt.
uvis25: 2/3.
uvis25: uv is 0.1875.
uvis25: find interrupt.
```

```shell
uvis25 -h

Usage:
  uvis25 (-i | --information)
  uvis25 (-h | --help)
  uvis25 (-p | --port)
  uvis25 (-t reg | --test=reg) [--interface=<iic | spi>]
  uvis25 (-t read | --test=read) [--interface=<iic | spi>] [--times=<num>]
  uvis25 (-t int | --test=int) [--interface=<iic | spi>] [--times=<num>] [--threshold=<th>]
  uvis25 (-e read | --example=read) [--interface=<iic | spi>] [--times=<num>]
  uvis25 (-e shot | --example=shot) [--interface=<iic | spi>] [--times=<num>]
  uvis25 (-e int | --example=int) [--interface=<iic | spi>] [--times=<num>] [--threshold=<th>]
         [--mode=<READY | HIGH | LOW | HIGH-OR-LOW>]

Options:
  -e <read | shot | int>, --example=<read | shot | int>
                          Run the driver example.
  -h, --help              Show the help.
  -i, --information       Show the chip information.
      --interface=<iic | spi>
                          Set the chip interface.([default: iic])
      --mode=<READY | HIGH | LOW | HIGH-OR-LOW>
                          Set the interrupt mode.([default: HIGH])
  -p, --port              Display the pin connections of the current board.
  -t <reg | read | int>, --test=<reg | read | int>
                          Run the driver test.
      --threshold=<th>    Set the interrupt threshold.([default: 0.5])
      --times=<num>       Set the running times.([default: 3])
```

