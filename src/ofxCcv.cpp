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
    nLayers = convnet->count;
}

vector<float> ofxCcv::encode(ofPixels & img, int layer) {
    convnet->count = layer; // hack to extract a particular layer with encode
    vector<float> data;
    ofImage imgCopy;
    imgCopy.setFromPixels(img);
    // resize hack. some resolutions cause runtime crash. not sure why.
    imgCopy.resize(300, 300);
    ccv_dense_matrix_t image;
    image = toCcv(imgCopy);
    ccv_dense_matrix_t* input = 0;
    ccv_size_t size = ccv_size(225, 225);
    ccv_convnet_input_formation(size, &image, &input);
    ccv_dense_matrix_t* b = 0;
    ccv_convnet_encode(convnet, &input, &b, 1);
    for (int i = 0; i < b->rows * b->cols; i++) {
        data.push_back(b->data.f32[i]);
    }
    convnet->count = nLayers;   // fix hack
    return data;
}
