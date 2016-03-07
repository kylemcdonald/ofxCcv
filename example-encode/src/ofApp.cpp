#include "ofApp.h"


void ofApp::setup() {
    //ccv.setup("image-net-2012-vgg-d.sqlite3");
    ccv.setup("image-net-2012.sqlite3");
    if (!ccv.isLoaded()) return;
    
    img.load("dog.jpg");
    img.resize(300, 300);
  
    // encode returns a vector containing the activations of a given
    // layer in the network. So if we run it on the last layer, i.e.
    // ccv.encode(myImage, ccv.numLayers()) we will receive a 1000-dim
    // vector containing the activations of the output neurons (the 1000
    // imageNet classes). better would be to use the second-to-last layer
    // instead, which for this network contains 4096 neurons. using this
    // layer makes a better compact image representation because these
    // neurons correspond to high-level features but are not as closely
    // tied to the specific classes of imageNet

    featureEncoding = ccv.encode(img, ccv.numLayers()-1);
    classifierEncoding = ccv.encode(img, ccv.numLayers());
}

void ofApp::update() {
}

void ofApp::draw() {
    if (!ccv.isLoaded()) {
        ofSetColor(0);
        ofDrawBitmapString("Network file not found! Check your data folder to make sure it exists.", 20, 20);
        return;
    }

    int h = 1000 * 15;
    
    ofBackground(0);
    img.draw(5, 5);

    ofTranslate(0, ofMap(ofGetMouseY(), 0, ofGetHeight(), 0, -h));
    
    ofSetColor(255, 0, 0);
    ofDrawBitmapString("Classifier (last layer) encoding", 400, -10);
    ofDrawBitmapString("fc7 (prev layer) encoding", 800, -10);
    
    vector<string> classNames = ccv.getClasses();
    for (int i=0; i<classifierEncoding.size(); i++) {
        int y = ofMap(i, 0, classifierEncoding.size(), 0, h);
        ofSetColor(255);
        ofDrawRectangle(400, y, 10 * classifierEncoding[i], 12);
        ofSetColor(0, 255, 0);
        ofDrawBitmapString(classNames[i], 10, y + 12);
    }
    
    ofSetColor(255);
    for (int i=0; i<featureEncoding.size(); i++) {
        int y = ofMap(i, 0, featureEncoding.size(), 0, h);
        ofDrawRectangle(800, y, 10 * featureEncoding[i], 4);
    }
}
