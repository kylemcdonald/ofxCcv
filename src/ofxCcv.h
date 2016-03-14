#pragma once

#include "ofMain.h"

extern "C" {
#include "ccv.h"
}

ccv_dense_matrix_t toCcv(const ofPixels& pix);
ccv_dense_matrix_t toCcv(const ofBaseHasPixels& pix);


class ofxCcv {
private:
    ccv_convnet_t* convnet;
    vector<string> words;
    int nLayers;
    vector<string> layerNames;
    bool loaded;
    
    
    //needed for face detection
    ccv_dense_matrix_t image;
    ccv_scd_classifier_cascade_t* cascade;
    
    //
    ccv_icf_classifier_cascade_t* cascadePedestrians;
public:
    
   
    
    class Classification {
    public:
        int imageNetId;
        string imageNetName;
        int rank;
        float confidence;
    };
    
    class FeatureMap {
    public:
        vector<float> acts;
        int rows;
        int cols;
        float max;
        void getImage(ofImage & img, bool autoBrighten=true);
    };
        
    ofxCcv();
    ~ofxCcv();
    void setup(string network);
    void setupFace(string network);
    void setupPedestrians(string network);
    
    bool isLoaded() {return loaded;}
    
    int numLayers() {return nLayers;}
    vector<string> & getLayerNames() {return layerNames;}
    vector<ofxCcv::FeatureMap> getFeatureMaps(int layer);
    vector<ofImage> getWeights();
    vector<string> & getClasses() {return words;}
    
    vector<float> encode(ofPixels & img, int layer);
    vector<float> encode(ofBaseHasPixels & img, int layer) {return encode(img.getPixels(), layer);}
    
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
    
    template <class TTT>
    vector<ofRectangle> classifyPedestrians(const TTT& img) {
        
        vector<ofRectangle> results;
        //http://libccv.org/tutorial/
        
        //    ccv_read(imagenetFilename.c_str(), &image, CCV_IO_ANY_FILE | CCV_IO_RGB_COLOR);
        image = toCcv(img);
        
        unsigned int elapsed_time = ofGetElapsedTimeMillis(); // get_current_time();
        ccv_array_t* seq = ccv_icf_detect_objects(&image, &cascadePedestrians, 1, ccv_icf_default_params);
        elapsed_time = ofGetElapsedTimeMillis() - elapsed_time;
    
        int i;
        for (i = 0; i < seq->rnum; i++)
        {
            ccv_comp_t* comp = (ccv_comp_t*)ccv_array_get(seq, i);
            results.push_back(ofRectangle(comp->rect.x, comp->rect.y, comp->rect.width, comp->rect.height));
//comp->classification.confidence
            printf("%d %d %d %d %f\n", comp->rect.x, comp->rect.y, comp->rect.width, comp->rect.height, comp->classification.confidence);
        }
        printf("total : %d in time %dms\n", seq->rnum, elapsed_time);
        ccv_array_free(seq);
        //ccv_matrix_free(image);
        //ccv_icf_classifier_cascade_free(cascade);
        
        /*
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
        */
        return results;
    }
};
