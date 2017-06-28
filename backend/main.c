#include <avr/io.h>

#define F_CPU 9600000L
#include <util/delay.h>

#define CLK_DELAY 100
#define BUTTON_PRESS_LENGHT 120

enum commands {
    EXPOSURE_INC,
    EXPOSURE_DEC,

    BUTTON_EXP,
    BUTTON_OK,
    BUTTON_SW
};

void exposure_inc() {
    PORTB |= (1 << PB1);
    _delay_ms(BUTTON_PRESS_LENGHT);
    PORTB &= ~(1 << PB1);
}

void exposure_dec() {
    PORTB |= (1 << PB2);
    _delay_ms(BUTTON_HOLD_LENGHT);
    PORTB &= ~(1 << PB2);
}

void button_exp() {
    PORTB |= (1 << PB3);
    _delay_ms(BUTTON_PRESS_LENGHT);
    PORTB &= ~(1 << PB3);
}

void button_ok() {
    PORTB |= (1 << PB4);
    _delay_ms(BUTTON_PRESS_LENGHT);
    PORTB &= ~(1 << PB4);
}

void button_sw() {
    PORTB |= (1 << PB5);
    _delay_ms(BUTTON_PRESS_LENGHT);
    PORTB &= ~(1 << PB5);
}

/* This function handles receiving the command from frontend */
void receive_command()
{
    /* Wait for start bit */
    while(PINB & (1 << PB0))
        continue;

    /* Shift command in */
    uint8_t cmd = 0;
    _delay_us(CLK_DELAY + (CLK_DELAY / 2.0));

    int i;
    for (i = 0; i < 4; i++) {
        cmd |= (PINB & (1 << PB0)) ? (1 << i) : 0;
        _delay_us(CLK_DELAY);
    }

    /* Call required command function */
    switch (cmd) {
    case EXPOSURE_INC:
        exposure_inc();
        break;
    case EXPOSURE_DEC:
        exposure_dec();
        break;
    case BUTTON_EXP:
        button_exp();
        break;
    case BUTTON_OK:
        button_ok();
        break;
    case BUTTON_SW:
        button_sw();
        break;
    }
}

int main(void)
{
    /* Setup input port for receive line */
    PORTB |= (1 << PB0);
    DDRB &= ~(1 << PB0);

    /* Setup output ports for buttons */
    PORTB &= ~(1 << PB3) | (1 << PB4) | (1 << PB5);
    DDRB |= (1 << PB3) | (1 << PB4) | (1 << PB5);

    /* Setup output ports for emulated exposure rotary encoder */
    PORTB |= (1 << PB1) | (1 << PB2);
    DDRB |= (1 << PB1) | (1 << PB2);

    while (1) {
        receive_command();
    }
}
