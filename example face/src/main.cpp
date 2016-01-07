#include "ofMain.h"
#include "ofxCcv.h"

class ofApp : public ofBaseApp {
public:
    ofxCcv ccv;
   // vector<ofxCcv::Classification> results;
     vector<ofRectangle> results;
    ofVideoGrabber cam;
    
    void setup() {
//        ccv.setup("image-net-2012-vgg-d.sqlite3");
       // ccv.setup("image-net-2012.sqlite3");
        ccv.setupFace("face.sqlite3");
        
//        ofPixels img;
//        ofLoadImage(img, "nature.png");
//        ccv.classify(img);
        
        cam.initGrabber(1920/2, 1080/2);
    }
    void update() {
        
        ofSetWindowTitle(ofToString(ofGetFrameRate()));
        cam.update();
        if(cam.isFrameNew()) {
           if(ofGetKeyPressed()) {
              //  results = ccv.classify(cam);
               results = ccv.classifyFace(cam);
               // ofLog()<<"results.size() "<<results.size();
            }
        }
    }
    void draw() {
        cam.draw(0, 0);
        
        /*
        ofPushStyle();
        ofTranslate(5, 5);
        //ofLog()<<"results.size() "<<results.size();
        for(int i = 0; i < results.size(); i++) {
            ofSetColor(ofColor::white);
            ofFill();
            ofDrawRectangle(0, 0, 100, 10);
            ofSetColor(ofColor::black);
            ofDrawRectangle(1, 1, (100-2) * results[i].confidence, 10-2);
            ofSetColor(ofColor::white);
            ofDrawBitmapString(results[i].imageNetName, 105, 10);
            ofTranslate(0, 15);
        }
        ofPopStyle();
         */
        
         ofPushStyle();
        ofNoFill();
        ofSetLineWidth(2);
         for(int i = 0; i < results.size(); i++) {
             ofDrawRectangle(results[i]);
         }
         ofPopStyle();
    }
};

int main() {
    ofSetupOpenGL(1920/2, 1080/2, OF_WINDOW);
    ofRunApp(new ofApp());
    
}