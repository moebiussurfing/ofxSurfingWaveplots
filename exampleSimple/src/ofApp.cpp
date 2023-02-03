#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	left.assign(bufferSize, 0.0);
	right.assign(bufferSize, 0.0);

	//--

	soundStream.printDeviceList();//NOTE: look your system devices here!
	const int d = 7;//NOTE: pick your device here!

	auto devices = soundStream.getDeviceList(ofSoundDevice::Api::MS_DS);
	if (!devices.empty())
	{
		ofSoundStreamSettings settings;
		settings.setInDevice(devices[d]);
		settings.setApi(ofSoundDevice::Api::MS_DS);
		settings.sampleRate = 48000;
		settings.numOutputChannels = 0;
		settings.numInputChannels = nChannels;
		settings.bufferSize = bufferSize;
		settings.setInListener(this);

		soundStream.setup(settings);
	}

	//--

	setupGui();

	//--

	// waveform
	waveformPlot.setup();
	waveformPlot.setUiPtr(&ui);
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
}

//--------------------------------------------------------------
void ofApp::update()
{
	waveformPlot.update();
}

//--------------------------------------------------------------
void ofApp::draw()
{
	drawGui();

	//--

	// Plots
	waveformPlot.drawPlots();
}

//--------------------------------------------------------------
void ofApp::drawGui()
{
	//TODO: 
	// Testing avoid some thread problems..
	ofScopedLock waveformLock(waveformMutex);

	// Gui
	ui.Begin();
	{
		if (ui.BeginWindow("ofApp"))
		{
			ui.AddMinimizerToggle();
			ui.AddExtraToggle();
			ui.AddSpacingBigSeparated();

			ui.Add(gain, OFX_IM_HSLIDER);

			ui.AddSpacingBigSeparated();

			//--

			// Commonly used controls
			// for the WaveformPlot object
			{
				ui.Add(waveformPlot.bGui, OFX_IM_TOGGLE_ROUNDED_MEDIUM);
				if (waveformPlot.bGui && !ui.bExtra)
				{
					ui.Indent();
					ui.Add(waveformPlot.bGui_Main, OFX_IM_TOGGLE_ROUNDED_SMALL);
					ui.Add(waveformPlot.bGui_Edit, OFX_IM_TOGGLE_ROUNDED_SMALL);
					ui.Unindent();
				}

				ui.AddSpacing();
				ui.Add(waveformPlot.bGui_Plots, OFX_IM_TOGGLE_ROUNDED_MEDIUM);
				if (waveformPlot.bGui_Plots)
				{
					// Center a single widget
					{
						// Pass the expected widget width divided by two
						float w = ui.getWidgetsWidth(2) / 2;
						AddSpacingPad(w);
						ui.Add(waveformPlot.gain, OFX_IM_KNOB_TICKKNOB, 2);
					}
				}
			}

			//--

			ui.EndWindow();
		}

		//--

		waveformPlot.drawImGui(ui.bExtra);
	}
	ui.End();
}

//--------------------------------------------------------------
void ofApp::audioIn(ofSoundBuffer& input) 
{
	//TODO: 
	// Testing avoid some thread problems..
	ofScopedLock waveformLock(waveformMutex);

	float g = gain.get();

	// Lets go through each sample 
	// and calculate the root mean square 
	// which is a rough way to calculate volume	
	for (size_t i = 0; i < input.getNumFrames(); i++)
	{
		left[i] = input[i * nChannels] * g;
		right[i] = input[i * nChannels + 1] * g;

		//--

		// A. Feed but not using the smoothing!
		//waveformPlot.plotIn[i] = left[i];
		//waveformPlot.plotIn[i] = input[i];
		//waveformPlot.plotIn[i] = input[i * 2];

		//--

		///*

		//TODO:
		// B. Feed but enabling the smoothing!

		//TODO: 
		// Smooth
		// Could be better inside the plot class..
		{
			static int indexIn = 0;
			static int indexOut = 0;

			float tempInput = left[i] * g;
			//float tempInput = input[i * nChannels] * g;

			float tempOutput = waveformPlot.plotIn[indexIn] * g;

			if (waveformPlot.bSmooth)
			{
				// sub gain
				float _gain = ofMap(waveformPlot.smoothGain.get(), waveformPlot.smoothGain.getMin(), waveformPlot.smoothGain.getMax(), 0, AMP_GAIN_MAX_POWER, true);

				tempInput *= _gain;

				/*
					float s1 = ofMap(waveformPlot.smoothVal1.get(), 0, 1, SMOOTH_MIN, SMOOTH_MAX);
					waveformPlot.ofxKuValueSmooth(tempOutput, tempInput, s1);
				*/

				if (waveformPlot.typeSmooth.get() == 1)
				{
					float s1 = ofMap(waveformPlot.smoothVal1.get(), 0, 1, SMOOTH_MIN, SMOOTH_MAX);
					float s2 = ofMap(waveformPlot.smoothVal2.get(), 0, 1, SMOOTH_MIN, SMOOTH_MAX);
					waveformPlot.ofxKuValueSmoothDirected(tempOutput, tempInput, s1, s2);
				}
				else if (waveformPlot.typeSmooth.get() == 0)
				{
					float s1 = ofMap(waveformPlot.smoothVal1.get(), 0, 1, SMOOTH_MIN, SMOOTH_MAX);
					waveformPlot.ofxKuValueSmooth(tempOutput, tempInput, s1);
				}

				waveformPlot.plotIn[indexIn] = tempOutput;
			}
			else
			{
				waveformPlot.plotIn[indexIn] = tempInput;
			}

			//--

			// increase buffer offset
			if (indexIn < (SIZE_BUFFER - 1))
			{
				++indexIn;
			}
			else
			{
				indexIn = 0;
			}

		}
		//*/
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (key == 's')
	{
		soundStream.start();
	}

	if (key == 'e') 
	{
		soundStream.stop();
	}
}