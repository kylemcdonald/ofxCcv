#include "ofApp.h"

void ofApp::setup() {
    // set parameters
    string imagePath = "hummingbird-0063.jpg";
    preloadMap = true;  // if you load a new image or need to reanalyze, make this false
    w = 60;
    h = 60;
    stride = 10;
    windowOccluded = true; // if false, it occludes outside the window instead

    // load ccv and image
    ccv.setup("image-net-2012.sqlite3");
    img.load(imagePath);
    fbo.allocate(img.getWidth(), img.getHeight(), GL_RGB);
    
    // get top classification
    results = ccv.classify(img);
    classification = results[0];
    ofLog() << "unoccluded classification: " << classification.imageNetName << endl;

    // slide occlusion window across image and color rank map
    // according to how highly-ranked the top-classification from above appears
    // at each pixel of the occluded image
    
    if (preloadMap) {
        map.load("hummingbird_60_60_10.png");
    }
    else {
        createMap();
    }
}

void ofApp::update() {
    if (!ccv.isLoaded()) return;

    occlude(ofGetMouseX(), ofGetMouseY(), w, h);
    results = ccv.classify(imgOccluded);
}

void ofApp::draw() {
    if (!ccv.isLoaded()) {
        ofSetColor(0);
        ofDrawBitmapString("Network file not found! Check your data folder to make sure it exists.", 20, 20);
        return;
    }
    
    // draw image
    ofSetColor(ofColor::white);
    imgOccluded.draw(0, 0);
    
    // draw occlusion / rank map
    ofPushMatrix();
    ofTranslate(0, imgOccluded.getHeight()+30);
    ofSetColor(ofColor::black);
    ofDrawBitmapString("avg rank of "+classification.imageNetName+" while sliding occlusion window across image", 0, -5);
    ofSetColor(ofColor::white);
    map.draw(0, 0);
    ofPopMatrix();
    
    // draw results
    ofTranslate(img.getWidth() + 5, 5);
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
}

void ofApp::occlude(int x, int y, int w, int h) {
    int iw = fbo.getWidth();
    int ih = fbo.getHeight();
    
    fbo.begin();
    ofPushStyle();
    ofClear(0, 255);
    img.draw(0, 0);
    ofSetColor(0);
    if (windowOccluded) {
        ofDrawRectangle(x, y, w, h);
    }
    else {
        ofDrawRectangle(0, 0, x, ih);
        ofDrawRectangle(x+w, 0, iw-(x+w), ih);
        ofDrawRectangle(0, 0, iw, y);
        ofDrawRectangle(0, y+h, iw, ih-(y+h));
    }
    ofPopStyle();
    fbo.end();
    
    fbo.readToPixels(pixels);
    imgOccluded.setFromPixels(pixels);
}

void ofApp::createMap() {
    int iw = img.getWidth();
    int ih = img.getHeight();
    
    vector<vector<int> > c, t;
    for (int y=0; y<ih; y++) {
        vector<int> c0, t0;
        for (int x=0; x<iw; x++) {
            c0.push_back(0);
            t0.push_back(0);
        }
        c.push_back(c0);
        t.push_back(t0);
    }
    for (int y=0; y<ih; y+=stride) {
        cout << "scanning y="<<y<<endl;
        for (int x=0; x<iw; x+=stride) {
            occlude(x, y, w, h);
            results = ccv.classify(imgOccluded);
            float amt = 0;
            for (int i=0; i<results.size(); i++) {
                if (classification.imageNetId == results[i].imageNetId) {
                    amt = results.size() - i;
                }
            }
            for (int y_=y; y_<min((int) imgOccluded.getHeight(), y+h); y_++) {
                for (int x_=x; x_<min((int) imgOccluded.getWidth(), x+w); x_++) {
                    c[y_][x_] += amt;
                    t[y_][x_] += 1;
                }
            }
        }
    }
    
    // create accuracy map from results
    int max_ = 0;
    int min_ = 1;
    for (int y=0; y<ih; y++) {
        for (int x=0; x<iw; x++) {
            max_ = max(max_, c[y][x]/t[y][x]);
            min_ = min(min_, c[y][x]/t[y][x]);
        }
    }
    map.allocate(iw, ih, OF_IMAGE_COLOR);
    for (int y=0; y<ih; y+=1) {
        for (int x=0; x<iw; x+=1) {
            float value = ofMap(c[y][x] / t[y][x], min_, max_, 0, 255);
            map.setColor(x, y, ofColor(value));
        }
    }
    map.update();
    map.save("hummingbird_60_60_10.png");
}

