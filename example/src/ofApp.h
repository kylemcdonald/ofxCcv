#pragma once

#include "ofxCcv.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    
    ofxCcv ccv;
    vector<ofxCcv::Classification> results;
    ofVideoGrabber cam;
};