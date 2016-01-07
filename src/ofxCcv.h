#pragma once

extern "C" {
#include "ccv.h"
}

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


class ofxCcv {
private:
    ccv_convnet_t* convnet;
    vector<string> words;
    
    ccv_scd_classifier_cascade_t* cascade;
     ccv_dense_matrix_t image;
    
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
        
        if(cascade){
            ccv_scd_classifier_cascade_free(cascade);
           // ccv_matrix_free(&image);
        }
        ccv_drain_cache();
    }
    void setup(string network) {
        string imagenetFilename = ofToDataPath(network);
        convnet = ccv_convnet_read(0, imagenetFilename.c_str());

        ofBuffer buffer = ofBufferFromFile("image-net-2012.words");
        for(auto line : buffer.getLines()) {
            words.push_back(line);
        }
        
    }
    
    void setupFace(string network) {
        string imagenetFilename = ofToDataPath(network);
        cascade = ccv_scd_classifier_cascade_read(imagenetFilename.c_str());
        //convnet = ccv_convnet_read(0, imagenetFilename.c_str());
    }
    
    template <class T>
    vector<Classification> classify(const T& img, int maxResults = 5) {
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
    
    template <class TT>
    vector<ofRectangle> classifyFace(const TT& img) {
    
        vector<ofRectangle> results;
        //http://libccv.org/tutorial/
        
       
        image = toCcv(img);
        
       // ccv_dense_matrix_t* input = &image; //0;
        
        // ccv_read(&image, &input, CCV_IO_RGB_COLOR | CCV_IO_ANY_FILE);
        //ccv_convnet_input_formation(convnet->input, &image, &input);
        
        ccv_array_t* faces = ccv_scd_detect_objects(&image, &cascade, 1, ccv_scd_default_params);
        int i;
        for (i = 0; i < faces->rnum; i++)
        {
            ccv_comp_t* face = (ccv_comp_t*)ccv_array_get(faces, i);
            results.push_back(ofRectangle(face->rect.x, face->rect.y, face->rect.width, face->rect.height));
            printf("%d %d %d %d\n", face->rect.x, face->rect.y, face->rect.width, face->rect.height);
        }
        ccv_array_free(faces);
        //ccv_scd_classifier_cascade_free(cascade);
       // ccv_matrix_free(&image);
        //ccv_matrix_free(input);
    
        return results;
    }

};