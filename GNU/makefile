# C Compiler.
CC=arm-none-eabi-gcc
# GDB.
GDB=arm-none-eabi-gdb
# JLink GDB Server executable.
JLINK=JLinkGDBServerCL
# JLink GDB Server path. This is the default path on Windows so may need modifying.
JLINK_PATH=C:/"Program Files (x86)"/SEGGER/JLink_V620h/$(JLINK)
# File removal tool. Likely to be "del" for Windows, "rm" for Unix.
RM=del

# Assembler flags.
ASMFLAGS=-std=gnu99 -ggdb -Wall -ffunction-sections -fdata-sections -mcpu=cortex-m0plus -mthumb
# Compiler flags.
CFLAGS=-std=gnu99 -O0 -ggdb -Wall -ffunction-sections -fdata-sections -mcpu=cortex-m0plus -mthumb -DCPU_MKW41Z512VHT4
# Linker flags.
LDFLAGS=--specs=nosys.specs -Xlinker -Map="flash.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -mcpu=cortex-m0plus -mthumb -T "MCUXpresso/MKW41Z512xxx4_flash.ld"

# All .c source files.
SOURCES=flash.c MCUXpresso/system_MKW41Z4.c FreeRTOS/croutine.c FreeRTOS/event_groups.c FreeRTOS/list.c FreeRTOS/queue.c FreeRTOS/tasks.c FreeRTOS/timers.c FreeRTOS/portable/MemMang/heap_4.c FreeRTOS/portable/GCC/ARM_CM0/port.c
# All .o object files.
OBJECTS=$(SOURCES:%.c=%.o)
# All .d dependency files.
DEPS=$(OBJECTS:%.o=%.d)
# All include paths.
INCLUDES=-IMCUXpresso/ -IFreeRTOS/ -IFreeRTOS/include -IFreeRTOS/portable/GCC/ARM_CM0/

# Link all object files.
flash: $(OBJECTS) MCUXpresso/startup_mkw41z4.o
	$(CC) $(LDFLAGS) $^ -o flash.elf

# Include all .d files.
-include $(DEPS)

# Build target for every object file.
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -c -o $@ $<
MCUXpresso/startup_mkw41z4.o: MCUXpresso/startup_mkw41z4.S
	$(CC) $(ASMFLAGS) $^ -c -o $@	

# Clean. Delete all files of the specified types (including in subdirectories).
clean:
	-$(RM) /S *.o *.d *.map *.elf

# Open JLink GDB server and GDB for debugging and programming.
debug: flash
	start $(JLINK_PATH) -Device MKW41Z512xxx4 -If SWD
	$(GDB) flash.elf
	taskkill /IM $(JLINK).exe