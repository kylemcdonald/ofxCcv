#include "ofMain.h"
#include "ofxCcv.h"

class ofApp : public ofBaseApp {
public:
    ofxCcv ccv;
    
    vector<ofRectangle> results;
    ofVideoGrabber cam;
    
    void setup() {
        ccv.setupFace("face.sqlite3");
        cam.initGrabber(640, 480);
    }
    void update() {

        cam.update();
        if(cam.isFrameNew()) {
            if(ofGetKeyPressed()) {
                results = ccv.classifyFace(cam);
            }
        }
    }
    void draw() {
        cam.draw(0, 0);
        
        ofPushStyle();
        ofNoFill();
        ofSetLineWidth(2);
        for(int i = 0; i < results.size(); i++) {
            ofDrawRectangle(results[i]);
            ofDrawBitmapStringHighlight(ofToString(i), results[i].getPosition() + ofPoint(-5,-5));
        }
        ofPopStyle();
        
        ofDrawBitmapStringHighlight("press any key to detect", 10,20);
    }
};

int main() {
    ofSetupOpenGL(640, 480, OF_WINDOW);
    ofRunApp(new ofApp());
    
}