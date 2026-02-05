#include <arduinoFFT.h>
#include <LiquidCrystal.h>

#define SAMPLES 128  // Bit rate
#define SAMPLING_FREQ 8000  // Hz
#define ANALOG_PIN A0

float vReal[SAMPLES];
float vImag[SAMPLES];

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
ArduinoFFT<float> FFT(vReal, vImag, SAMPLES, SAMPLING_FREQ);

void setup() {
	lcd.begin(16, 2);
	lcd.print("Guitar Tuner");
	delay(1500);
	lcd.clear();
	Serial.begin(9600);
}

void loop() {
	// Read samples
	for (int i = 0; i < SAMPLES; i++) {
		vReal[i] = analogRead(ANALOG_PIN);
		vImag[i] = 0.0;
		delayMicroseconds(1000000 / SAMPLING_FREQ);
	}

	// FFT configuration and computation
	FFT.windowing(FFTWindow::Hamming, FFTDirection::Forward);
	FFT.compute(FFTDirection::Forward);
	FFT.complexToMagnitude();

	// Find peak bin with interpolation
	int peakIndex = 1;
	float maxMag = vReal[1];
	for (int i = 2; i < SAMPLES / 2 - 1; i++) {
		if (vReal[i] > maxMag) {
			maxMag = vReal[i];
			peakIndex = i;
		}
	}

	// Parabolic interpolation to improve peak accuracy
	float alpha = vReal[peakIndex - 1];
	float beta = vReal[peakIndex];
	float gamma = vReal[peakIndex + 1];
	float interp = 0.5f * (alpha - gamma) / (alpha - 2 * beta + gamma);
	float trueIndex = peakIndex + interp;

	float frequency = trueIndex * ((float)SAMPLING_FREQ / SAMPLES);

	Serial.print(frequency);  // Print frequency to serial for debugging and plotting

	// Display frequency and note on LCD screen
	lcd.setCursor(0, 0);
	lcd.print("Freq: ");
	lcd.print(frequency, 1);
	lcd.print(" Hz   ");

	String note = noteFromFrequency(frequency);
	lcd.setCursor(0, 1);
	lcd.print("Note: ");
	lcd.print(note);
	lcd.print("     ");

	Serial.print("Freq: ");
	Serial.println(frequency, 2);

	delay(200);
}

/**
 * Returns a string representing the musical note that corresponds to the given frequency.
 * If the frequency is below 60 Hz or above 659 Hz, returns "Too Low" or "Too High" respectively.
 * Otherwise, returns the note name as a string (e.g. "E2", "F#4", etc.).
 *
 * @param freq The frequency in Hz to convert to a note string.
 * @return A string representing the musical note corresponding to the given frequency.
 */
String noteFromFrequency(float freq) {
	if (freq < 60) return "Too Low";
	if (freq < 90) return "E2";
	if (freq < 110) return "F#2";
	if (freq < 123.5) return "G#2";
	if (freq < 147) return "A2";
	if (freq < 165) return "B2";
	if (freq < 185) return "C#3";
	if (freq < 196) return "D3";
	if (freq < 220) return "E3";
	if (freq < 247) return "F#3";
	if (freq < 262) return "G3";
	if (freq < 294) return "A3";
	if (freq < 330) return "B3";
	if (freq < 349) return "C#4";
	if (freq < 370) return "D4";
	if (freq < 392) return "E4";
	if (freq < 440) return "F#4";
	if (freq < 494) return "G#4";
	if (freq < 523) return "A4";
	if (freq < 587) return "B4";
	if (freq < 659) return "C#5";
	return "Too High";
}
