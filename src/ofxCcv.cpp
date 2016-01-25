#include "ofxCcv.h"

ccv_dense_matrix_t toCcv(const ofPixels& pix) {
    return ccv_dense_matrix(pix.getHeight(),
                            pix.getWidth(),
                            CCV_8U | CCV_C3,
                            (void*) pix.getData(),
                            0);
}

ccv_dense_matrix_t toCcv(const ofBaseHasPixels& pix) {
    return toCcv(pix.getPixels());
}

ofxCcv::ofxCcv() {
    ccv_enable_default_cache();
}
    
ofxCcv::~ofxCcv() {
    if(convnet) {
        ccv_convnet_free(convnet);
    }
    ccv_drain_cache();
}
    
void ofxCcv::setup(string network) {
    string imagenetFilename = ofToDataPath(network);
    convnet = ccv_convnet_read(0, imagenetFilename.c_str());
    ofBuffer buffer = ofBufferFromFile("image-net-2012.words");
    for(auto line : buffer.getLines()) {
        words.push_back(line);
    }
}
