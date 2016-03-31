#include "ofMain.h"
#include "ofxCcv.h"

//http://libccv.org/doc/doc-icf/

class ofApp : public ofBaseApp {
public:
    ofxCcv ccv;
    
    vector<ofRectangle> results;
    ofVideoGrabber cam;
    ofImage imgSource;
    
    void setup() {
        ccv.setupPedestrians("pedestrian.icf");
        cam.setup(640, 480);
    }
    void update() {
        cam.update();
        if(cam.isFrameNew()) {
            results = ccv.classifyPedestrians(cam);
        }
    }
    void draw() {
        cam.draw(0, 0);
        
        ofPushStyle();
        ofNoFill();
        ofSetLineWidth(2);
        for(int i = 0; i < results.size(); i++) {
            ofDrawRectangle(results[i].x,results[i].y,results[i].getWidth(),results[i].getHeight());
            ofDrawBitmapStringHighlight(ofToString(i), results[i].getPosition() + ofPoint(-5,-5));
        }
        ofPopStyle();
    }
};

int main() {
    ofSetupOpenGL(640, 480, OF_WINDOW);
    ofRunApp(new ofApp());
    
}