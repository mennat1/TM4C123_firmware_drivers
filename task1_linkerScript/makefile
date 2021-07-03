CC=arm-none-eabi-gcc-9.3.1.exe
MACH=cortex-m4
ARCH=armv7e-m
CFLAGS= -mcpu=$(MACH) -march=$(ARCH) -mthumb -save-temps -nostdlib -Wall -O0 -Wl,-Map="app.map" -Wl,-T "tm4c123gh6pm.lds"

all: app_output.elf

	
app_output.elf: app_main.c app_2.c startupCode.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf *.s *.i *.o *.elf