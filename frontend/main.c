#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 9600000L
#include <util/delay.h>

#define CLK_DELAY 100

/* Used to identify rising-edge of pin A */
int encoder_prev_state = 0;

/* Storing previous state of buttons to identify falling-edges */
volatile int prev_state_button_exp = 1;
volatile int prev_state_button_ok = 1;
volatile int prev_state_button_sw = 1;

/*
 * This bits are set if buttons are pressed
 * and after the command is sent they are cleared
 */
volatile int is_button_exp_pressed = 0;
volatile int is_button_ok_pressed = 0;
volatile int is_button_sw_pressed = 0;

enum commands {
    EXPOSURE_INC,
    EXPOSURE_DEC,

    BUTTON_EXP,
    BUTTON_OK,
    BUTTON_SW
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
    if (pin_a == 0 && encoder_prev_state != 0) {
        if ((PINB & (1 << PB1)) == 0)
            shift_data_out(EXPOSURE_DEC);
        else
            shift_data_out(EXPOSURE_INC);
    }
    encoder_prev_state = pin_a;
}

/* Send command if button was pressed */
void send_button_command(void)
{
    if (is_button_exp_pressed) {
        shift_data_out(BUTTON_EXP);
        is_button_exp_pressed = 0;
    } else if (is_button_ok_pressed) {
        shift_data_out(BUTTON_OK);
        is_button_ok_pressed = 0;
    } else if (is_button_sw_pressed) {
        shift_data_out(BUTTON_SW);
        is_button_sw_pressed = 0;
    }
}

/* Trigger command sending via transmit line when button pressed (falling-edge) */
ISR(PCINT0_vect)
{
    int actual_state_button_exp = PINB & (1 << PB3);
    int actual_state_button_ok = PINB & (1 << PB4);
    int actual_state_button_sw = PINB & (1 << PB5);

    if (prev_state_button_exp != 0 && actual_state_button_exp == 0)
        is_button_exp_pressed = 1;
    else if (prev_state_button_ok != 0 && actual_state_button_ok == 0)
        is_button_ok_pressed = 1;
    else if (prev_state_button_sw != 0 && actual_state_button_sw == 0)
        is_button_sw_pressed = 1;

    prev_state_button_exp = actual_state_button_exp;
    prev_state_button_ok = actual_state_button_ok;
    prev_state_button_sw = actual_state_button_sw;
}

int main(void)
{
    /* Setup input ports for exposure rotary encoder */
    PORTB |= (1 << PB0) | (1 << PB1);
    DDRB &= ~((1 << PB0) | (1 << PB1));

    /* Setup output port for transmit line */
    PORTB |= (1 << PB2);
    DDRB |= (1 << PB2);

    /* Setup input ports for buttons */
    PORTB |= (1 << PB3) | (1 << PB4) | (1 << PB5);
    DDRB &= ~((1 << PB3) | (1 << PB4) | (1 << PB5));

    /* Enable pin change interrupt on button's ports */
    GIMSK = 0x20;
    PCMSK = 0x38;
    sei();

    while (1) {
        read_exposure_encoder();
        send_button_command();
    }
}
