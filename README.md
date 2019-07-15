# Neopixel Lanyard
Made for Defcon 27

Inspired by attendance at Defcon 26 I wanted to fabricate and bring something unique with me to Defcon 27. Experimenting recently with PCB fabrication I thought I would attempt to make a Neopixel Lanyard that could double as a badge holder AND accomodate both SAO standards.

Total parts cost ~$40/unit.

Contained within this repository is everything you *should* need to fabricate and modify the lanyard as you desire.

### Project Keywords
- MCU - Attiny85
- Arduino Stack
- Neopixel

## Software Credits:
- [Adafruit The magic of neopixels](https://learn.adafruit.com/adafruit-neopixel-uberguide/the-magic-of-neopixels)
- [DefCon SAO](https://hackaday.io/project/52950-shitty-add-ons)

## Hardware Programming Tools:
- [SparkFun ISP Pogo Adapter](https://www.sparkfun.com/products/11591)
- [Tiny AVR Programmer](https://www.sparkfun.com/products/11801)
- AVR Programming Cable 2x3

## Other Materials
- [Bill of Material (aka BOM)](https://github.com/daneallen/neopixelLanyard/BOM) for all parts outlines in great detail every part necassary for fabricating the PCB
  - PCB Gerber and/or KiCad files are available for fabrication with your favorite vendor.
- Lanyard Ribbon (aka Polyester Tubing) from a fabric store (i.e. JoAnns)
  - Thread to stitch the final parts together.
- Thin and flexible wire to conenct the Neopixel strip to the PCB. I used [Adafruits Silicone Cover Stranded-Core Wire](https://www.adafruit.com/product/3165?gclid=Cj0KCQjwpavpBRDQARIsAPfTwizGN_g7z_eFXelhqXAeSSqgvykAK18QdwBzPNuDS2eKq5C_RoPe8C0aAj8EEALw_wcB)
- 3mm-.5 6mm tapered bolt and screw used to securly fashion the battery pack to the PCB. Ace Hardware was great for these small parts.
- x3 AAA batteries

## Instructions
1. Collect all of the necassary parts (i.e. manufacture the PCB, order from Digikey, etc).
2. Populate the PCB. 
    - Solder the battery pack LAST!
    - Ensure that the through hole components are clipped as flush as possible on the backside to ensure tha the battery pack unit sits flush with the back of the PCB.
    - Once satisfied and op-checked solder the battery pack leads last (from the front of the PCB).
3. Thread the Neopixel strip through the lanyard material. This is slow going, about 1-2 inches per minute by hand.
4. Carefully select the desired length and hand stitch the filled lanyard to the PCB
5. Using the AVR ISP tools modify/compile/upload the source code directly to the PCB.
    - Don't forget to "Burn Bootloader" as the first step in the load process since the source code is built utilizing the Arduino stack. The bootloader is only loaded once per PCB.
