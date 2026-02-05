# Guitar FFT Arduino

This project started as a personal summer experiment to get hands-on experience with the Arduino IDE, analog signal conditioning, and FFT-based frequency analysis. It takes a raw guitar signal (with straight single coiled pickups used in testing), conditions it using an analog preamp (from an old Crate Amplifiers Board), and then performs a Fast Fourier Transform (FFT) on the Arduino to estimate the fundamental frequency and musical note.

The project is still a work in progress, but it’s functional and meant to be educational - both for me and for anyone curious about guitar signal processing on microcontrollers.

## Picture(s)

Here is a picture of the circuit I had made for this project.
![Circuit](/messy_circuit_picture.jpeg)

## Hardware Used

- Elgoo Arduino Uno
- Potentiometer (for LCD brightness & contrast)
- 16x2 LCD
- 1/4" Guitar Jack
- TL072 / TL082 (dual JFET-input op-amp)
- Breadboard & jumper wires

Passive components (resistors, capacitors) are also used.

## Software Used

- Arduino IDE
- Fast Fourier Transform (FFT) library
	```
	#include <arduinoFFT.h>
	```
- LiquidCrystal library
  	```
	#include <LiquidCrystal.h>
	```
## How it Works (in theory at least)
1. Guitar Input
   - The guitar signal enters through a standard ¼″ jack.
   - The signal is AC-coupled to remove DC offset.

2. Analog Signal Conditioning
   - A TL072/TL082 op-amp is used as a high-impedance input buffer.
   - The signal is biased to mid-rail so it can be safely read by the Arduino’s ADC.
   - A capacitor is used to condition the signal. This prevents pickup loading and improves frequency accuracy.

3. Sampling & FFT
   - The Arduino samples the conditioned signal at a fixed rate.
   - An FFT is performed on the sampled data to find the dominant frequency.
   - The detected frequency is mapped to the nearest musical note.

4. Display
   - Frequency (Hz) and note name are displayed on a 16×2 LCD.
   - Potentiometers are used to adjust LCD contrast and brightness.

## Why use TL072 / TL082 op-amp?
* Very high input impedance, ideal for guitar pickups
* Low noise for audio applications
* Commonly used in analog guitar amplifiers and pedals
* Easily reused from salvaged gear (this one came from a Crate amp!)