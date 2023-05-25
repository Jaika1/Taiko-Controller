# Jaika's Taiko Drum Controller/Interface
I am creating this with the hopes of some day providing a free and open-source full-design for people to make their own controllers from using inexpensive parts that are readily available.

## Hardware Setup
### You will need
- Arduino Leonardo or compatible equivalent
- 4x Piezo transducer/sensor
- 4x 10K resistor*
- 4x 56N capacitor*

<sub>*Please adjust these values as required using the serial plotter as reference. Some setups may not require the capacitors.</sub>

### My currently working schematic.
![Layout schematic](https://bluesquid.ink/static/projects/taiko-controller/schematic.png)

## Future/possible improvements
- Bring the polling rate up to 1KHz as is found in the Taikoller
- Integrate TVS diodes or similar to ensure piezo doesn't kill Arduino (Currently V is regulated by RC)
- Integrate a screen and a UI to adjust settings on the device itself
- Find a smaller, more capable board/MCU for this task, especially one with USB-B or USB-C. (micro-b sucks!)
- Create a PCB design and 3D printable case for the electronics.
- Create arcade-perfect schematics for the drum assembly to be milled on a CNC.
- Fully documented assembly guide for a complete controller, start to finish.