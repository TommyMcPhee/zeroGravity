#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	feedback = 1.0;
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
		if (key < 47) {
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
	cout << key << endl;
}

void ofApp::ofSoundStreamSetup(ofSoundStreamSettings& settings) {

}

void ofApp::audioOut(ofSoundBuffer& buffer) {
	for (int a = 0; a < bufferSize; a++) {
		oscillatorASample = unipolar(oscillatorA.getSample());
		oscillatorBSample = unipolar(oscillatorB.getSample());
		ring = oscillatorASample * oscillatorBSample;
		frameRate = ofGetFrameRate();
		oscillatorC.setFreq(getFeedbackFrequency(oscillatorDSample));
		oscillatorD.setFreq(getFeedbackFrequency(oscillatorCSample));
		oscillatorCSample = oscillatorC.getSample();
		oscillatorDSample = oscillatorD.getSample();
		oscillators.set(oscillatorASample, oscillatorBSample, oscillatorCSample, oscillatorDSample);
		color.set(transitionValue(0), transitionValue(1), transitionValue(2), transitionValue(3));
		translate.set(transitionValue(4), transitionValue(5), transitionValue(6), transitionValue(7));
		for (int b = 0; b < 2; b++) {

		}
		//feedback += feedbackIncrement;
	}
}

float ofApp::getFeedbackFrequency(float sample) {
	float absRing = abs(ring);
	return frameRate * (sample + absRing) / (1.0 + absRing);
}

void ofApp::audioSetup() {
	frameRate = ofGetFrameRate();
	fundamentalFrequency = (0.2 * ofRandomf() + 0.2) / frameRate;
	oscillatorA = sinOsc(fundamentalFrequency, getRandomPhase(), 1.0, sampleRate);
	oscillatorB = sinOsc(1.6180339887 / fundamentalFrequency, getRandomPhase(), 1.0, sampleRate);
	oscillatorC = sinOsc(getRandomFrequency(), getRandomPhase(), 1.0, sampleRate);
	oscillatorD = sinOsc(getRandomFrequency(), getRandomPhase(), 1.0, sampleRate);
	//feedbackIncrement = 1.0 / (length * (float)sampleRate);
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
	width = (float)ofGetWidth();
	height = (float)ofGetHeight();
	buffer.allocate(width, height);
	window.set(width, height, width * height);
	ofClear(0, 0, 0, 255);
	setVectors();
}

void ofApp::setVectors() {
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
	shader.setUniform4f("oscillators", oscillators);
	//shader.setUniform1f("feedback", feedback);
	shader.setUniform4f("translate", translate);
	shader.setUniform4f("color", color);
}

void ofApp::beginSection() {
	oscillatorC.setFreq(getRandomFrequency());
	oscillatorD.setFreq(getRandomFrequency());
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