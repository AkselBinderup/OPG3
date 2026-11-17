#include "../../projectDefines.h"
#include "../../include/UART/UART.h"
#include "../../include/states/statemachine.h"

static Tilstand_t nuvaerende_tilstand = Modtag_Adresse;

static char adresse_buffer[10];
static uint8_t buffer_indeks = 0;

static uint16_t valgt_adresse = 0;
static uint8_t bitPosition = 0;
static uint8_t bitValue = 0;

static void skift_tilstand(Tilstand_t ny_tilstand) {
    nuvaerende_tilstand = ny_tilstand;
    buffer_indeks = 0;

    if (ny_tilstand == Modtag_Adresse) {
        uart0_send_tekst("\r\n[TILSTAND: Modtag_Adresse] Indtast hex-adresse afsluttet med ':' (f.eks. 0025:):\r\n");
    } 
    else if (ny_tilstand == Modtag_Bit_Position_I_Adresse) {
        uart0_send_tekst("\r\n[TILSTAND: Modtag_Bit_Position_I_Adresse] Indtast bit-position (0-7):\r\n");
    } 
    else if (ny_tilstand == Modtag_Bit_Positon_Vaerdi) {
        uart0_send_tekst("\r\n[TILSTAND: Modtag_Bit_Positon_Vaerdi] Indtast bit-vaerdi (0 eller 1):\r\n");
    }
}

static void udskriv_resultat(uint16_t addr, uint8_t pos, uint8_t val, uint8_t reg_indhold) {
    char linje[60];
    char bit_tekst[10]; 

    int tekst_pos = 0;
    for (int bit = 7; bit >= 0; bit--) {
        if (reg_indhold & (1 << bit)) {
            bit_tekst[tekst_pos++] = '1';
        } else {
            bit_tekst[tekst_pos++] = '0';
        }
        if (bit == 4) {
            bit_tekst[tekst_pos++] = ' '; // Mellemrum mellem de 4 øverste og 4 nederste bits
        }
    }
    bit_tekst[tekst_pos] = '\0';

    uart0_send_tekst("\r\n--- RESULTAT ---\r\n");
    sprintf(linje, "Linje 1: Modtagen adresse: 0x%04X\r\n", addr);
    uart0_send_tekst(linje);

    sprintf(linje, "Linje 2: Modtagen bit position: %d\r\n", pos);
    uart0_send_tekst(linje);

    sprintf(linje, "Linje 3: Modtagen bit vaerdi: %d\r\n", val);
    uart0_send_tekst(linje);

    sprintf(linje, "Linje 4: Bit moenster: %s\r\n", bit_tekst);
    uart0_send_tekst(linje);
}

void init_state_machine(void) {
    skift_tilstand(Modtag_Adresse);
}

void haandter_tegn(char c) {
    if (c == '\r' || c == '\n') {
        return;
    }

    uart0_send_tegn(c);

    switch (nuvaerende_tilstand) {
        
        case Modtag_Adresse:
            if (c == ':') {
                // Afslut tekststrengen og konverter hex-tekst til et tal
                adresse_buffer[buffer_indeks] = '\0';
                valgt_adresse = (uint16_t)strtol(adresse_buffer, NULL, 16);
                skift_tilstand(Modtag_Bit_Position_I_Adresse);
            } 
            else if (buffer_indeks < 8) {
                // Gem tegnet i bufferen (f.eks. '0', 'x', '0', '0', '2', '5')
                adresse_buffer[buffer_indeks++] = c;
            }
            break;

        case Modtag_Bit_Position_I_Adresse:
            if (c >= '0' && c <= '7') {
                bitPosition = c - '0'; // Konverter ASCII-tegn '0'-'7' til tallet 0-7
                skift_tilstand(Modtag_Bit_Positon_Vaerdi);
            } else {
                uart0_send_tekst("\r\nUgyldig bit-position! Starter forfra.\r\n");
                skift_tilstand(Modtag_Adresse);
            }
            break;

        case Modtag_Bit_Positon_Vaerdi:
            if (c == '0' || c == '1') {
                bitValue = c - '0'; 

                volatile uint8_t *p_8 = (volatile uint8_t *)(uintptr_t)valgt_adresse;

                if (bitValue == 1) {
                    *p_8 |= (1 << bitPosition);
                } else {
                    *p_8 &= ~(1 << bitPosition);
                }

                udskriv_resultat(valgt_adresse, bitPosition, bitValue, *p_8);

                skift_tilstand(Modtag_Adresse);
            } else {
                uart0_send_tekst("\r\nUgyldig bit-vaerdi! Starter forfra.\r\n");
                skift_tilstand(Modtag_Adresse);
            }
            break;
    }
}