#pragma once

/*

	WIP
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


		//TODO: other audio widgets

		//IMGUI_API bool Fader(const char* label, const ImVec2& size, int* v, const int v_min, const int v_max, const char* format = nullptr, float power = 1.0f);


		//IMGUI_API void UvMeter(char const* label, ImVec2 const& size, int* value, int v_min, int v_max, int steps = 10, int* stack = nullptr, int* count = nullptr);
		//IMGUI_API void UvMeter(char const* label, ImVec2 const& size, float* value, float v_min, float v_max, int steps = 10, float* stack = nullptr, int* count = nullptr);
		//IMGUI_API void UvMeter(ImDrawList* draw_list, char const* label, ImVec2 const& size, int* value, int v_min, int v_max, int steps = 10, int* stack = nullptr, int* count = nullptr);
		//IMGUI_API void UvMeter(ImDrawList* draw_list, char const* label, ImVec2 const& size, float* value, float v_min, float v_max, int steps = 10, float* stack = nullptr, int*
};
