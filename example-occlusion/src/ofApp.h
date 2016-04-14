#pragma once

#include "ofxCcv.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    
    void occlude(int x, int y, int w, int h);
    void createMap();
    
    ofxCcv ccv;
    vector<ofxCcv::Classification> results;
    ofxCcv::Classification classification;
    ofImage img, imgOccluded, map;
    ofFbo fbo;
    ofPixels pixels;
    
    int w, h, stride;
    bool windowOccluded;
    bool preloadMap;
};