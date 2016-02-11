#include "ofApp.h"

void ofApp::setup() {
    //ccv.setup("image-net-2012-vgg-d.sqlite3");
    ccv.setup("image-net-2012.sqlite3");
               
//    ofPixels img;
//    ofLoadImage(img, "cat.jpg");
//    ccv.classify(img);
    
    cam.initGrabber(640, 480);
}

void ofApp::update() {
    if (!ccv.isLoaded()) return;
    cam.update();
    if(cam.isFrameNew()) {
        if(ofGetKeyPressed()) {
            results = ccv.classify(cam);
        }
    }
}

void ofApp::draw() {
    if (!ccv.isLoaded()) {
        ofSetColor(0);
        ofDrawBitmapString("Network file not found! Check your data folder to make sure it exists.", 20, 20);
        return;
    }
    ofSetColor(255);
    cam.draw(0, 0);
    ofPushStyle();
    ofTranslate(5, 5);
    for(int i = 0; i < results.size(); i++) {
        ofSetColor(ofColor::white);
        ofFill();
        ofDrawRectangle(0, 0, 100, 10);
        ofSetColor(ofColor::black);
        ofDrawRectangle(1, 1, (100-2) * results[i].confidence, 10-2);
        ofSetColor(ofColor::white);
        ofDrawBitmapStringHighlight(results[i].imageNetName, 105, 10);
        ofTranslate(0, 15);
    }
    ofPopStyle();
}
