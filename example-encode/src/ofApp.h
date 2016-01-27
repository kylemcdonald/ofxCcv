#pragma once

#include "ofxCcv.h"


float correlation(vector<float> & v1, vector<float> & v2);

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    
    ofxCcv ccv;
    ofImage cat, dog, car;
    float r_cat_dog, r_cat_car, r_dog_car;
    
    ofVideoGrabber grab;
};