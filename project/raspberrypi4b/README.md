### 1. chip

#### 1.1 chip info

chip name : Raspberry Pi 4B

iic pin: SCL/SDA GPIO3/GPIO2

spi pin: SCLK/MOSI/MISO/CS GPIO11/GPIO10/GPIO9/GPIO8

gpio pin: INT GPIO17

### 2. install

#### 2.1 install info

```shell
sudo apt-get install libgpiod-dev

make
```

### 3. uvis25

#### 3.1 command Instruction

​          uvis25 is a basic command which can test all uvis25 driver function:

​           -i        show uvis25 chip and driver information.

​           -h       show uvis25 help.

​           -p       show uvis25 pin connections of the current board.

​           -t (reg (-iic | -spi) | read <times> (-iic | -spi) | int <times> (-iic | -spi)  -th <threshold>) 

​           -t reg (-iic | -spi)        run uvis25 register test.

​           -t read <times> (-iic | -spi)        run uvis25 read test. times means test times.

​           -t int <times> (-iic | -spi)  -th <threshold>        run uvis25 interrupt test. times means test times. threshold means the interrupt threshold.

​           -c (read <times> (-iic | -spi) | shot <times> (-iic | -spi) | int <times>  (-iic | -spi) -m <mode> -th <threshold>)

​           -c read <times> (-iic | -spi)         run uvis25 read function. times means test times.

​           -c shot <times> (-iic | -spi)        run uvis25 shot function. times means test times.

​           -c int <times>  (-iic | -spi) -m <mode> -th <threshold>       run uvis25 interrupt function. times means test times. mode means interrupt mode and it can be "READY","HIGH","LOW" or "HIGH|LOW". threshold means the interrupt threshold.

#### 3.2 command example

```shell
./uvis25 -i

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
./uvis25 -p

uvis25: SPI interface SCK connected to GPIO11(BCM).
uvis25: SPI interface MISO connected to GPIO9(BCM).
uvis25: SPI interface MOSI connected to GPIO10(BCM).
uvis25: SPI interface CS connected to GPIO8(BCM).
uvis25: IIC interface SCL connected to GPIO3(BCM).
uvis25: IIC interface SDA connected to GPIO2(BCM).
uvis25: INT connected to GPIO17(BCM).
```

```shell
./uvis25 -t reg -spi

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
uvis25: uvis25_set_iic/uvis25_get_iic test.
uvis25: set iic true.
uvis25: check iic ok.
uvis25: set iic false.
uvis25: check iic ok.
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
uvis25: set threshold 176.
uvis25: check threshold ok.
uvis25: uvis25_set_boot/uvis25_get_boot test.
uvis25: set boot mode normal.
uvis25: check boot mode ok.
uvis25: uvis25_threshold_convert_to_register/uvis25_threshold_convert_to_data test.
uvis25: threshold convert to register.
uvis25: uv is 12.6700.
uvis25: register is 0xCA.
uvis25: threshold convert to data.
uvis25: register is 0xCA.
uvis25: uv is 12.6250.
uvis25: finish register test.
```

```shell
./uvis25 -t read 3 -spi

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
./uvis25 -t int 3 -spi -th 0.5

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
./uvis25 -c read 3 -spi

uvis25: 1/3.
uvis25: uv is 0.0000.
uvis25: 2/3.
uvis25: uv is 0.0000.
uvis25: 3/3.
uvis25: uv is 0.0000.
```

```shell
./uvis25 -c shot 3 -spi

uvis25: 1/3.
uvis25: uv is 0.0000.
uvis25: 2/3.
uvis25: uv is 0.0000.
uvis25: 3/3.
uvis25: uv is 0.0000.
```

```shell
./uvis25 -c int 3 -spi -m HIGH -th 0.5

uvis25: 1/3.
uvis25: uv is 0.8750.
uvis25: find interrupt.
```

```shell
./uvis25 -h

uvis25 -i
	show uvis25 chip and driver information.
uvis25 -h
	show uvis25 help.
uvis25 -p
	show uvis25 pin connections of the current board.
uvis25 -t reg (-iic | -spi)
	run uvis25 register test.
uvis25 -t read <times> (-iic | -spi)
	run uvis25 read test.times means test times.
uvis25 -t int <times> (-iic | -spi) -th <threshold>
	run uvis25 interrupt test.times means test times.threshold means the interrupt threshold.
uvis25 -c read <times> (-iic | -spi)
	run uvis25 read function.times means test times.
uvis25 -c shot <times> (-iic | -spi)
	run uvis25 shot function.times means test times.
uvis25 -c int <times> (-iic | -spi) -m <mode> -th <threshold>
	run uvis25 interrupt function.times means test times.mode means interrupt mode and it can be "READY","HIGH","LOW" or "HIGH|LOW".threshold means the interrupt threshold.
```

