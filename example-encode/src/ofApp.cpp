#include "ofApp.h"


void ofApp::setup() {
    //ccv.setup("image-net-2012-vgg-d.sqlite3");
    ccv.setup("image-net-2012.sqlite3");

    cat.load("cat.jpg");
    dog.load("dog.jpg");
    car.load("car.jpg");
  
    // encode gives you a 1000-dim vector with the activations of the last
    // layer in the network (the 1000 imageNet output classes).
    // it can be interpreted as a compact representation of an image,
    // which can be used for statistical post-processing
    vector<float> catFeatures = ccv.encode(cat);
    vector<float> dogFeatures = ccv.encode(dog);
    vector<float> carFeatures = ccv.encode(car);
    
    // here we take the correlation of the three images and
    // see that the correlation between the cat and dog pictures
    // is much higher than the other two
    r_cat_dog = correlation(catFeatures, dogFeatures);
    r_cat_car = correlation(catFeatures, carFeatures);
    r_dog_car = correlation(dogFeatures, carFeatures);
}

void ofApp::update() {
}

void ofApp::draw() {
    ofBackground(0);
    
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
