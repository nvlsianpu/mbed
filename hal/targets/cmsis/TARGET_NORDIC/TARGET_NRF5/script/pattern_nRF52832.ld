/* mega Linker script to configure memory regions. */


MEMORY
{{
  FLASH (rx) : ORIGIN = 0x1C000, LENGTH = 0x64000
  RAM (rwx) :  ORIGIN = {0:#x}, LENGTH = {1:#x}
}}

INCLUDE \"NRF52832.ld\"\n"
