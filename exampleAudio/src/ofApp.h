#pragma once

/*

	WIP

	fix feeding buffer into object
	fix load settings for box and addon
	fix broken toggle params
	use smoothedVol (circle) for the addon vu
	define better default variables for styles
	fix dual channel width error. add stereo/mono mode to addon
	add reset styles
	split another window bc too much widgets

*/


#include "ofMain.h"

#include "ofxSurfingImGui.h"
#include "ofxSurfingAudioPlots.h"

class ofApp : public ofBaseApp{
	
	public:
		
		void setup();
		void update();
		void draw();
		void keyPressed(int key);
	
		void drawGui();
		void drawScene();
	
		vector<float> left;
		vector<float> right;
		vector<float> volHistory;
		int bufferCounter;
		int drawCounter;
		float smoothedVol;
		float scaledVol;

		void setupGui();
		ofxSurfingGui ui;
		ofParameter<bool> bGui_Scene{ "Scene", false };
		ofxSurfingBoxInteractive boxScene;

		ofSoundStream soundStream;
		void audioIn(ofSoundBuffer & input);
		ofParameter<float> gain{ "Gain", 0.5, 0, 1 };

		WaveformPlot waveformPlot;

		//TODO: 
		// Testing avoid some thread problems..
		ofMutex waveformMutex;
};
