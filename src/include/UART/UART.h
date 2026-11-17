#ifndef UART_H
#define UART_H

#include <stdint.h>

extern volatile char modtaget_tegn;
extern volatile uint8_t nyt_tegn_klar;

void start_uart(void);
void uart0_send_tegn(char tegn);
void uart0_send_tekst(const char* tekst);

#endif