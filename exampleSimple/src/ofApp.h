#pragma once

#include "ofMain.h"

#include "ofxSurfingImGui.h"
#include "ofxSurfingAudioPlots.h"

class ofApp : public ofBaseApp{
	
	public:
		
		void setup();
		void update();
		void draw();
		void keyPressed(int key);
	
		void setupGui();
		void drawGui();
		ofxSurfingGui ui;

		ofSoundStream soundStream;
		// hardcoded
		const int nChannels = 2;
		const int bufferSize = 512;

		void audioIn(ofSoundBuffer & input);
		vector<float> left;
		vector<float> right;
		ofParameter<float> gain{ "Gain", 0.5, 0, 1 };

		WaveformPlot waveformPlot;

		//TODO: 
		// Testing avoid some thread problems..
		ofMutex waveformMutex;
};
