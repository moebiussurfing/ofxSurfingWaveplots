# OVERVIEW
**openFrameworks** add-on to easily plot an audio signal waveform using different customized presets with many combined shapes and different styles.  

## SCREENSHOTS

#### exampleSimple
![](/exampleSimple/Capture.PNG)  

#### An example from ofxSoundDevicesManager. (Using ofxBloom)
![](/exampleScene/Capture.PNG)  

## FEATURES
* Scalable and draggable box container.
* Extra gain control for scaling.
* Combine different reactive to the audio elements:  
    * Scope lines. 
    * Thin lines. 
    * Transparent circles.
    * Rounded fat bars.
    * Circled shaped elements.
    * Filled or outlined contour mesh. 
    * A 3d rective object as an alternative example.
* Spreaded or padding more closer between elements.
* All the styles are handled by a Presets Manager.
* Smoother to customize animation speed and release behavior.
* **OPTIONAL**: WIP **Bloom Shader** for shader post-FX styling. 
* **OPTIONAL**: Easily integrable with my add-on: [ofxSoundDevicesManager](https://github.com/moebiussurfing/ofxSoundDevicesManager).  

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
    waveformPlot.setUiPtr(&ui); // We will use a parent ui! Passed as reference.
}
void ofApp::update()
{
    waveformPlot.update();
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
void ofApp::draw()
{
    waveformPlot.drawPlots();
    drawGui();
}
void ofApp::audioIn(ofSoundBuffer& input) 
{
    // Feed the waveformPlot object with the incomming audio samples.
    // Look the examples.
}
```
</details>

## DEPENDENCIES
* [ofxSurfingImGui](https://github.com/moebiussurfing/ofxSurfingImGui)
* [ofxImGui](https://github.com/Daandelange/ofxImGui/) / Fork
* [ofxSurfingHelpers](https://github.com/moebiussurfing/ofxSurfingHelpers)
* [ofxSurfingBox](https://github.com/moebiussurfing/ofxSurfingBox)
<details>
<summary>OPTIONAL</summary>

* ofxGui / oF core (Probably not working _out-of-the-box_ as a replacement to `ImGu`, but could be done with some work.
* [ofxBloom](https://github.com/P-A-N/ofxBloom) / WIP
</details>

<details>
<summary>TODO</summary>

* Improve plotting performance using `ofMesh`.
* Improve spread of elements correctly related to audio samples and buffer size.
</details>

## TESTED SYSTEMS
* **Windows 10** / **VS 2022** / **OF ~0.11+**

## AUTHOR
An add-on by **@moebiusSurfing**  
*(ManuMolina). 2022-2023.*

## LICENSE
*MIT License*
