# OVERVIEW
**openFrameworks** add-on to easily plot an audio signal waveform using different customized presets with different combined shapes and styles.  

## SCREENSHOT

#### exampleSimple
![](/exampleSimple/Capture.PNG)  

#### Example from ofxSoundDevicesManager using ofxBloom
![](/exampleScene/Capture.PNG)  

## FEATURES
- Scalable and draggable layout.
- Styles editor with presets.
- Optional: WIP **Bloom Shader** for post fx styling. 
- Easily integrable with my add-on: [ofxSoundDevicesManager](https://github.com/moebiussurfing/ofxSoundDevicesManager).  

<details>
<summary>USAGE</summary>

#### ofApp.h
```.cpp
#include "ofxSurfingImGui.h"
#include "ofxSurfingAudioPlots.h"

void setupGui();
void drawGui();

void audioIn(ofSoundBuffer & input);
WaveformPlot waveformPlot;

```
#### ofApp.cpp
```.cpp
void ofApp::setup()
{
    setupGui();
    // Look example

    waveformPlot.setup();
    waveformPlot.setUiPtr(&ui);
}
void ofApp::update()
{
    waveformPlot.update();
}
void ofApp::draw()
{
    waveformPlot.drawPlots();
}
void ofApp::drawGui()
{
    ui.Begin();
    {
        //..

        waveformPlot.drawImGui(false);
    }
    ui.End();
}
void ofApp::audioIn(ofSoundBuffer& input) 
{
    // Feed the waveformPlot object
    // Look example
}
```
</details>

## DEPENDENCIES
* [ofxSurfingHelpers](https://github.com/moebiussurfing/ofxSurfingHelpers)
* [ofxSurfingBox](https://github.com/moebiussurfing/ofxSurfingBox)
* [ofxSurfingImGui](https://github.com/moebiussurfing/ofxSurfingImGui)
* [ofxImGui](https://github.com/Daandelange/ofxImGui/) / Fork
<details>
<summary>Optional</summary>

* ofxGui / oF core
* [ofxBloom](https://github.com/P-A-N/ofxBloom) / WIP
</details>

<details>
<summary>TODO</summary>

* Improve plotting performance using `ofMesh`.
* Add **VU meters** maybe from ImGui widgets or **ofxSoundObjects**.
</details>

## TESTED SYSTEMS
* **Windows 10** / **VS 2022** / **OF ~0.11+**

## AUTHOR
An add-on by **@moebiusSurfing**  
*(ManuMolina). 2023.*

## LICENSE
*MIT License*