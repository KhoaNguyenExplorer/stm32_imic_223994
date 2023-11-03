/*
 * firmware_update.c
 *
 *  Created on: Oct 1, 2023
 *      Author: Admin
 */

#include "Firmware_update.h"

extern char firmware_transmit_complete_flag;

__attribute__((section(".FunctionsOnRAM"))) void bootloader()
{
	custom_printf("Welcome to our application!\r\n");
	if (button_read())
	{
	    update_firmware();
	}
	else
	{
		jump_to_app();
	}
}

__attribute__((section(".FunctionsOnRAM"))) void update_firmware()
{
    custom_printf("Button pressed, we are now in booting mode!\r\n");
    custom_printf("Please input firmware size (8 digits - ex: 00008000): ");
    dma2_init(8);
    while(firmware_transmit_complete_flag != 1);
    firmware_transmit_complete_flag = 0;
    int fw_size = 0;
    sscanf(rx_buf, "%d", &fw_size);     // write fw_size to memory

    dma2_init(fw_size);
    custom_printf("\nPlease input %d bytes for new firmware: \r\n", fw_size);
    while(firmware_transmit_complete_flag != 1);
    custom_printf("Received %d bytes of new firmware\r\n", fw_size);

    uint32_t* SYST_CSR = (uint32_t*)(0xe000e010);
    *SYST_CSR = 0;      // disable interrupts during firmware update by disabling the system tick

    flash_erase_sector(0);
    flash_program((char*)0x08000000, rx_buf, sizeof(rx_buf));

    // soft reset
    uint32_t* SYS_AIRCR = (uint32_t*)(0xe000ed0c);
    *SYS_AIRCR = (0x5fa << 16) | (1 << 2);
}

__attribute__((section(".FunctionsOnRAM"))) void jump_to_app()
{
	custom_printf("No button pressed, jumping to app... \r\n");
	uint32_t* pResetHandler = (uint32_t*)(APP_START_ADDR + 4);
    void (*app)() = NULL;
    app = (void(*)())((*pResetHandler) | 1);
    app();
}
