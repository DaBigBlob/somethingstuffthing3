
CC = zig cc
BFLAGS = -target x86_64-windows-gnu -lkernel32
CFLAGS = -static -nostdinc -nostdlib -O3 -Wall -Wextra -Wfatal-errors -ffreestanding -fomit-frame-pointer -fno-plt -nodefaultlibs -fno-asynchronous-unwind-tables -fno-unwind-tables -fdata-sections -fno-stack-protector -fno-builtin -std=c11 -pedantic -Wno-strict-prototypes -Wl,-e,__sys_entry -g0 -fno-exceptions -fno-rtti -fno-ident -Wa,--noexecstack -Wa,--strip-local-absolute -fno-ident
AFLAGS = -nostdinc -ffreestanding -nodefaultlibs -fno-builtin -Wl,-e,WinMain,--subsystem,windows

ENTRY = src/main.c
BIN = bin/
ASM = asm/
NAME = intrusive

_PRINTER = printf
_MKDIR = mkdir -p
_RM = rm -rf
_NEWLINE = \n
_SED = sed

bdir:
	@${_MKDIR} ${BIN} ${ASM}

cmpl: ${ENTRY} bdir
	${CC} ${ENTRY} ${BFLAGS} ${CFLAGS} -S -o ${ASM}${NAME}.S
	@${_PRINTER} "compiled to: ${ASM}${NAME}.S${_NEWLINE}"

ascl: ${ASM}${NAME}.S
	${_SED} -E -i "" 's/^.*\.(file|def|scl|type|endef).*$$//' ${ASM}${NAME}.S
	${_SED} -E -i "" 's/^.*#+.*\%bb\..*$$//' ${ASM}${NAME}.S
	${_SED} -E -i "" 's/^.*@feat\..*$$//' ${ASM}${NAME}.S

assm: ${ASM}${NAME}.S
	${CC} ${ASM}${NAME}.S ${BFLAGS} ${AFLAGS} -o ${BIN}${NAME}.exe
	@${_PRINTER} "assembled to: ${BIN}${NAME}.exe${_NEWLINE}"

all: cmpl ascl assm

clen: ${BIN} ${ASM}
	${_RM} ${BIN} ${ASM}