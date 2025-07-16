# bagheera-hero-watch

Creates a LED circle that defaults to a slow soft pulsation, showing some effects when specific RFID tags are scanned. We used it to select teams for the cubs.

## Hardware

Raspberry Pi Model B, Arduino Leonardo, 8 x NeoPixel, any random RFID scanner that acts as a keyboard. Also, probably, a small breadboard and a ton of connecting wires

## Software

Compile `main.go` for your raspberry pi, flash `lights/lights.ino` to the arduino. Easy enough.

To make it fully headless, plug and play, I configured raspberry pi os to startup in multi-user mode using `raspi-config` and created a systemd service to auto-start the Go code and feed it all keyboard input. See `bagheera-hero-watch.service` for how that works.

### Playing around with things

If you're playing around with the lighting pattern, if you're making it longer, you probably also want to update the sleep in the rfid reading code to match. Otherwise the Arduino may "miss" signals (and yes, better code could fully remove that problem, but w/e).

## Wiring

***I am NOT an electrical engineer**. Based on what I read online, the following had a good chance of not blowing up my rpi or arduino, and it didn't, but that's fully at your own risk.*

Simply put, there is a chain of 4 components: RFDI Scanner >> RPi >> Arduino >> NeoPixels. Below describes each stepp.

### RFDI Scanner >> RPi

The RFID scanner I used acts like a USB keyboard, so just plug it into any USB port.

### RPi >> Arduino

*This is the part that may not be the greatest idea*.

Depending on what "team" the scanned RFID tag belongs to, the RPi puts +3.3v on a different port. These ports were **directly** connected to 5v input ports on the Arduino. I used digital ports, which read 3.3v as "high", so the signal came trough.

|"Team"|RPi Port|Arduino Port|
|---|---|---|
|Red| GPIO 7 | 2 |
|Green| GPIO 25 | 3 |
|Blue| GPIO 24 | 4 |
|Yellow| GPIO 8 | 5 |

### Arduino & NeoPixels

The Arduino needs to send its commands to the first NeoPixel in the chain. Connect Ardiuno PIN6 to NeoPixel 1 DataIn (which is the one next to its +5v). Connect NeoPixel 1 DataOut (next to GRND) to NeoPixel 2 DataIn. Rinse, repeat. The DataOut of the last NeoPixel doens't need to go anywhere (I connected it to a female-female wire just to be sure).

Also, the Arduino +5v needs to go to the +5v of each NeoPixel. Same for the GRND.

If you've swapped around the DataIn and DataOut on a NeoPixel, the chain will stop working at that point.

If you've swapped around the +5v and GRND on a NeoPixel, it will pop, but data will still pass through it.

If you prick your finger with any of the NeoPixel legs, it will bleed.
