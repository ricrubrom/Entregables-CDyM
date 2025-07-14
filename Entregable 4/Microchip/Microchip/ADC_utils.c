#include "ADC_utils.h"

void ADC_init()
{
    ADMUX = (1 << REFS0) | (1 << ADLAR) | (1 << MUX1) | (1 << MUX0); // canal 3, AVcc ref, justificado a la izquierda
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // prescaler 128
    DDRC &= ~(1 << PORTC3);
    DIDR0 |= (1 << ADC3D); // Deshabilita entrada digital de ADC3
}

uint8_t ADC_get_value()
{
    ADCSRA |= (1 << ADSC);              // Start conversion
    while (!(ADCSRA & (1 << ADIF)));    // Espera que termine
    ADCSRA |= (1 << ADIF);              // Limpia el flag
    return ADCH;                        // Justificado a izquierda, leemos solo ADCH (8 bits)
}

