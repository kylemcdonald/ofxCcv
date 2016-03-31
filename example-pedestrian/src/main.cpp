#include "ofMain.h"
#include "ofxCcv.h"

//http://libccv.org/doc/doc-icf/



class ofApp : public ofBaseApp {
public:
    ofxCcv ccv;
    
    vector<ofRectangle> results;
    ofVideoPlayer  vidSource;

    
    int camWidth, camHeight;
    ofImage imgSource;
    
    void setup() {
        
        ccv.setupPedestrians("pedestrian.icf");
        camWidth = 640;
        camHeight = 480;
        
        vidSource.load("garage.mov");
        vidSource.play();
        
        camHeight = vidSource.getHeight();
        camWidth = vidSource.getWidth();

        
    }
    void update() {
        
        ofSetWindowTitle(ofToString(ofGetFrameRate()));
        vidSource.update();
        if(vidSource.isFrameNew()) {
            
            imgSource.setFromPixels(vidSource.getPixels());
            if(ofGetKeyPressed()) {
                results = ccv.classifyPedestrians(imgSource);
            }
        }
    }
    void draw() {
        imgSource.draw(0, 0);
        
        ofPushStyle();
        ofNoFill();
        ofSetLineWidth(2);
        for(int i = 0; i < results.size(); i++) {
            ofDrawRectangle(results[i].x,results[i].y,results[i].getWidth(),results[i].getHeight());
            ofDrawBitmapStringHighlight(ofToString(i), results[i].getPosition() + ofPoint(-5,-5));
        }
        ofPopStyle();
        
        ofDrawBitmapStringHighlight("press any key to track", 10,20);
        
    }
};

int main() {
    ofSetupOpenGL(1920/2, 1080/2, OF_WINDOW);
    ofRunApp(new ofApp());
    
}