CC=arm-none-eabi-gcc
COMPILER_FLAG=-mcpu=cortex-m4 -std=gnu11 -specs=nosys.specs -specs=nano.specs -u _printf_float -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
 
LINKER_FLAG=-T"STM32F411VETX_FLASH.ld" -Wl,-Map="./Build/map/mapfile.map" -Wl,--gc-sections -static -Wl,--start-group -lc -lm -Wl,--end-group

CORE_DIR=-I"./Core/inc"
OS_DIR=./FreeRTOS-LTS/FreeRTOS/FreeRTOS-Kernel
PORT_DIR=${OS_DIR}/portable/GCC/ARM_CM4F
MEM_MANG_DIR=${OS_DIR}/portable/MemMang
HEADER_DIR=-I"./My_Library/My_HAL/inc" -I"./My_Library/My_Apps/inc" -I${OS_DIR}/include -I${PORT_DIR}

All:
	${CC} -c ${COMPILER_FLAG} ${HEADER_DIR} ${OS_DIR}/croutine.c -o Build/obj/croutine.o
	${CC} -c ${COMPILER_FLAG} ${HEADER_DIR} ${OS_DIR}/event_groups.c -o Build/obj/event_groups.o	
	${CC} -c ${COMPILER_FLAG} ${HEADER_DIR} ${OS_DIR}/list.c -o Build/obj/list.o
	${CC} -c ${COMPILER_FLAG} ${HEADER_DIR} ${OS_DIR}/queue.c -o Build/obj/queue.o
	${CC} -c ${COMPILER_FLAG} ${HEADER_DIR} ${OS_DIR}/stream_buffer.c -o Build/obj/stream_buffer.o			
	${CC} -c ${COMPILER_FLAG} ${HEADER_DIR} ${OS_DIR}/tasks.c -o Build/obj/tasks.o
	${CC} -c ${COMPILER_FLAG} ${HEADER_DIR} ${OS_DIR}/timers.c -o Build/obj/timers.o
	${CC} -c ${COMPILER_FLAG} ${HEADER_DIR} ${PORT_DIR}/port.c -o Build/obj/port.o	
	${CC} -c ${COMPILER_FLAG} ${HEADER_DIR} ${MEM_MANG_DIR}/heap_4.c -o Build/obj/heap_4.o		

	${CC} -c ${COMPILER_FLAG} ${CORE_DIR} ${HEADER_DIR} ./Core/src/main.c -o Build/obj/main.o
	${CC} -c ${COMPILER_FLAG} ${CORE_DIR} ${HEADER_DIR} ./My_Library/My_HAL/src/uart.c -o Build/obj/uart.o	
	${CC} -c ${COMPILER_FLAG} ${CORE_DIR} ${HEADER_DIR} ./My_Library/My_HAL/src/Spi.c -o Build/obj/Spi.o	
	${CC} -c ${COMPILER_FLAG} ${CORE_DIR} ${HEADER_DIR} ./My_Library/My_HAL/src/clock.c -o Build/obj/clock.o	
	${CC} -c ${COMPILER_FLAG} ${CORE_DIR} ${HEADER_DIR} ./My_Library/My_HAL/src/AD_converter.c -o Build/obj/AD_converter.o	
	${CC} -c ${COMPILER_FLAG} ${CORE_DIR} ${HEADER_DIR} ./My_Library/My_HAL/src/Peripheral_LED.c -o Build/obj/Peripheral_LED.o
	${CC} -c ${COMPILER_FLAG} ${CORE_DIR} ${HEADER_DIR} ./My_Library/My_HAL/src/Timer.c -o Build/obj/Timer.o
	${CC} -c ${COMPILER_FLAG} ${CORE_DIR} ${HEADER_DIR} ./My_Library/My_Apps/src/MFRC522.c -o Build/obj/MFRC522.o	
	
	${CC} -c -x assembler-with-cpp startup_stm32f411vetx.s -o Build/obj/startup_stm32f411vetx.o
	
	arm-none-eabi-gcc Build/obj/*.o ${COMPILER_FLAG} ${LINKER_FLAG} -o ./Build/exe/stm32_fw.elf

	arm-none-eabi-size $ Build/exe/stm32_fw.elf
	arm-none-eabi-objcopy -O binary ./Build/exe/stm32_fw.elf ./Build/exe/stm32_fw.bin
	arm-none-eabi-objcopy -O ihex ./Build/exe/stm32_fw.elf ./Build/exe/stm32_fw.hex

Clear:
	rm ./Build/map/*	
	rm ./Build/exe/*	
	rm ./Build/obj/*

Flash:
	ST-LINK_CLI -c SWD -SE 0 2 -P "Build/exe/stm32_fw.bin" 0x08000000 -V -Run