#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	sampleCount = 0.0;
	audioSetup();
	videoSetup();
	sectionSetup();
}

//--------------------------------------------------------------
void ofApp::draw() {
	refresh();
	buffer.begin();
	shader.begin();
	setUniforms();
	buffer.draw(0, 0);
	shader.end();
	buffer.end();
	buffer.draw(0.0, 0.0);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	if (key > 41 && key < 57 && key != 44 && key != 46) {
		if (key > 47) {
			sectionIndex = key - 48;
		}
		if (key == 47) {
			sectionIndex = 10;
		}
		if (key == 45) {
			sectionIndex = 11;
		}
		if (key == 43) {
			sectionIndex = 12;
		}
		if (key == 42) {
			sectionIndex = 13;
		}
		beginSection();
	}
}

void ofApp::ofSoundStreamSetup(ofSoundStreamSettings& settings) {

}

void ofApp::audioOut(ofSoundBuffer& buffer) {
	for (int a = 0; a < bufferSize; a++) {
		checkReset();
		sampleCount++;
		oscillatorASample = oscillatorA.getSample();
		oscillatorBSample = oscillatorB.getSample();
		oscillatorAUnipolar = unipolar(oscillatorASample);
		oscillatorBUnipolar = unipolar(oscillatorBSample);
		ring = oscillatorASample * oscillatorBSample;
		ringTotal += ring;
		ringAverage = ringTotal / sampleCount;
		transitionIncrement = abs((ringAverage + minimumFloat) / (pow(frameRate + 1.0, 3.0)));
		transitionalSeed -= transitionIncrement * transitionalSeed;
		transitionalValues[1] = transitionalSeed;
		transitionalValues[2] = 1.0 - transitionalValues[1];
		transitionalValues[3] = transitionalValues[1] * transitionalValues[2];
		transitionalValues[0] = 1.0 - transitionalValues[3];
		for (int b = 0; b < 2; b++) {

		}
		feedback += feedbackIncrement;
		sampleCount += feedbackIncrement;
	}
}

void ofApp::checkReset() {
	if (reset) {
		transitionalSeed = 1.0;
		sampleCount = 0.0;
		ringTotal = 0.0;
		reset = false;
	}
}

float ofApp::getFeedbackFrequency(float sample) {
	float absRing = abs(ring);
	return (frameRate * ofSign(sample) * pow((abs(sample) + absRing) / (1.0 + absRing), 2.0)) + std::numeric_limits<float>::min();
}

void ofApp::audioSetup() {
	minimumFloat = std::numeric_limits<float>::min();
	frameRate = ofGetFrameRate();
	fundamentalFrequency = 1.0 / frameRate;
	oscillatorA = sinOsc(fundamentalFrequency, getRandomPhase(), 1.0, sampleRate);
	oscillatorB = sinOsc(fundamentalFrequency / 1.6180339887, getRandomPhase(), 1.0, sampleRate);
	fOscillator = sinOsc(5.457, 0.0, 0.0, sampleRate);
	cOscillator = sinOsc(16.352, 0.0, 0.0, sampleRate);
	gOscillator = sinOsc(48.999, 0.0, 0.0, sampleRate);
	dOscillator = sinOsc(293.665, 0.0, 0.0, sampleRate);
	aOscillator = sinOsc(880.0, 0.0, 0.0, sampleRate);
	eOscillator = sinOsc(2637.02, 0.0, 0.0, sampleRate);
	bOscillator = sinOsc(7902.133, 0.0, 0.0, sampleRate);
	feedbackIncrement = 1.0 / (float)sampleRate;
	transitionalSeed = 1.0;
	settings.setOutListener(this);
	settings.sampleRate = sampleRate;
	settings.bufferSize = bufferSize;
	settings.numOutputChannels = channels;
	settings.setApi(ofSoundDevice::Api::MS_DS);
	stream.setup(settings);
}

float ofApp::getRandomFrequency() {
	return ofRandomf() * frameRate;
}

float ofApp::getRandomPhase() {
	return ofRandomf() * TWO_PI;
}

void ofApp::videoSetup() {
	shader.load("zeroGravity");
	buffer.allocate(ofGetScreenWidth(), ofGetScreenHeight());
	buffer.begin();
	ofClear(0, 0, 0, 255);
	buffer.end();
}

void ofApp::sectionSetup() {

}

inline float ofApp::unipolar(float input) {
	return input * 0.5 + 0.5;
}

void ofApp::refresh() {
	frameRate = ofGetFrameRate();
	width = (float)ofGetWidth();
	height = (float)ofGetHeight();
	buffer.allocate(width, height);
	ofClear(0, 0, 0, 255);
	setVectors();
}

void ofApp::setVectors() {
	window.set(width, height, width * height);
	oscillators.set(oscillatorAUnipolar, oscillatorBUnipolar);
	color.set(transitionValue(0), transitionValue(1), transitionValue(2), transitionValue(3));
	translate.set(transitionValue(4), transitionValue(5), transitionValue(6), transitionValue(7));
}

float ofApp::transitionValue(int index) {
	bool previous = previousSectionValues[index];
	bool next = nextSectionValues[index];
	if (!currentSectionValues[index]) {
		return 0.0;
	}
	else {
		if (previous && next) {
			return transitionalValues[0];
		}
		else {
			if (previous) {
				return transitionalValues[1];
			}
			else {
				if (next) {
					return transitionalValues[2];
				}
				else {
					return transitionalValues[3];
				}
			}
		}
	}
}

void ofApp::setUniforms() {
	shader.setUniform3f("window", window);
	shader.setUniform2f("oscillators", oscillators);
	shader.setUniform4f("translate", translate);
	shader.setUniform4f("color", color);
}

void ofApp::beginSection() {
	reset = true;
	if (sectionIndex == 0) {
		previousSectionValues = defaultSectionValues;
	}
	else {
		previousSectionValues = sectionValues[sectionIndex - 1];
	}
	if (sectionIndex == 13) {
		nextSectionValues = defaultSectionValues;
	}
	else {
		nextSectionValues = sectionValues[sectionIndex + 1];
	}
	currentSectionValues = sectionValues[sectionIndex];
}