#pragma once

/*

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
		void drawGui();
		void drawScene();
		void keyPressed(int key);
				
		void audioIn(ofSoundBuffer & input);
	
		vector<float> left;
		vector<float> right;
		vector<float> volHistory;
		
		int bufferCounter;
		int drawCounter;
		
		float smoothedVol;
		float scaledVol;
		
		ofSoundStream soundStream;

		void setupGui();
		ofxSurfingGui ui;

		WaveformPlot waveformPlot;
		
		ofParameter<bool> bGui_Scene{ "Scene", false };
		ofxSurfingBoxInteractive boxScene;
};
