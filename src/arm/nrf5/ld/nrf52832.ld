_estack = ORIGIN(RAM) + LENGTH(RAM);
_ebl = ORIGIN(BLFLASH) + LENGTH(BLFLASH);

SECTIONS {
    .mbrcfg : {
	KEEP(*(.mbr.mbrcfg))
    } >MBRCFG
    .boot : {
	. = ALIGN(4);
	KEEP(*(.boot.isr_vector))
	. = ALIGN(4);
	*(.boot*)
	*(.text*)
	*(.rodata*)
    } >BLFLASH
}
