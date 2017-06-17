# sony_camera_interface
Remote interface for Sony cameras driven from MCU (Atmel ATtiny13A)

Camera has one built-in rotary encoder for exposure changes, one button for changes menu enter and one button for OK. There is one more button for reserved purpose.
Backend hardware is hacked into camera and connected to frontend via one RX/TX line. Frontend has all those camera built-in hardware on front panel included. 

### Hardware wiring on frontend

**Rotary encoder**

A - *PB0*

B - *PB1*

**Transmit line**

TX - *PB2*

**Buttons**

Exposure menu - *PB3*

Submit (OK) - *PB4*

Reserved - *PB5*

### Hardware wiring on backend

**Receive line**

RX - *PB0*

**Emulated rotary encoder**

A - *PB1*

B - *PB2*

**Emulated buttons**

Exposure menu - *PB3*

Submit (OK) - *PB4*

Reserved - *PB5*
