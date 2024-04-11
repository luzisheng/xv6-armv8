// Physical memory layout

// qemu -machine virt is set up like this,
// based on qemu's hw/arm/virt.c:
//
// Space up to 0x8000000 is reserved for a boot ROM, provided by qemu
// 0x08000000 -- GIC
// 0x09000000 -- UART 
// 0x09030000 -- GPIO
// 0x0a000000 -- MMIO (difference?)
// 0x0c000000 -- virtio disk 
// 0x40000000 -- boot ROM jumps here, loads the kernel here
// unused RAM after 40000000.

// the kernel uses physical memory thus:
// 40000000 -- entry.S, then kernel text and data
// end -- start of kernel page allocation area
// PHYSTOP -- end RAM used by the kernel

// qemu puts UART registers here in physical memory.
#define UART0 0x09000000L
#define UART0_IRQ 1

// virtio mmio interface
#define VIRTIO0 0x0c000000L
#define VIRTIO0_IRQ 2

// the kernel expects there to be RAM
// for use by the kernel and user pages
// from physical address 0x80000000 to PHYSTOP.
#define KERNBASE 0x40000000L
#define PHYSTOP (KERNBASE + 128*1024*1024)
