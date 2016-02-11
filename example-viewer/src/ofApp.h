#pragma once

#include "ofxCcv.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    
    ofImage pic;
    ofVideoGrabber grab;

    ofxCcv ccv;
    vector<ofxCcv::FeatureMap> maps;
  
    int layer;
    int inputMode;
    int viewMode;
    int highlighted;
    int scroll;
};