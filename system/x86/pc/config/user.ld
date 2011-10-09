ENTRY (crt_entry)

SECTIONS{
    . = 0x80000000;

    .text :{
        *(.text)
    }

    .rodata ALIGN (0x1000) : {
        *(.rodata)
    }

    .data ALIGN (0x1000) : {
        *(.data)
    }

    .bss : {
        *(COMMON)
        *(.bss)
    }
}

