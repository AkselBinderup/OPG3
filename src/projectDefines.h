#ifndef PROJECT_DEFINES_H
#define PROJECT_DEFINES_H

#include <xc.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h> 
#include <stdint.h>  

#define BAUD_9600 103 // 9600 baud ved 16 MHz

typedef enum {
    Modtag_Adresse,
    Modtag_Bit_Position_I_Adresse,
    Modtag_Bit_Positon_Vaerdi
} Tilstand_t;

#endif