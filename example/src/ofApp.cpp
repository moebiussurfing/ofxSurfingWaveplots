#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetWindowPosition(-1920, 25);

	ofSetVerticalSync(true);
	ofSetCircleResolution(80);
	ofBackground(54, 54, 54);

	boxScene.setMode(ofxSurfingBoxInteractive::BOX_LAYOUT::CENTER_CENTER);
	boxScene.setName("Scene");
	bGui_Scene.makeReferenceTo(boxScene.bGui);
	//boxScene.bGui.makeReferenceTo(bGui_Scene);
	boxScene.setup();
	boxScene.setUseBorder(true);
	//TODO: must fix
	//boxScene.setWidth(1000);
	//boxScene.setHeight(650);
	//boxScene.setLockW(true);
	//boxScene.setLockH(true);

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
	waveformPlot.bGui_Main = true;
	waveformPlot.bGui_Edit = true;

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
void ofApp::update()
{
	//TODO: fix workaround
	static bool bDone = false;
	if (!bDone)
	{
		if (ofGetFrameNum() == 0) {
			bDone = true;
			boxScene.setWidth(1000);
			boxScene.setHeight(650);
			boxScene.setLockW(true);
			boxScene.setLockH(true);
		}
	}

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
	if (bGui_Scene) drawScene();
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

			//int idb = val * 80;
			//ImGui::Fader("##mastervol", ImVec2(20, 80), &idb, 0, 80, "%d", 1.0f); ImGui::ShowTooltipOnHover("Slide.");
			//ImGui::SameLine();
			//static int stack = 0;
			//static int count = 0;
			//ImGui::UvMeter("##vuvr", ImVec2(10, 80), &idb, 0, 80, 20); ImGui::ShowTooltipOnHover("Vertical Uv meters.");
			//ImGui::UvMeter("##huvr", ImVec2(80, 10), &idb, 0, 80, 20, &stack, &count); ImGui::ShowTooltipOnHover("Horizon Uv meters width stack effect.");

			ui.EndWindow();
		}

		//waveformPlot.bGui = true;
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
	//boxScene.drawBorder();
}

//--------------------------------------------------------------
void ofApp::audioIn(ofSoundBuffer& input) {

	float curVol = 0.0;

	// samples are "interleaved"
	int numCounted = 0;

	// lets go through each sample and calculate the root mean square which is a rough way to calculate volume	
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

	// this is how we get the mean of rms :) 
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
//
//namespace ImGui
//{
//	void ImGui::UvMeter(char const* label, ImVec2 const& size, int* value, int v_min, int v_max, int steps, int* stack, int* count)
//	{
//		UvMeter(ImGui::GetWindowDrawList(), label, size, value, v_min, v_max, steps, stack, count);
//	}
//
//	void ImGui::UvMeter(ImDrawList* draw_list, char const* label, ImVec2 const& size, int* value, int v_min, int v_max, int steps, int* stack, int* count)
//	{
//		float fvalue = (float)*value;
//		float* fstack = nullptr;
//		float _f = 0.f;
//		if (stack) { fstack = &_f; *fstack = (float)*stack; }
//		UvMeter(draw_list, label, size, &fvalue, (float)v_min, (float)v_max, steps, fstack, count);
//		*value = (int)fvalue;
//		if (stack) *stack = (int)*fstack;
//	}
//
//	void ImGui::UvMeter(char const* label, ImVec2 const& size, float* value, float v_min, float v_max, int steps, float* stack, int* count)
//	{
//		UvMeter(ImGui::GetWindowDrawList(), label, size, value, v_min, v_max, steps, stack, count);
//	}
//
//	void ImGui::UvMeter(ImDrawList* draw_list, char const* label, ImVec2 const& size, float* value, float v_min, float v_max, int steps, float* stack, int* count)
//	{
//		ImVec2 pos = ImGui::GetCursorScreenPos();
//
//		ImGui::InvisibleButton(label, size);
//		float steps_size = (v_max - v_min) / (float)steps;
//		if (stack && count)
//		{
//			if (*value > *stack)
//			{
//				*stack = *value;
//				*count = 0;
//			}
//			else
//			{
//				*(count) += 1;
//				if (*count > 10)
//				{
//					*stack -= steps_size / 2;
//					if (*stack < v_min) *stack = v_min;
//				}
//			}
//		}
//
//		if (size.y > size.x)
//		{
//			float stepHeight = size.y / (v_max - v_min + 1);
//			auto y = pos.y + size.y;
//			auto hue = 0.4f;
//			auto sat = 0.6f;
//			auto lum = 0.6f;
//			for (float i = v_min; i <= v_max; i += steps_size)
//			{
//				hue = 0.4f - (i / (v_max - v_min)) / 2.0f;
//				sat = (*value < i ? 0.0f : 0.6f);
//				lum = (*value < i ? 0.0f : 0.6f);
//				draw_list->AddRectFilled(ImVec2(pos.x, y), ImVec2(pos.x + size.x, y - (stepHeight * steps_size - 1)), static_cast<ImU32>(ImColor::HSV(hue, sat, lum)));
//				y = pos.y + size.y - (i * stepHeight);
//			}
//			if (stack && count)
//			{
//				draw_list->AddLine(ImVec2(pos.x, pos.y + size.y - (*stack * stepHeight)), ImVec2(pos.x + size.x, pos.y + size.y - (*stack * stepHeight)), IM_COL32_WHITE, 2.f);
//			}
//		}
//		else
//		{
//			float stepWidth = size.x / (v_max - v_min + 1);
//			auto x = pos.x;
//			auto hue = 0.4f;
//			auto sat = 0.6f;
//			auto lum = 0.6f;
//			for (float i = v_min; i <= v_max; i += steps_size)
//			{
//				hue = 0.4f - (i / (v_max - v_min)) / 2.0f;
//				sat = (*value < i ? 0.0f : 0.6f);
//				lum = (*value < i ? 0.0f : 0.6f);
//				draw_list->AddRectFilled(ImVec2(x, pos.y), ImVec2(x + (stepWidth * steps_size - 1), pos.y + size.y), static_cast<ImU32>(ImColor::HSV(hue, sat, lum)));
//				x = pos.x + (i * stepWidth);
//			}
//			if (stack && count)
//			{
//				draw_list->AddLine(ImVec2(pos.x + (*stack * stepWidth), pos.y), ImVec2(pos.x + (*stack * stepWidth), pos.y + size.y), IM_COL32_WHITE, 2.f);
//			}
//		}
//	}
//
//	bool ImGui::Fader(const char* label, const ImVec2& size, int* v, const int v_min, const int v_max, const char* format, float power)
//	{
//		ImGuiDataType data_type = ImGuiDataType_S32;
//		ImGuiWindow* window = GetCurrentWindow();
//		if (window->SkipItems)
//			return false;
//
//		ImGuiContext& g = *GImGui;
//		const ImGuiStyle& style = g.Style;
//		const ImGuiID id = window->GetID(label);
//
//		const ImVec2 label_size = CalcTextSize(label, nullptr, true);
//		const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + size);
//		const ImRect bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));
//
//		ItemSize(bb, style.FramePadding.y);
//		if (!ItemAdd(frame_bb, id))
//			return false;
//
//		IM_ASSERT(data_type >= 0 && data_type < ImGuiDataType_COUNT);
//		if (format == nullptr)
//			format = "%d";
//
//		const bool hovered = ItemHoverable(frame_bb, id);
//		if ((hovered && g.IO.MouseClicked[0]) || g.NavActivateId == id || g.NavActivateInputId == id)
//		{
//			SetActiveID(id, window);
//			SetFocusID(id, window);
//			FocusWindow(window);
//			//        g.ActiveIdAllowNavDirFlags = (1 << ImGuiDir_Left) | (1 << ImGuiDir_Right);
//		}
//
//		// Draw frame
//		const ImU32 frame_col = GetColorU32(g.ActiveId == id ? ImGuiCol_FrameBgActive : g.HoveredId == id ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg);
//		RenderNavHighlight(frame_bb, id);
//		RenderFrame(frame_bb.Min, frame_bb.Max, frame_col, true, g.Style.FrameRounding);
//
//		// Slider behavior
//		ImRect grab_bb;
//		const bool value_changed = SliderBehavior(frame_bb, id, data_type, v, &v_min, &v_max, format, power, ImGuiSliderFlags_Vertical, &grab_bb);
//		if (value_changed)
//			MarkItemEdited(id);
//
//		ImRect gutter;
//		gutter.Min = grab_bb.Min;
//		gutter.Max = ImVec2(frame_bb.Max.x - 2.0f, frame_bb.Max.y - 2.0f);
//		auto w = ((gutter.Max.x - gutter.Min.x) - 4.0f) / 2.0f;
//		gutter.Min.x += w;
//		gutter.Max.x -= w;
//		window->DrawList->AddRectFilled(gutter.Min, gutter.Max, GetColorU32(ImGuiCol_ButtonActive), style.GrabRounding);
//
//		// Render grab
//		window->DrawList->AddRectFilled(grab_bb.Min, grab_bb.Max, GetColorU32(ImGuiCol_Text), style.GrabRounding);
//
//		// Display value using user-provided display format so user can add prefix/suffix/decorations to the value.
//		// For the vertical slider we allow centered text to overlap the frame padding
//		char value_buf[64];
//		snprintf(value_buf, 64, format, int(*v * power)); // modify by dicky reduce warning
//		const char* value_buf_end = value_buf + strlen(value_buf);
//		RenderTextClipped(ImVec2(frame_bb.Min.x, frame_bb.Min.y + style.FramePadding.y), frame_bb.Max, value_buf, value_buf_end, nullptr, ImVec2(0.5f, 0.0f));
//		if (label_size.x > 0.0f)
//			RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);
//
//		return value_changed;
//	}
//}