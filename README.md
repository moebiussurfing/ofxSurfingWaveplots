# OVERVIEW
**openFrameworks** add-on to easily plot an audio signal waveform using different customized presets, shapes and styles.  

## Screenshot
![](/example/Capture.PNG)  

## Features
- Scalable and draggable layout.
- Styles editor with presets.
- **Bloom Shader** for plot styling.

## Usage

## Code Example
#### ofApp.h
```.cpp
#include "ofxSurfingUndoHelper.h"
ofParameterGroup params {"Params"};
```

#### ofApp.cpp
```.cpp
void ofApp::setup()
{
}

void ofApp::draw()
{
}

void ofApp::keyPressed(int key)
{
    if (key == 's')
    {
    }
}
```

## Dependencies
* [ofxSurfingHelpers](https://github.com/moebiussurfing/ofxSurfingHelpers)
* [ofxSurfingBox](https://github.com/moebiussurfing/ofxSurfingBox)
* [ofxSurfingImGui](https://github.com/moebiussurfing/ofxSurfingImGui)
* [ofxImGui](https://github.com/Daandelange/ofxImGui/) / Fork
* ofxGui / Optional
* [ofxBloom](https://github.com/P-A-N/ofxBloom) / Optional

## TODO
* Improve plotting performance using ofMesh.
* Add **VU meters** FROM ofxSoundObjects.

## Tested Systems
* **Windows 10** / **VS 2022** / **OF ~0.11**

## AUTHOR
An add-on by **@moebiusSurfing**  
*(ManuMolina). 2022.*

## License
*MIT License*