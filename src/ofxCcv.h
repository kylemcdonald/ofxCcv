#pragma once

#include "ofMain.h"

extern "C" {
#include "ccv.h"
}

ccv_dense_matrix_t toCcv(const ofPixels& pix);
ccv_dense_matrix_t toCcv(const ofBaseHasPixels& pix);

class ofxCcv {
private:
    ccv_convnet_t* convnet = nullptr;
    vector<string> words;
    int nLayers = 0;
    vector<string> layerNames;
    bool loaded = false;
    
    //needed for face detection
    mutable ccv_scd_classifier_cascade_t* cascade = nullptr;
    
    //needed for pedestrian detection
    mutable ccv_icf_classifier_cascade_t* cascadePedestrians = nullptr;
    
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
    void setup(const std::string& network, const std::string& labels = "image-net-2012.words");
    void setupFace(const std::string& network);
    void setupPedestrians(const std::string& network);
    
    bool isLoaded() const {return loaded;}
    int numLayers() const {return nLayers;}
    const vector<string>& getLayerNames() const {return layerNames;}
    vector<ofxCcv::FeatureMap> getFeatureMaps(int layer) const;
    vector<ofImage> getWeights() const;
    const vector<string>& getClasses() const {return words;}
    
    vector<float> encode(const ofPixels& img, int layer) const;
    vector<float> encode(const ofBaseHasPixels& img, int layer) const {return encode(img.getPixels(), layer);}
    
    template <class T>
    vector<Classification> classify(const T& img, int maxResults = 5) const {
        vector<Classification> results;
        ccv_dense_matrix_t image;
        image = toCcv(img);
        
        ccv_dense_matrix_t* input = nullptr;
        
        ccv_convnet_input_formation(convnet->input, &image, &input);
        
        ccv_array_t* rank = nullptr;

        ccv_convnet_classify(convnet, &input, 1, &rank, maxResults, 1);
        
        for (int i = 0; i < rank->rnum; i++) {
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
    vector<ofRectangle> classifyFace(const TT& img) const {
        
        vector<ofRectangle> results;
    
        ccv_dense_matrix_t image = toCcv(img);
        
        ccv_array_t* faces = ccv_scd_detect_objects(&image, &cascade, 1, ccv_scd_default_params);
        
        for (int i = 0; i < faces->rnum; i++)
        {
            ccv_comp_t* face = (ccv_comp_t*)ccv_array_get(faces, i);
            results.push_back(ofRectangle(face->rect.x, face->rect.y, face->rect.width, face->rect.height));
        }
        
        ccv_array_free(faces);
        return results;
    }
    
    template <class TTT>
    vector<ofRectangle> classifyPedestrians(const TTT& img) const {
        
        vector<ofRectangle> results;

        ccv_dense_matrix_t image = toCcv(img);
        
        ccv_array_t* seq = ccv_icf_detect_objects(&image, &cascadePedestrians, 1, ccv_icf_default_params);
    
        for (int i = 0; i < seq->rnum; i++)
        {
            ccv_comp_t* comp = (ccv_comp_t*)ccv_array_get(seq, i);
            results.push_back(ofRectangle(comp->rect.x, comp->rect.y, comp->rect.width, comp->rect.height));
        }

        ccv_array_free(seq);
        
        return results;
    }
};
