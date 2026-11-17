#include "src/projectDefines.h"
#include "src/include/UART/UART.h"
#include "src/include/States/stateMachine.h"

int main(void) {
    start_uart();
    sei(); 

    init_state_machine();  

    while (1) {
        if (nyt_tegn_klar == 1) {
            nyt_tegn_klar = 0; 
            haandter_tegn(modtaget_tegn);
        }
    }
}