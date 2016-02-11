#include "ofApp.h"


void ofApp::setup() {
    ofSetWindowShape(1280, 800);
    ccv.setup("image-net-2012.sqlite3");
    grab.initGrabber(640, 480);
}

void ofApp::update() {
    if (inputMode == 0 && ccv.isLoaded()) {
        grab.update();
        ccv.encode(grab, ccv.numLayers());
    }
}

void ofApp::draw() {
    ofBackground(100);
    
    if (!ccv.isLoaded()) {
        ofDrawBitmapString("Can't find network file! Check to make sure it's in your data folder.", 20, 20);
        return;
    }
    
    if (inputMode == 0) {   // webcam
        grab.draw(0, 0, 320, 240);
    }
    else {  // image
        pic.draw(0, 0, 320, pic.getHeight() * 320 / pic.getWidth());
    }

    // draw menu
    ofNoFill();
    inputMode == 0 ? ofSetColor(0, 255, 0) : ofSetColor(255);
    ofDrawRectangle(10, 245, 200, 20);
    ofDrawBitmapString("Webcam", 15, 262);
    inputMode == 1 ? ofSetColor(0, 255, 0) : ofSetColor(255);
    ofDrawRectangle(10, 268, 200, 20);
    ofDrawBitmapString("Load picture", 15, 285);
    viewMode == 0 ? ofSetColor(0, 255, 0) : ofSetColor(255);
    ofDrawRectangle(10, 310, 200, 20);
    ofDrawBitmapString("View Feature maps", 15, 327);
    viewMode == 1 ? ofSetColor(0, 255, 0) : ofSetColor(255);
    ofDrawRectangle(10, 333, 200, 20);
    ofDrawBitmapString("View Filters", 15, 350);
    vector<string> layerNames = ccv.getLayerNames();
    for (int i=0; i<layerNames.size(); i++) {
        i == layer ? ofSetColor(0, 255, 0) : ofSetColor(255);
        ofDrawRectangle(10, 375+23*i, 200, 20);
        ofDrawBitmapString(layerNames[i], 15, 392+23*i);
    }
    ofSetColor(255);
    ofDrawBitmapString("drag feature maps to scroll", 5, ofGetHeight()-5);

    
    ofTranslate(325, 5);
    
    // draw feature maps
    if (viewMode == 0) {
        maps = ccv.getFeatureMaps(layer);
        int rows = maps.rows;   // rows in each feature map
        int cols = maps.cols;   // cols in each feature map
        int numMaps = maps.channels;    // num feature maps
        
        ofImage img;
        img.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
        for (int m=0; m<numMaps; m++) {
            ofxCcv::FeatureMap map = maps.maps[m];
            map.getImage(img);
            ofPushMatrix();
            ofTranslate((m%8)*105, floor(m/8)*105 - scroll);
            img.draw(0, 0, 100, 100);
            ofPopMatrix();
        }
        
        // draw highlighted
        if (highlighted != -1 && highlighted < numMaps) {
            ofxCcv::FeatureMap map = maps.maps[highlighted];
            map.getImage(img);
            ofPushMatrix();
            ofTranslate(ofClamp((highlighted%8)*105 - 50, 0, ofGetWidth()-220), ofClamp(floor(highlighted/8)*105 - scroll - 50, 20, ofGetHeight()-220));
            ofSetColor(0, 255, 0);
            ofFill();
            ofDrawRectangle(-10, -25, 220, 235);
            ofSetColor(0);
            ofDrawBitmapString(ccv.getLayerNames()[layer]+" - "+ofToString(highlighted), 0, -2);
            ofSetColor(255);
            img.draw(0, 0, 200, 200);
            ofPopMatrix();
        }
    }
    
    // draw weights
    else if (viewMode == 1) {
        ofSetColor(255);
        ofDrawBitmapString("just first layer filters for now", 2, 15);
        vector<ofImage> weightImgs = ccv.getWeights();
        for (int i=0; i<weightImgs.size(); i++) {
            ofPushMatrix();
            ofTranslate((i%12)*65, 24 + floor(i/12)*65);
            weightImgs[i].getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
            weightImgs[i].draw(0, 0, 60, 60);
            ofPopMatrix();
        }
    }
}

void ofApp::keyPressed(int key) {
}

void ofApp::mouseMoved(int x, int y) {
    for (int i=0; i<8; i++) {
        for (int j=0; j<32; j++) {
            if (ofRectangle(325 + 105*i, 5 + 105*j - scroll, 100, 100).inside(x, y)) {
                highlighted = i + 8 * j;
                return;
            }
        }
    }
    highlighted = -1;
}

void ofApp::mouseDragged(int x, int y, int button) {
    scroll = ofClamp(scroll - (ofGetMouseY() - ofGetPreviousMouseY()), 0, 32 * 110);
}

void ofApp::mousePressed(int x, int y, int button) {
    if      (ofRectangle(10, 310, 200, 20).inside(x, y)) viewMode = 0;
    else if (ofRectangle(10, 333, 200, 20).inside(x, y)) viewMode = 1;
    else if (ofRectangle(10, 245, 200, 20).inside(x, y)) inputMode = 0;
    else if (ofRectangle(10, 268, 200, 20).inside(x, y)) {
        ofFileDialogResult result = ofSystemLoadDialog("Select an image");
        if (result.bSuccess) {
            inputMode = 1;
            pic.load(result.filePath);
            ccv.encode(pic, ccv.numLayers());
        }
    }
    else {
        for (int i=0; i<ccv.getLayerNames().size(); i++) {
            if (ofRectangle(10, 375+23*i, 200, 20).inside(x, y)) {
                layer = i;
                return;
            }
        }
    }
}