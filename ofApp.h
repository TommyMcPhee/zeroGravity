#pragma once

#include "ofMain.h"
#include "sinOsc.hpp"

class ofApp : public ofBaseApp {

public:
	void setup();
	void draw();
	void keyReleased(int key);
	void ofSoundStreamSetup(ofSoundStreamSettings& settings);
	void audioOut(ofSoundBuffer& buffer);
	void checkReset();
	float getFeedbackFrequency(float sample);
	void audioSetup();
	float getRandomFrequency();
	float getRandomPhase();
	void videoSetup();
	void sectionSetup();
	inline float unipolar(float input);
	void refresh();
	void setVectors();
	float transitionValue(int index);
	void setUniforms();
	void beginSection();
	array<array<bool, 8>, 14> sectionValues = { {
		{false, false, false, true, true, true, false, true},
		{false, true, false, true, true, true, false, false},
		{false, true, false, false, true, true, true, false},
		{true, true, false, false, false, true, true, false},
		{true, true, false, true, false, true, false, false},
		{false, true, true, true, true, false, false, false},
		{false, true, true, false, true, false, true, false},
		{true, true, true, false, false, true, false, false},
		{false, false, true, true, true, false, false, true},
		{true, false, true, true, false, false, false, true},
		{true, false, true, false, false, true, false, true},
		{true, false, false, true, false, false, true, true},
		{true, false, false, false, false, true, true, true},
		{false, false, true, false, true, true, false, true} } };
	array<bool, 8> defaultSectionValues;
	array<bool, 8> previousSectionValues;
	array<bool, 8> nextSectionValues;
	array<bool, 8> currentSectionValues;
	array<float, 8> currentValues;
	array<float, 4> transitionalValues;
	ofSoundStreamSettings settings;
	ofSoundStream stream;
	const int sampleRate = 48000;
	const int bufferSize = 256;
	const int channels = 2;
	float minimumFloat;
	sinOsc oscillatorA;
	sinOsc oscillatorB;
	sinOsc oscillatorC;
	sinOsc oscillatorD;
	float fundamentalFrequency;
	bool reset = false;
	float sampleCount;
	//const float length = 210.0;
	float feedback;
	float feedbackIncrement;
	float transitionIncrement = 0.000001;
	ofFbo buffer;
	ofShader shader;
	float oscillatorASample;
	float oscillatorBSample;
	float oscillatorAUnipolar;
	float oscillatorBUnipolar;
	float ringTotal;
	float ringAverage;
	//float oscillatorABSample;
	//float sampleABAverage;
	float oscillatorCSample;
	float oscillatorDSample;
	float oscillatorCDSample;
	float sampleCDTotal;
	float sampleCDAverage;
	float transitionalSeed;
	float ring;
	float frameRate;
	ofVec4f oscillators;
	float width;
	float height;
	ofVec3f window;
	ofVec4f color;
	ofVec4f translate;

	unsigned int sectionIndex;
};