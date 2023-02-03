# OVERVIEW
**openFrameworks** add-on to easily plot an audio signal waveform using different customized presets with different combined shapes and styles.  

## SCREENSHOTS

#### exampleSimple
![](/exampleSimple/Capture.PNG)  

#### An example from ofxSoundDevicesManager. 
#### Using ofxBloom.
![](/exampleScene/Capture.PNG)  

## FEATURES
- Scalable and draggable layout.
- Styles editor with presets.
- Extra gain for scale.
- Smoother to customize smooth, speed and release behavior.
- **OPTIONAL**: WIP **Bloom Shader** for post-fx styling. 
- Easily integrable with my add-on: [ofxSoundDevicesManager](https://github.com/moebiussurfing/ofxSoundDevicesManager).  

<details>
<summary>USAGE</summary>

#### ofApp.h
```.cpp
#include "ofxSurfingImGui.h"
#include "ofxSurfingAudioPlots.h"

ofxSurfingGui ui;
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
    // Look the examples.

    waveformPlot.setup();
    waveformPlot.setUiPtr(&ui); // we will use a parent ui.
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
    // Feed the waveformPlot object with audio samples.
    // Look the examples.
}
```
</details>

## DEPENDENCIES
* [ofxSurfingHelpers](https://github.com/moebiussurfing/ofxSurfingHelpers)
* [ofxSurfingBox](https://github.com/moebiussurfing/ofxSurfingBox)
* [ofxSurfingImGui](https://github.com/moebiussurfing/ofxSurfingImGui)
* [ofxImGui](https://github.com/Daandelange/ofxImGui/) / Fork
<details>
<summary>OPTIONAL</summary>

* ofxGui / oF core
* [ofxBloom](https://github.com/P-A-N/ofxBloom) / WIP
</details>

<details>
<summary>TODO</summary>

* Improve plotting performance using `ofMesh`.
</details>

## TESTED SYSTEMS
* **Windows 10** / **VS 2022** / **OF ~0.11+**

## AUTHOR
An add-on by **@moebiusSurfing**  
*(ManuMolina). 2023.*

## LICENSE
*MIT License*
