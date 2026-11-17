#include "../projectDefines.h"
#include "../include/UART/UART.h"

volatile char modtaget_tegn = 0;
volatile uint8_t nyt_tegn_klar = 0;

void start_uart(void) {
    UBRR0 = BAUD_9600;
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0); 
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);                
}

void uart0_send_tegn(char tegn) {
    while (!(UCSR0A & (1 << UDRE0))) {
    }
    UDR0 = tegn;
}

void uart0_send_tekst(const char* tekst) {
    int i = 0;
    while (tekst[i] != '\0') {
        uart0_send_tegn(tekst[i]);
        i++;
    }
}

ISR(USART0_RX_vect) {
    modtaget_tegn = UDR0;
    nyt_tegn_klar = 1;
}