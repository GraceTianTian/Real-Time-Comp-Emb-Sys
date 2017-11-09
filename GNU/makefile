# C Compiler.
CC=arm-none-eabi-gcc
# Assembler flags.
ASMFLAGS=-ggdb -mcpu=cortex-m0plus -Wall -mfloat-abi=soft -mthumb -fno-common -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -mapcs -std=gnu99
# Compiler flags.
CFLAGS=-std=gnu99 -O0 -fno-common -ggdb -Wall -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0plus -mthumb -DCPU_MKW41Z512VHT4
# Linker flags/
LDFLAGS=-Xlinker -Map="flash.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -mcpu=cortex-m0plus -mthumb -T "MKW41Z512xxx4_flash.ld"

# All .c source files.
SOURCES=flash.c Keil/LED_FRDM-KW41Z.c MCUXpresso/system_MKW41Z4.c FreeRTOS/croutine.c FreeRTOS/event_groups.c FreeRTOS/list.c FreeRTOS/queue.c FreeRTOS/tasks.c FreeRTOS/timers.c FreeRTOS/portable/MemMang/heap_4.c FreeRTOS/portable/GCC/ARM_CM0/port.c
# All .o object files.
OBJECTS=$(SOURCES:%.c=%.o)
# All .d dependency files.
DEPS=$(OBJECTS:%.o=%.d)
# All include paths.
INCLUDES=-IKeil/ -IMCUXpresso/ -IFreeRTOS/ -IFreeRTOS/include -IFreeRTOS/portable/GCC/ARM_CM0/

# Link all object files.
flash: $(OBJECTS) startup_mkw41z4.o
	$(CC) --specs=nosys.specs $(LDFLAGS) $^ -o flash.elf

# Include all .d files.
-include $(DEPS)

# Build target for every object file.
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -MP -MF $(@:%.o=%.d) -MT $@ -MT $(@:%.o=%.d) -c -o $@ $<
startup_mkw41z4.o: startup_mkw41z4.S
	$(CC) $(ASMFLAGS) $^ -c -o $@