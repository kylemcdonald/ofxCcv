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
    loaded = false;
}
    
ofxCcv::~ofxCcv() {
    if(convnet) {
        ccv_convnet_free(convnet);
    }
    ccv_drain_cache();
}
    
void ofxCcv::setup(string network) {
    string imagenetFilename = ofToDataPath(network);
    loaded = ofFile::doesFileExist(ofToDataPath(network));
    if (!loaded) {
        ofLog(OF_LOG_ERROR, "Can't find network file "+network);
        return;
    }
    
    convnet = ccv_convnet_read(0, imagenetFilename.c_str());
    ofBuffer buffer = ofBufferFromFile("image-net-2012.words");
    for(auto line : buffer.getLines()) {
        words.push_back(line);
    }
    
    nLayers = convnet->count;
    layerNames.clear();
    int convLayers=0, fcLayers=0, lrnLayers=0, poolLayers=0;
    for (int i=0; i<nLayers-1; i++) {
        int type = convnet->layers[i].type;
        if      (type == CCV_CONVNET_CONVOLUTIONAL) layerNames.push_back("CONV_"+ofToString(convLayers++));
        else if (type == CCV_CONVNET_FULL_CONNECT) layerNames.push_back("FC_"+ofToString(fcLayers++));
        else if (type == CCV_CONVNET_LOCAL_RESPONSE_NORM) layerNames.push_back("LOCAL_RESPONSE_NORM_"+ofToString(lrnLayers++));
        else if (type == CCV_CONVNET_MAX_POOL) layerNames.push_back("MAX_POOL_"+ofToString(poolLayers++));
        else if (type == CCV_CONVNET_AVERAGE_POOL) layerNames.push_back("AVG_POOL_"+ofToString(poolLayers++));
    }
}

void ofxCcv::setupFace(string network) {
    string imagenetFilename = ofToDataPath(network);
    cascade = ccv_scd_classifier_cascade_read(imagenetFilename.c_str());
}

void ofxCcv::setupPedestrians(string network){
    string imagenetFilename = ofToDataPath(network);
    cascadePedestrians = ccv_icf_read_classifier_cascade(imagenetFilename.c_str());
}

void ofxCcv::FeatureMap::getImage(ofImage & img, bool autoBrighten) {
    float mul = autoBrighten ? 255.0 / max : 1.0;
    ofPixels pix;
    pix.allocate(rows, cols, OF_PIXELS_GRAY);
    for (int i=0; i<rows*cols; i++) {
        pix[i] = mul * acts[i];
    }
    img.setFromPixels(pix);
}

vector<ofxCcv::FeatureMap> ofxCcv::getFeatureMaps(int layer) {
    int rows = convnet->acts[layer]->rows;
    int cols = convnet->acts[layer]->cols;
    int channels = CCV_GET_CHANNEL(convnet->acts[layer]->type);
    
    vector<FeatureMap> maps;
    for (int c=0; c<channels; c++) {
        FeatureMap map;
        map.rows = rows;
        map.cols = cols;
        map.max = 0;
        for (int i=0; i<rows*cols; i++) {
            float val = convnet->acts[layer]->data.f32[c + i * channels];
            map.acts.push_back(val);
            if (val > map.max) {
                map.max = val;
            }
        }
        maps.push_back(map);
    }
    return maps;
}

vector<ofImage> ofxCcv::getWeights() {
    int layer = 0;
    int wnum = convnet->layers[layer].wnum;
    int wrows = 7;
    int wcols = 7;
    int wchannels = 3;
    int channels = wnum / (wrows * wcols * wchannels);

    vector<ofImage> weightImgs;
    for (int c=0; c<channels; c++) {
        ofPixels pix;
        pix.allocate(wrows, wcols, OF_PIXELS_RGB);
        for (int i=0; i<wrows*wcols; i++) {
            int ir = wrows * wcols * 3 * c + 3 * i + 0;
            int ig = wrows * wcols * 3 * c + 3 * i + 1;
            int ib = wrows * wcols * 3 * c + 3 * i + 2;
            pix[3*i  ] = 255 * convnet->layers[layer].w[ir];
            pix[3*i+1] = 255 * convnet->layers[layer].w[ig];
            pix[3*i+2] = 255 * convnet->layers[layer].w[ib];
        }
        ofImage img;
        img.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
        img.setFromPixels(pix);
        weightImgs.push_back(img);
    }
    return weightImgs;
}

vector<float> ofxCcv::encode(ofPixels & img, int layer) {
    convnet->count = layer; // hack to extract a particular layer with encode
    vector<float> data;
    ofImage imgCopy;
    imgCopy.setFromPixels(img);
    imgCopy.resize(224, 224); // resize to AlexNet input size (224x224)
    ccv_dense_matrix_t image;
    image = toCcv(imgCopy);
    ccv_dense_matrix_t* input = 0;
    ccv_size_t size = ccv_size(225, 225);   // size of the default net, this may need to be changed
    ccv_convnet_input_formation(size, &image, &input);
    ccv_dense_matrix_t* b = 0;
    ccv_convnet_encode(convnet, &input, &b, 1);
    int numChannels = CCV_GET_CHANNEL(b->type);
    int numElements = b->rows * b->cols * numChannels;
    for (int i = 0; i < numElements; i++) {
        data.push_back(b->data.f32[i]);
    }
    convnet->count = nLayers;   // fix hack
    return data;
}
