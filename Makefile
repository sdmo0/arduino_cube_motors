ARDUINO_DIR=/usr/share/arduino
CORES=$(ARDUINO_DIR)/hardware/arduino/cores/arduino
STDVARIANT=$(ARDUINO_DIR)/hardware/arduino/variants/standard
AVRD_CONF=$(ARDUINO_DIR)/hardware/tools/avrdude.conf

MMU=atmega328p
CPUCLOCK=16000000L
ARDUINOVER=101
#SERIALPORT=/dev/ttyACM0
SERIALPORT_1=/dev/ttyUSB0
SERIALPORT_2=/dev/ttyUSB1
BAUDRATE=115200

CODENAME_1=arduino_1
CODENAME_2=arduino_2

GC_HWOPTS=-mmcu=$(MMU) -DF_CPU=$(CPUCLOCK) -DARDUINO=$(ARDUINOVER)
GC_STDINC=-I$(CORES) -I$(STDVARIANT)
GPP_OPTS=-c -g -Os -Wall -fno-exceptions -ffunction-sections -fdata-sections $(GC_HWOPTS) $(GC_STDINC)
GCC_OPTS=-c -g -Os -Wall -ffunction-sections -fdata-sections $(GC_HWOPTS) $(GC_STDINC)
GCC_LINKOPTS=-mmcu=$(MMU) -Wl,--gc-sections -L. -lm -lcore
OBJCPY_EEPOPTS=-O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0
OBJCPY_HEXOPTS=-O ihex -j .text -j .data -R .eeprom
AVRD_OPTS_1=-C$(AVRD_CONF) -p$(MMU) -carduino -P$(SERIALPORT_1) -b$(BAUDRATE) -D -Uflash:w:./$(CODENAME_1).hex:i
AVRD_OPTS_2=-C$(AVRD_CONF) -p$(MMU) -carduino -P$(SERIALPORT_2) -b$(BAUDRATE) -D -Uflash:w:./$(CODENAME_2).hex:i


build:
	avr-g++ $(GPP_OPTS) -o$(CODENAME_1).o $(CODENAME_1).cpp
	avr-g++ $(GPP_OPTS) -o$(CODENAME_2).o $(CODENAME_2).cpp
	avr-gcc $(GCC_OPTS) -owiring_analog.o $(CORES)/wiring_analog.c
	avr-gcc $(GCC_OPTS) -owiring_digital.o $(CORES)/wiring_digital.c
	avr-gcc $(GCC_OPTS) -owiring_pulse.o $(CORES)/wiring_pulse.c
	avr-gcc $(GCC_OPTS) -owiring_shift.o $(CORES)/wiring_shift.c
	avr-gcc $(GCC_OPTS) -owiring.o $(CORES)/wiring.c
	avr-gcc $(GCC_OPTS) -oWInterrupts.o $(CORES)/WInterrupts.c
	avr-g++ $(GPP_OPTS) -oCDC.o $(CORES)/CDC.cpp
	avr-g++ $(GPP_OPTS) -oHardwareSerial.o $(CORES)/HardwareSerial.cpp
	avr-g++ $(GPP_OPTS) -oHID.o $(CORES)/HID.cpp
	avr-g++ $(GPP_OPTS) -oIPAddress.o $(CORES)/IPAddress.cpp
#	avr-g++ $(GPP_OPTS) -omain.o $(CORES)/main.cpp
	avr-g++ $(GPP_OPTS) -onew.o $(CORES)/new.cpp
	avr-g++ $(GPP_OPTS) -oPrint.o $(CORES)/Print.cpp
	avr-g++ $(GPP_OPTS) -oStream.o $(CORES)/Stream.cpp
	avr-g++ $(GPP_OPTS) -oTone.o $(CORES)/Tone.cpp
	avr-g++ $(GPP_OPTS) -oUSBCore.o $(CORES)/USBCore.cpp
	avr-g++ $(GPP_OPTS) -oWString.o $(CORES)/WString.cpp
	avr-g++ $(GPP_OPTS) -oWMath.o $(CORES)/WMath.cpp
	avr-ar rcs ./libcore.a ./wiring_analog.o
	avr-ar rcs ./libcore.a ./wiring_digital.o
	avr-ar rcs ./libcore.a ./wiring_pulse.o
	avr-ar rcs ./libcore.a ./wiring_shift.o
	avr-ar rcs ./libcore.a ./wiring.o
	avr-ar rcs ./libcore.a ./WInterrupts.o
	avr-ar rcs ./libcore.a ./CDC.o
	avr-ar rcs ./libcore.a ./HardwareSerial.o
	avr-ar rcs ./libcore.a ./HID.o
	avr-ar rcs ./libcore.a ./IPAddress.o
#	avr-ar rcs ./libcore.a ./main.o
	avr-ar rcs ./libcore.a ./new.o
	avr-ar rcs ./libcore.a ./Print.o
	avr-ar rcs ./libcore.a ./Stream.o
	avr-ar rcs ./libcore.a ./Tone.o
	avr-ar rcs ./libcore.a ./USBCore.o
	avr-ar rcs ./libcore.a ./WString.o
	avr-ar rcs ./libcore.a ./WMath.o

	avr-gcc -o$(CODENAME_1).elf $(CODENAME_1).o $(GCC_LINKOPTS)
	avr-objcopy $(OBJCPY_EEPOPTS) $(CODENAME_1).elf $(CODENAME_1).eep
	avr-objcopy $(OBJCPY_HEXOPTS) $(CODENAME_1).elf $(CODENAME_1).hex

	avr-gcc -o$(CODENAME_2).elf $(CODENAME_2).o $(GCC_LINKOPTS)
	avr-objcopy $(OBJCPY_EEPOPTS) $(CODENAME_2).elf $(CODENAME_2).eep
	avr-objcopy $(OBJCPY_HEXOPTS) $(CODENAME_2).elf $(CODENAME_2).hex

flash_1:
	avrdude $(AVRD_OPTS_1)

flash_2:
	avrdude $(AVRD_OPTS_2)

clean:
	-rm -f *.o *.elf *.hex *.eep libcore.a *~
