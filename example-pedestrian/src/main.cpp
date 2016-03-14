#include "ofMain.h"
#include "ofxCcv.h"

//http://libccv.org/doc/doc-icf/

#define USE_VIDEO

class ofApp : public ofBaseApp {
public:
    ofxCcv ccv;
   // vector<ofxCcv::Classification> results;
     vector<ofRectangle> results;
#ifdef USE_VIDEO
    ofVideoPlayer  vidSource;
#else
    
    ofVideoGrabber vidSource;
#endif
    
    int camWidth, camHeight;
    
    void setup() {
//        ccv.setup("image-net-2012-vgg-d.sqlite3");
       // ccv.setup("image-net-2012.sqlite3");
//        ccv.setupFace("face.sqlite3");
        ccv.setupPedestrians("pedestrian.icf");
        
//        ofPixels img;
//        ofLoadImage(img, "nature.png");
//        ccv.classify(img);
        
        camWidth = 640;
        camHeight = 480;
        
#ifdef USE_VIDEO
        vidSource.load("garage.mov"); //walking2.m4v"); //
        
        //vidSource.setLoopState(OF_LOOP_NORMAL);
        //vidSource.setUseTexture(false);
        vidSource.play();
        
        camHeight = vidSource.getHeight();
        camWidth = vidSource.getWidth();
        ofLogNotice("threadedObject")<<"load video";
#else
        vidSource.setVerbose(false);
        vidSource.setDeviceID(0);
        vidSource.setup(camWidth, camHeight);
#endif
        
    }
    void update() {
        
        ofSetWindowTitle(ofToString(ofGetFrameRate()));
        vidSource.update();
        if(vidSource.isFrameNew()) {
           if(ofGetKeyPressed()) {
              //  results = ccv.classify(cam);
               results = ccv.classifyPedestrians(vidSource);
               // ofLog()<<"results.size() "<<results.size();
            }
        }
    }
    void draw() {
        vidSource.draw(0, 0);
        
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
             ofDrawRectangle(results[i].x,results[i].y,results[i].getWidth(),results[i].getHeight());
             ofDrawBitmapStringHighlight(ofToString(i), results[i].getPosition() + ofPoint(-5,-5));
            //+" : "+ofToString(results[i].z)
         }
         ofPopStyle();
    }
};

int main() {
    ofSetupOpenGL(1920/2, 1080/2, OF_WINDOW);
    ofRunApp(new ofApp());
    
}