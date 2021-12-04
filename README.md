# EERAM Rally Mini Demo

[![Demonstration Video](/Docs/Media/EERAM-Curiosity-Demo.jpg)](https://vimeo.com/604095048)

## ABOUT
The demo uses the [**47L16**](https://www.microchip.com/47L16), a 3.3V I2C EERAM with a density of 16 Kbits. A 5V version, as well as a 4 Kbit density in 3.3V and 5V, are also available.

EERAM is a standalone SRAM memory with shadow EEPROM backup that helps retain the contents of the SRAM memory when system power is lost. The EERAM uses a small external capacitor (C1 on the board) to provide the energy needed to store the contents of the SRAM on to the EEPROM when system power is lost. Unlike NVSRAM, no external battery is needed. EERAM offers unlimited erase and write cycles to the memory and FRAM-like functionality at a fraction of the price.

The demo is supposed to showcase the special capabilities of this part.

## The Demo Board
The demo board does not contain many components. The arched LED array on the top side is supposed to represent anything from a VU meter to an RPM meter, but it’s up to the customer what interpretation they give it. On the top side there are also two separate LEDs, marked “Play” and “Record” on the silkscreen, as text and icons.

The lower part of the top side has the EERAM itself (U1), and a larger 22uF capacitor (C1) which is the external capacitor mentioned above. On the right-hand side of the (U1) EERAM device, there is a 100nF decoupling capacitor, alongside with two 2k Ohm pull-up resistors necessary for correct I2C bus operation.

The bottom side of the board, which the customer will probably never see, has six series resistors, inline, that are the current limiting resistors for all the LED’s on the board. Below those there is an RC pair, which in this application acts as a voltage ramp limiter, necessary for correct operation.

The board interfaces with two 8pin 2.54mm headers, which are part of the MikroBUS™ specification. MikroBUS™ is a standard that defines mainboard sockets and add-on boards for interfacing microcontrollers with integrate circuits. Microchip currently has a collaboration with MikroBUS™.

## The Curiosity Board
The complete demo is built around one of Microchip’s Curiosity line of boards. The board number used is [**DM164137**](https://www.microchip.com/DM164137).

These boards use the PIC16F1619[**PIC16F1619**](https://www.microchip.com/PIC16F1619) MCU, necessary for the firmware provided. Other microcontrollers might or might not work, so please double check that your Curiosity board uses this MCU.

The primary parts that the user interacts with are:
- The USB connector on the left side of the Curiosity board
- The S1 button switch on the bottom-left corner of the top of the Curiosity board
- The POT1 potentiometer on the bottom-right corner of the top of the Curiosity board
- The MicroBUS™ connector on the top-right corner of the top of the Curiosity board
- The green PCB provided by MPD, that reads “Microchip EERAM”

![Demonstration](/Docs/Media/EERAM-Curiosity-Demo_annotated.jpg)

## Demo Behavior
As mentioned above, the demo is supposed to showcase the behavior of the EERAM part: that it’s able to record information with no endurance penalty (like with writing to EEPROM – which has a limited endurance), while still being able to retain that data indefinitely if power were to be interrupted.

![Demonstration](/Docs/Media/EERAM-Curiosity-Demo_Flow-Diagram.jpg)

Initially, the demo is powered off. Once plugged into an USB power source, the EERAM will recall data from its internal EEPROM array. Once that is done, the arch of LEDs will blink once, signaling the start of a replay cycle. The ‘Play’ LED will also turn on. If the user wants to record a new pattern, they will have to simultaneously press the (S1) button while moving the (POT1) potentiometer in either direction. The ‘Record’ LED lights up.

![Demonstration](/Docs/Media/EERAM-Curiosity-Demo_action.jpg)

The LEDs will light up in accordance with the potentiometer’s angular position. The pinky finger pictured above presses the (S1) switch, while the index operates the potentiometer (POT1). In addition, the user can disconnect the USB, to showcase the Autostore feature of the EERAM. 

## Other Considerations
Due to the density of the Curiosity board, some lines are share between devices. Thus, in order to avoid extra LEDs lighting up on the Curiosity board, the 0 ohm resistors R47, R48, R49 and R50 can all be removed without consequence to the demo. These drive LEDs D4 through D7 on the Curiosity board.

## **Support**
To submit a support ticket request, please contact [Microchip Support](https://microchipsupport.force.com/s/)