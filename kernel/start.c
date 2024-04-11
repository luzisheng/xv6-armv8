#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "defs.h"

// entry.S needs one stack per CPU.
__attribute__ ((aligned (16))) char stack0[4096 * NCPU];

// entry.S jumps here in machine mode on stack0.
void start() {
	uartinit();
	uartputc("Hello world\n");
}

