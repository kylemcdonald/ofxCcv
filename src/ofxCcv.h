#pragma once

extern "C" {
#include "ccv.h"
}

ccv_dense_matrix_t toCcv(ofPixels& pix) {
    return ccv_dense_matrix(pix.getHeight(),
                            pix.getWidth(),
                            CCV_8U | CCV_C3,
                            (void*) pix.getPixels(),
                            0);
}

ccv_dense_matrix_t toCcv(ofBaseHasPixels& pix) {
    return toCcv(pix.getPixelsRef());
}


class ofxCcv {
private:
    ccv_convnet_t* convnet;
    vector<string> words;
public:
    class Classification {
    public:
        int imageNetId;
        string imageNetName;
        int rank;
        float confidence;
    };
    
    ofxCcv() {
        ccv_enable_default_cache();
    }
    ~ofxCcv() {
        if(convnet) {
            ccv_convnet_free(convnet);
        }
        ccv_drain_cache();
    }
    void setup(string network) {
        string imagenetFilename = ofToDataPath(network);
        convnet = ccv_convnet_read(0, imagenetFilename.c_str());
        words = ofSplitString(ofBufferFromFile("image-net-2012.words"), "\n");
    }
    template <class T>
    vector<Classification> classify( T& img, int maxResults = 5) {
        vector<Classification> results;
        ccv_dense_matrix_t image;
        image = toCcv(img);
        ccv_dense_matrix_t* input = 0;
        ccv_convnet_input_formation(convnet->input, &image, &input);
        ccv_array_t* rank = 0;
        ccv_convnet_classify(convnet, &input, 1, &rank, maxResults, 1);
        int i;
        for (i = 0; i < rank->rnum; i++) {
            ccv_classification_t* classification = (ccv_classification_t*)ccv_array_get(rank, i);
            Classification result;
            result.imageNetId = classification->id + 1;
            result.imageNetName = words[classification->id];
            result.confidence = classification->confidence;
            result.rank = i;
            results.push_back(result);
        }
        ccv_array_free(rank);
        ccv_matrix_free(input);
        return results;
    }
};