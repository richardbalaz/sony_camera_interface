#include <avr/io.h>

#define F_CPU 9600000L
#include <util/delay.h>

#define CLK_DELAY 10

/* Used to identify rising-edge of pin A */
int encoder_previous_state = 0;

enum commands {
    EXPOSURE_INC    = 0x00,
    EXPOSURE_DEC    = 0x01,

    BUTTON_EXP      = 0x02,
    BUTTON_OK       = 0x03,
    BUTTON_SW       = 0x04
};

/* Transmits 4 LSB bits to transmit line */
void shift_data_out(uint8_t data)
{
    PORTB &= ~(1 << PB2);
    _delay_us(CLK_DELAY);
    int i;
    for (i = 0x01; i <= 0x08; i <<= 1) {
        if (data & i)
            PORTB |= (1 << PB2);
        else
            PORTB &= ~(1 << PB2);
        _delay_us(CLK_DELAY);
    }
    PORTB |= (1 << PB2);
}

/* Checks if there is a rotation made by human */
void read_exposure_encoder(void)
{
    int pin_a = PINB & (1 << PB0);
    if (pin_a == 0 && encoder_previous_state != 0) {
        if ((PINB & (1 << PB1)) == 0)
            shift_data_out(EXPOSURE_DEC);
        else
            shift_data_out(EXPOSURE_INC);
    }
    encoder_previous_state = pin_a;
}

int main(void)
{
    /* Setup input ports for exposure rotary encoder */
    PORTB |= (1 << PB0) | (1 << PB1);
    DDRB &= ~((1 << PB0) | (1 << PB1));

    /* Setup output port for transmit line */
    PORTB |= (1 << PB2);
    DDRB |= (1 << PB2);

    while (1) {
        read_exposure_encoder();
    }
}
