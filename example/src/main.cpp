#include "ofMain.h"
#include "ofxCcv.h"

class ofApp : public ofBaseApp {
public:
    ofxCcv ccv;
    vector<ofxCcv::Classification> results;
    ofVideoGrabber cam;
    
    void setup() {
//        ccv.setup("image-net-2012-vgg-d.sqlite3");
        ccv.setup("image-net-2012.sqlite3");
        
//        ofPixels img;
//        ofLoadImage(img, "nature.png");
//        ccv.classify(img);
        
        cam.initGrabber(640, 480);
    }
    void update() {
        cam.update();
        if(cam.isFrameNew()) {
            if(ofGetKeyPressed()) {
                results = ccv.classify(cam);
            }
        }
    }
    void draw() {
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
};

int main() {
    ofSetupOpenGL(640, 480, OF_WINDOW);
    ofRunApp(new ofApp());
}