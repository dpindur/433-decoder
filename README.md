# 433 Decoder

Arduino program to decode transmissions using a 433Mhz receiver. In theory it should work with any receiver hooked up to the Arduino.

## Operation
Registers an interrupt handler which is called everytime there is a change in voltage from the receiver. The time between changes is pushed onto the *timings* ring buffer. This loops around until a synchronisation signal is detected. Sync signal is configured to be 15000 microseconds of silence. After the sync signal is received it continues to record until *timings* is full. When *timings* is full the interrupt handler is removed and the output of timings is printed.