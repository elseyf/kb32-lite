PROJECT		= kb32

CFILES		= $(wildcard src/*.c)
SFILES		= $(wildcard src/*.s)

INCLUDES	= -I./src
LDLIBS		= 

LDSCRIPT		= stm32f103x8.ld
OPENCM3_LIB		= opencm3_stm32f1
OPENCM3_DEFS	= -DSTM32F1
OPT				= -Os
ARCH_FLAGS		= \
	-Wall -mthumb -mcpu=cortex-m3 -msoft-float -mfix-cortex-m3-ldrd  -DGCC_HASCLASSVISIBILITY -D_GLIBCXX_USE_TMPNAM=0 \
	-fno-exceptions -nodefaultlibs -nostdlib -fno-builtin
OOCD_INTERFACE	= stlink-v2
OOCD_TARGET		= stm32f1x

include ./rules.mk
