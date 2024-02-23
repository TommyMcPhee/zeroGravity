#pragma once

#include "ofMain.h"
#include "sinOsc.hpp"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void draw();
		void keyReleased(int key);
		void ofSoundStreamSetup(ofSoundStreamSettings &settings);
		void audioOut(ofSoundBuffer &buffer);

		void audioSetup();
		void videoSetup();
		void sectionSetup();
		inline float unipolar(float input);
		void refresh();
		void setVectors();
		float transitionValue(int index, float currentValue);
		float bellFade(float currentValue);
		float decrementFade(float currentValue);
		float incrementFade(float currentValue);
		float inverseBellFade(float currentValue);
		float randomIncrement(float bias, float maximum);
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
		ofSoundStreamSettings settings;
		ofSoundStream stream;
		const int sampleRate = 48000;
		const int bufferSize = 256;
		const int channels = 2;
		sinOsc oscillatorA;
		sinOsc oscillatorB;
		const float length = 210.0;
		float feedback;
		float feedbackIncrement;
		float transitionIncrement;
		ofFbo buffer;
		ofShader shader;
		float oscillatorASample;
		float oscillatorBSample;
		float ring;
		ofVec2f oscillators;
		float width;
		float height;
		ofVec2f window;
		ofVec4f color;
		ofVec4f translate;

		unsigned int sectionIndex;
};
