# sony_camera_interface
Remote interface for Sony cameras driven from MCU

Camera has one built-in rotary encoder for exposure changes, one button for changes menu enter and one button for OK. There is one more button for reserved purpose.
Backend hardwa is hacked in camera and connected to frontend via one RX/TX line. Frontend has all those camera built-in hardware on front panel included. 

### Hardware wiring

**Rotary encoder**

(Frontend) A - *PB0*

(Frontend) B - *PB1*

**Transmit line**

(Frontend) TX - *PB2*
