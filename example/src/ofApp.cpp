#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetVerticalSync(true);
	ofSetCircleResolution(80);
	ofBackground(54, 54, 54);

	boxScene.setName("Scene");
	//bGui_Scene.makeReferenceTo(boxScene.bGui);
	boxScene.bGui.makeReferenceTo(bGui_Scene);
	boxScene.setup();
	boxScene.setUseBorder(true);
	boxScene.setWidth(1000);
	boxScene.setHeight(650);

	//-

	int bufferSize = 256;

	left.assign(bufferSize, 0.0);
	right.assign(bufferSize, 0.0);
	volHistory.assign(400, 0.0);

	bufferCounter = 0;
	drawCounter = 0;
	smoothedVol = 0.0;
	scaledVol = 0.0;

	//-

	soundStream.printDeviceList();
	auto devices = soundStream.getDeviceList(ofSoundDevice::Api::MS_DS);

	if (!devices.empty()) 
	{
		ofSoundStreamSettings settings;
	
		int d = 7;
		settings.setInDevice(devices[d]);
		settings.setApi(ofSoundDevice::Api::MS_DS);
		settings.sampleRate = 48000;
		settings.numOutputChannels = 0;
		settings.numInputChannels = 2;
		settings.bufferSize = bufferSize;
		settings.setInListener(this);

		soundStream.setup(settings);
	}

	//-
	
	setupGui();
}

//--------------------------------------------------------------
void ofApp::setupGui()
{
	ofLogNotice() << (__FUNCTION__);

	ui.setName("ofApp");
	ui.setWindowsMode(IM_GUI_MODE_WINDOWS_SPECIAL_ORGANIZER);
	ui.setup();

	ui.addWindowSpecial(waveformPlot.bGui_Main);
	ui.addWindowSpecial(waveformPlot.bGui_Edit);
#ifdef USE_WAVEFORM_3D_OBJECT
	ui.addWindowSpecial(waveformPlot.o.bGui);
#endif

	// Plot
	//ui.addWindowSpecial(waveformPlot.bGui_Main);

	ui.startup();

	// stack windows vertically/horizontally
	ui.setWindowsSpecialsOrientation(false);

	waveformPlot.setUiPtr(&ui);
}

//--------------------------------------------------------------
void ofApp::update() {
	//lets scale the vol up to a 0-1 range 
	scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);

	//lets record the volume into an array
	volHistory.push_back(scaledVol);

	//if we are bigger the the size we want to record - lets drop the oldest value
	if (volHistory.size() >= 400) {
		volHistory.erase(volHistory.begin(), volHistory.begin() + 1);
	}

	waveformPlot.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	if(bGui_Scene) drawScene();
	drawGui();
}

//--------------------------------------------------------------
void ofApp::drawGui()
{
	ui.Begin();
	{
		if (ui.BeginWindow("ofApp")) {

			ui.Add(waveformPlot.bGui, OFX_IM_TOGGLE_ROUNDED);
			ui.Add(waveformPlot.bGui_PlotIn, OFX_IM_TOGGLE_ROUNDED_MINI);
			ui.AddSpacingBigSeparated();

			ui.Add(bGui_Scene, OFX_IM_TOGGLE_ROUNDED);

			ui.EndWindow();
		}

		waveformPlot.drawImGui();
	}
	ui.End();

	waveformPlot.drawPlots();
}

//--------------------------------------------------------------
void ofApp::drawScene() {

	ofPushStyle();
	ofPushMatrix();
	ofTranslate(boxScene.getX(), boxScene.getY());

	ofSetColor(225);
	ofDrawBitmapString("AUDIO INPUT EXAMPLE", 32, 32);
	ofDrawBitmapString("press 's' to unpause the audio\n'e' to pause the audio", 31, 92);

	ofNoFill();

	// draw the left channel:
	ofPushStyle();
	ofPushMatrix();
	ofTranslate(32, 170, 0);

	ofSetColor(225);
	ofDrawBitmapString("Left Channel", 4, 18);

	ofSetLineWidth(1);
	ofDrawRectangle(0, 0, 512, 200);

	ofSetColor(245, 58, 135);
	ofSetLineWidth(3);

	ofBeginShape();
	for (unsigned int i = 0; i < left.size(); i++) {
		ofVertex(i * 2, 100 - left[i] * 180.0f);
	}
	ofEndShape(false);

	ofPopMatrix();
	ofPopStyle();

	// draw the right channel:
	ofPushStyle();
	ofPushMatrix();
	ofTranslate(32, 370, 0);

	ofSetColor(225);
	ofDrawBitmapString("Right Channel", 4, 18);

	ofSetLineWidth(1);
	ofDrawRectangle(0, 0, 512, 200);

	ofSetColor(245, 58, 135);
	ofSetLineWidth(3);

	ofBeginShape();
	for (unsigned int i = 0; i < right.size(); i++) {
		ofVertex(i * 2, 100 - right[i] * 180.0f);
	}
	ofEndShape(false);

	ofPopMatrix();
	ofPopStyle();

	// draw the average volume:
	ofPushStyle();
	ofPushMatrix();
	ofTranslate(565, 170, 0);

	ofSetColor(225);
	ofDrawBitmapString("Scaled average vol (0-100): " + ofToString(scaledVol * 100.0, 0), 4, 18);
	ofDrawRectangle(0, 0, 400, 400);

	ofSetColor(245, 58, 135);
	ofFill();
	ofDrawCircle(200, 200, scaledVol * 190.0f);

	// lets draw the volume history as a graph
	ofBeginShape();
	for (unsigned int i = 0; i < volHistory.size(); i++) {
		if (i == 0) ofVertex(i, 400);

		ofVertex(i, 400 - volHistory[i] * 70);

		if (i == volHistory.size() - 1) ofVertex(i, 400);
	}
	ofEndShape(false);

	ofPopMatrix();
	ofPopStyle();

	drawCounter++;

	ofSetColor(225);
	string reportString = "buffers received: " + ofToString(bufferCounter) + "\ndraw routines called: " + ofToString(drawCounter) + "\nticks: " + ofToString(soundStream.getTickCount());
	ofDrawBitmapString(reportString, 32, 589);

	ofPopMatrix();
	ofPopStyle();


	boxScene.draw();
	boxScene.drawBorder();
}

//--------------------------------------------------------------
void ofApp::audioIn(ofSoundBuffer& input) {

	float curVol = 0.0;

	// samples are "interleaved"
	int numCounted = 0;

	//lets go through each sample and calculate the root mean square which is a rough way to calculate volume	
	for (size_t i = 0; i < input.getNumFrames(); i++) {
		left[i] = input[i * 2] * 0.5;
		right[i] = input[i * 2 + 1] * 0.5;

		curVol += left[i] * left[i];
		curVol += right[i] * right[i];
		numCounted += 2;

		//--
	
		// feed
		//waveformPlot.plotIn[i] = input[i * 2];
		waveformPlot.plotIn[i] = left[i];
	}

	//this is how we get the mean of rms :) 
	curVol /= (float)numCounted;

	// this is how we get the root of rms :) 
	curVol = sqrt(curVol);

	smoothedVol *= 0.93;
	smoothedVol += 0.07 * curVol;

	bufferCounter++;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == 's') {
		soundStream.start();
	}

	if (key == 'e') {
		soundStream.stop();
	}
}

