//
// the virt board supports one PL011 UART
//
#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "defs.h"

// the following PL011 UART driver is from this ebook: 
// https://github.com/umanovskis/baremetal-arm
// the style is identical to xv6

// the UART control registers are memory-mapped
// at address UART0. this macro returns the
// address of one of the registers.
#define Reg(reg) ((volatile unsigned char *)(UART0 + reg))

#define UART_DR		    0	        // data register
#define UART_RSR	    1	        // receive status register/error clear register
#define UART_FR		    6	        // flag register
#define UART_FR_TXFF	(1 << 5)	// tramit FIFO full
#define UART_FR_RXFE	(1 << 4)	// receive FIFO empty
#define	UART_IBRD	    9	        // integer baudrate register
#define UART_FBRD	    10	      // fraction baudrate register
#define UART_LCRH	    11	      // line control register high
#define UART_LCRH_FEN	(1 << 4)	// enable FIFO
#define UART_CR		    12	      // control register
#define	UART_CR_RXE	  (1 << 9)	// enable receive
#define UART_CR_TXE	  (1 << 8)	// enable transmit
#define	UART_CR_EN	  (1 << 0)	// enable UART
#define UART_IMSC	    14	      // interrupt mask set/clear register
#define UART_IMSC_RXI	(1 << 4)	// receive interrupt
#define UART_IMSC_TXI	(1 << 5)	// transmit interrupt
#define UART_MIS	    16	      // masked interrupt status register
#define	UART_ICR	    17	      // interrupt clear register

#define UART_BITRATE  19200
#define UART_CLK      24000000 

#define ReadReg(reg) (*(Reg(reg)))
#define WriteReg(reg, v) (*(Reg(reg)) = (v))

void uartinit(void) {
	// set baudrate
	unsigned int left = UART_CLK % (16 * UART_BITRATE);
	WriteReg(UART_IBRD, UART_CLK / (16 * UART_BITRATE));
	WriteReg(UART_FBRD, (left * 4 + UART_BITRATE / 2) / UART_BITRATE);

  // enable trasmit and receive
	unsigned char flagCR = ReadReg(UART_CR);
	WriteReg(UART_CR, flagCR | UART_CR_EN | UART_CR_RXE | UART_CR_TXE);

  // enable FIFO
	unsigned char flagLCRH = ReadReg(UART_LCRH);
	WriteReg(UART_LCRH, flagLCRH | UART_LCRH_FEN);
}

void uartputc(const char* s) {
	while (*s != '\0') {
		WriteReg(UART_DR, *s);
		s++;
	}
}
