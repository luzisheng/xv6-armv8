        # qemu -kernel loads the kernel at 0x40000000
        # and causes each hart (i.e. CPU) to jump there.
        # kernel.ld causes the following code to
        # be placed at 0x40000000.
.section .text
.global _entry
_entry:
        # set up a stack for C.
        # stack0 is declared in start.c,
        # with a 4096-byte stack per CPU.
        # sp = stack0 + (hartid * 4096)
        # at first, we consider one cpu
        ldr x0, =stack0
        add sp, x0, #4096
        # jump to start() in start.c
        # beware of branch out of range
        bl start
spin:
        b spin
