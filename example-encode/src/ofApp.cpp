#include "ofApp.h"


void ofApp::setup() {
    //ccv.setup("image-net-2012-vgg-d.sqlite3");
    ccv.setup("image-net-2012.sqlite3");
    if (!ccv.isLoaded()) return;
    
    cat.load("cat.jpg");
    dog.load("dog.jpg");
    car.load("car.jpg");
  
    // encode returns a vector containing the activations of a given
    // layer in the network. So if we run it on the last layer, i.e.
    // ccv.encode(myImage, ccv.numLayers()) we will receive a 1000-dim
    // vector containing the activations of the output neurons (the 1000
    // imageNet classes). better would be to use the second-to-last layer
    // instead, which for this network contains 4096 neurons. using this
    // layer makes a better compact image representation because these
    // neurons correspond to high-level features but are not as closely
    // tied to the specific classes of imageNet
    vector<float> catFeatures = ccv.encode(cat, ccv.numLayers()-1);
    vector<float> dogFeatures = ccv.encode(dog, ccv.numLayers()-1);
    vector<float> carFeatures = ccv.encode(car, ccv.numLayers()-1);

    // we take the correlation between every pair of feature
    // representations and see that the correlation between the
    // cat and dog pictures is higher than the other two.
    // this is probably because cats and dogs share a number of
    // common features
    r_cat_dog = correlation(catFeatures, dogFeatures);
    r_cat_car = correlation(catFeatures, carFeatures);
    r_dog_car = correlation(dogFeatures, carFeatures);
}

void ofApp::update() {
}

void ofApp::draw() {
    if (!ccv.isLoaded()) {
        ofSetColor(0);
        ofDrawBitmapString("Network file not found! Check your data folder to make sure it exists.", 20, 20);
        return;
    }
    
    ofBackground(0);
    
    ofSetColor(255);
    cat.draw(10, 20);
    dog.draw(320, 20);
    car.draw(630, 20);
    
    ofDrawBitmapString("correlation cat, dog = "+ofToString(r_cat_dog), 20, 400);
    ofDrawBitmapString("correlation cat, car = "+ofToString(r_cat_car), 20, 420);
    ofDrawBitmapString("correlation dog, car = "+ofToString(r_dog_car), 20, 440);
}

float correlation(vector<float> & v1, vector<float> & v2) {
    // pearson correlation
    int n = v1.size();
    float s1=0, s12=0, s2=0, s22=0, s1s2=0;
    for (int i=0; i<n; i++) {
        s1 += v1[i];
        s2 += v2[i];
        s12 += pow(v1[i], 2);
        s22 += pow(v2[i], 2);
        s1s2 += (v1[i] * v2[i]);
    }
    float r = (n * s1s2 - s1 * s2) / (sqrt(n * s12 - pow(s1, 2)) * sqrt(n * s22 - pow(s2, 2)));
    return r;
}
