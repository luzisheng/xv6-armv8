QEMU = qemu-system-aarch64

K = kernel

OBJS = \
	$K/entry.o \
	$K/start.o \
	$K/uart.o

TOOLPREFIX = aarch64-linux-gnu-

CC = $(TOOLPREFIX)gcc
LD = $(TOOLPREFIX)ld
AS = $(TOOLPREFIX)as
OBJCOPY = $(TOOLPREFIX)objcopy
OBJDUMP = $(TOOLPREFIX)objdump

CFLAGS = -Wall -g -O -fno-pie -fno-pic -fno-stack-protector -nostdlib -static -ffreestanding -nostartfiles
# header file search path
CFLAGS += -I. -Iinc
# determine the dependencies using gcc opts
CFLAGS += -MMD -MP
# machine-dependent opts
CFLAGS += -march=armv8-a -mtune=cortex-a57 -mcmodel=large -mgeneral-regs-only
LDFLAGS = -z max-page-size=4096
ASFLAGS = -march=armv8-a

$K/kernel: $(OBJS) $K/kernel.ld
	$(LD) $(LDFLAGS) -T $K/kernel.ld -o $@ $(OBJS) $(OBJS_KCSAN)
	$(OBJDUMP) -S $@ > $K/kernel.asm
	$(OBJDUMP) -x $@ > $K/kernel.sym

$K/%.o: $K/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$K/%.o: $K/%.S
	$(AS) $(ASFLAGS) -o $@ $<

# mkfs/mkfs: mkfs/mkfs.c $K/fs.h $K/param.h
#	gcc $(XCFLAGS) -Werror -Wall -I. -o mkfs/mkfs mkfs/mkfs.c

# Prevent deletion of intermediate files, e.g. cat.o, after first build, so
# that disk image changes after first build are persistent until clean.  More
# details:
# http://www.gnu.org/software/make/manual/html_node/Chained-Rules.html
.PRECIOUS: %.o

#fs.img: mkfs/mkfs README $(UEXTRA) $(UPROGS)
#	mkfs/mkfs fs.img README $(UEXTRA) $(UPROGS)

# without mkfs

# without qemu-gdb

# without -smp $(CPUS) 
QEMUOPTS = -machine virt -cpu cortex-a57 -kernel $< -m 128M -nographic


qemu: $K/kernel
	$(QEMU) $(QEMUOPTS)

-include kernel/*.d 

clean:
	rm -f */*.o */*.d $K/kernel

.PHONY: clean
