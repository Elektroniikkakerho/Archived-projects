> Tämä projektikuvaus on siirretty tänne elektroniikkakerhon vanhoilta verkkosivuilta.

[Projektin etusivu](README.md)

# AVR Speden Spelit - Operation of the circuit

### Common things

The core of this device is Atmel's microcontroller AT90S2313 (later referenced as mcu). In addition to that, few components are needed. They are 8-bit shift-register 74HC595, two NPN-transistors and a handful of passive components. And, of course, the device also needs four buttons and lights and a two-digit seven-segment display.


### Buttons

Each of the four buttons needs one capasitor and three resistors to function correctly. The buttons must be such that the poles of a button are connected when the button is pressed. When a button is released, the corresponding reading pin of the mcu is connected to ground via a 4.7 kohm pull-down resistor. When pressed, a button connects 5 V (logical 1) to the reading pin via a 100 ohm resistor forming a resistive voltage-divider. However, the attenuation of the voltage-divider is barely noticeable. Thus, because of these two resistors, the reading pin is in ground potential (logical 0) when the corresponding button is not pressed and when the button is pressed, the voltage of the reading pin will rise to 5 V (logical 1).

Electrical switches (buttons) are never precise. When one's state is changed, there always occurs unwanted vibrations during which it is impossible to determine the state of the switch. Thus, the mcu may "see" many pushes when a button is actually pressed only once. That would make playing the game impossible. That is why there are four capasitors in the schematic diagram, wired parallel with the 4.7 kohm resistors. As is known, the voltage across a capasitor can't change instantaneously but little by little and so the rapid voltage changes caused by the switch vibrations are filtered away. When a button is pressed, the corresponding capasitor is charged to 5 V (logical 1) via a 100 ohm pull-up resistor. When the button is released, the charge runs down throgh the 4.7 komh resistor and the voltage descends to 0 V. This prevents the vibrations from disturbing playing the game.

When the device is switched off, the filtering capasitors of the buttons may still remain charged. If this charge was let to run down through the mcu freely, the mcu could be damaged. To prevent that, there is a third resistor wired between the capasitor and the reading pin of each button.


### Controlling the lights

Every light has a control pin of it's own in the mcu. Either leds or bulbs can be used as lights. When leds are used, their anodes are wired to 5 V and their cathodes are wired via about 175 ohm resistors to the control pins of the leds in the mcu. The resistance, 175 ohm, works with red, yellow and green leds. If there is a need to use some other color, the value must be recalculated considering the threshold voltage and the current draw of the led wanted to be used.

When using bulbs, a transistor is needed between each bulb and the mcu because of the limited ability of the mcu to source current. This device uses universal NPN-transistors. Their emitters are connected to ground, their bases via resistors to the control pins in the mcu and their collectors to ones of the terminals of the bulbs. The other terminals are wired to a voltage source. The voltage needed and the possible need of serial resistors depend on the bulb.

Allthough the selection between leds and bulbs affects the wiring a little, it doesn't affect the operation of the mcu. Thus the same code works with both alternatives.


### Controlling the score diplays

The device uses a two seven-segment led-displays to display score. Controlling the displays requires 2 + 7 control pins. Two of them control the transistors, which are used as switches for the displays, connecting the common anodes of the displays to Vcc. The display chosen to be switched on can be activated by giving base current to the corresponding transistor by setting it's control pin in the mcu into voltage of 5 V (logical 1). The rest of the required control pins (seven) are used to connect the cathodes of the segments to ground or to 5 V via resistors. When a cathode is grounded, a current flows through the led and it becomes lighted. When the cathode is connected to Vcc, no current flows and thus there is no light.

So the desired digit can be made to show up on one of the two displays by setting the seven pull-down pins to states that correspond to the digit's seven-segment code and selecting display using the switch transistors. This way any digit can be displayed on either or both of the displays. But there are two displays and in most cases it is required to have them display different digits simultaneously. Real simultaneous operation is not possible with the displays multiplexed as they are in this device, but they can still be made to seem like they displayed different digits at same time. This is done by flashing the displays by turns. If the flashing frequency is high enough, human eye doesn't recognise the flashing but thinks that the displays are both on simultaneously. The flashing must be done rapidly enough, the minimum frequency being around 50 Hz. This device uses as high frequency as about 250 Hz, which is much more than enough, but still not too much.

As stated earlier, controlling the displays requires nine control pins. The mcu used in this device however doesn't have enough of them, so a little trick must be used. Pulling down the desired cathodes is done using an other chip, 8-bit shift-register 74HC595. In this case it needs three control pins and can transform eight bits of serial data to parallel form. That means that by using this shift-register, three control pins of the mcu can be turned into eight. Because there is need for only seven pull-down pins, one of the shift-register's outputs is not used. That output may be used for any application felt necessary, like a buzzer.

The two transistors are controlled directly using two control pins of the mcu and controlling the shift-register requires three pins. Thus, by using the shift-register, a function, that would usually require nine control pins, can be realized using only five pins, which is little enough for the mcu used.

---

Copyright Antti Gärding 2003, 2004
