SCRIPT_NAME=elf
OUTPUT_FORMAT="elf32-m68k"
TEXT_START_ADDR=0x80000000
MAXPAGESIZE=0x2000
NONPAGED_TEXT_START_ADDR=${TEXT_START_ADDR}
ARCH=m68k
MACHINE=
NOP=0x4e754e75
TEMPLATE_NAME=elf32
EXTRA_EM_FILE=m68kelf
GENERATE_SHLIB_SCRIPT=yes
ELFSIZE=32
NO_SMALL_DATA=yes
