#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	feedback = 1.0;
	audioSetup();
	videoSetup();
	sectionSetup();
}

//--------------------------------------------------------------
void ofApp::draw(){
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
void ofApp::keyReleased(int key){
	if(key > 41 && key < 57 && key != 44 && key != 46){
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

void ofApp::audioOut(ofSoundBuffer &buffer) {
	for (int a = 0; a < bufferSize; a++) {
		oscillatorASample = unipolar(oscillatorA.getSample());
		oscillatorBSample = unipolar(oscillatorB.getSample());
		ring = oscillatorASample * oscillatorBSample;
		oscillators.set(oscillatorASample, oscillatorBSample);
		color.set(1.0, 1.0, 1.0, 1.0);
		translate.set(oscillatorASample * -1.0, oscillatorBSample * -1.0, oscillatorASample + oscillatorBSample, oscillatorASample - oscillatorBSample);
		//feedback += feedbackIncrement;
	}
}

void ofApp::audioSetup() {
	oscillatorA = sinOsc(1.0 / length, 0.0, 1.0, sampleRate);
	oscillatorB = sinOsc(1.6180339887 / length, 0.0, 1.0, sampleRate);
	//feedbackIncrement = 1.0 / (length * (float)sampleRate);
	settings.setOutListener(this);
	settings.sampleRate = sampleRate;
	settings.bufferSize = bufferSize;
	settings.numOutputChannels = channels;
	settings.setApi(ofSoundDevice::Api::MS_DS);
	stream.setup(settings);
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
	window.set(width, height);
	ofClear(0, 0, 0, 255);
	setVectors();
}

void ofApp::setVectors() {

}

float ofApp::transitionValue(int index, float currentValue) {
	bool previous = previousSectionValues[index];
	bool next = nextSectionValues[index];
	if (!currentSectionValues[index]) {
		return 0.0;
	}
	else {
		if (previous && next) {
			bellFade(currentValue);
		}
		else {
			if (previous) {
				decrementFade(currentValue);
			}
			else {
				if (next) {
					incrementFade(currentValue);
				}
				else {
					inverseBellFade(currentValue);
				}
			}
		}
	}
}

float ofApp::bellFade(float currentValue) {
	return 0;
}

float ofApp::incrementFade(float currentValue) {
	float inverseCurrentValue = 1.0 - currentValue;
	float newValue = currentValue + randomIncrement(inverseCurrentValue, inverseCurrentValue);
	return newValue;
}

float ofApp::decrementFade(float currentValue) {
	float inverseCurrentValue = 1.0 - currentValue;
	return currentValue - randomIncrement(inverseCurrentValue, inverseCurrentValue);
}

float ofApp::inverseBellFade(float currentValue) {
	return 0;
}

float ofApp::randomIncrement(float bias, float maximum) {
	return pow(maximum * (ring + bias) / (1.0 + bias), 10.0);
}

void ofApp::setUniforms() {
	shader.setUniform2f("window", window);
	shader.setUniform2f("oscillators", oscillators);
	shader.setUniform1f("feedback", feedback);
	shader.setUniform4f("translate", translate);
	shader.setUniform4f("color", color);
}

void ofApp::beginSection() {
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
	//color.set(currentSectionValues[0], currentSectionValues[1], currentSectionValues[2], currentSectionValues[3]);
	//translate.set(currentSectionValues[4], currentSectionValues[5], currentSectionValues[6], currentSectionValues[7]);
}