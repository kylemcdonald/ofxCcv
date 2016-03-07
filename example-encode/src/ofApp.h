#pragma once

#include "ofxCcv.h"



class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    
    ofxCcv ccv;
    ofImage img;
    vector<float> classifierEncoding;
    vector<float> featureEncoding;
};