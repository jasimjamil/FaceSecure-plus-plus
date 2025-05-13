#ifndef FACE_RECOGNIZER_HPP
#define FACE_RECOGNIZER_HPP

#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include <string>
#include <vector>
#include <map>

class FaceRecognizer {
public:
    FaceRecognizer();
    ~FaceRecognizer() = default;

    // Initialize the face recognizer
    bool initialize();
    
    // Train the recognizer with new face images
    bool train(const std::string& name, const std::vector<cv::Mat>& faceImages);
    
    // Recognize a face from the given image
    std::string recognize(const cv::Mat& faceImage, double& confidence);
    
    // Save the trained model
    bool saveModel(const std::string& filename = "data/trained_model.yml");
    
    // Load a trained model
    bool loadModel(const std::string& filename = "data/trained_model.yml");

private:
    cv::Ptr<cv::face::LBPHFaceRecognizer> model;
    std::map<int, std::string> labelNames;
    int nextLabel;

    // Prepare face image for recognition
    cv::Mat preprocessFace(const cv::Mat& faceImage);
};

#endif // FACE_RECOGNIZER_HPP 