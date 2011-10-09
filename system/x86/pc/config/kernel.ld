ENTRY (multiboot_entry)

SECTIONS{
    . = 0x01000000;

    .text : {
	_kernel_start = .;
	*(.boot)
        *(.text)
    }

    .rodata ALIGN (0x1000) : {
        *(.rodata)
    }

    .data ALIGN (0x1000) : {
        *(.data)
        . = ALIGN(0x1000);
        _boot_img_start = .;
        *(.binary)
        _boot_img_end = .;
    }

    .bss : {
        *(COMMON)
        *(.bss)
	_kernel_end = .;
    }
}
