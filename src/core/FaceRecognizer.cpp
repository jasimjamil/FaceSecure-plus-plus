#include "../../include/core/FaceRecognizer.hpp"
#include <opencv2/imgproc.hpp>

FaceRecognizer::FaceRecognizer() : nextLabel(0) {}

bool FaceRecognizer::initialize() {
    model = cv::face::LBPHFaceRecognizer::create();
    return true;
}

bool FaceRecognizer::train(const std::string& name, const std::vector<cv::Mat>& faceImages) {
    std::vector<cv::Mat> processedImages;
    std::vector<int> labels;
    
    for (const auto& image : faceImages) {
        processedImages.push_back(preprocessFace(image));
        labels.push_back(nextLabel);
    }
    
    labelNames[nextLabel] = name;
    nextLabel++;
    
    try {
        model->train(processedImages, labels);
        return true;
    } catch (const cv::Exception& e) {
        return false;
    }
}

std::string FaceRecognizer::recognize(const cv::Mat& faceImage, double& confidence) {
    cv::Mat processed = preprocessFace(faceImage);
    int label = -1;
    
    try {
        model->predict(processed, label, confidence);
        if (label != -1 && confidence < 100.0) {
            return labelNames[label];
        }
    } catch (const cv::Exception& e) {}
    
    return "Unknown";
}

bool FaceRecognizer::saveModel(const std::string& filename) {
    try {
        model->save(filename);
        return true;
    } catch (const cv::Exception& e) {
        return false;
    }
}

bool FaceRecognizer::loadModel(const std::string& filename) {
    try {
        model->read(filename);
        return true;
    } catch (const cv::Exception& e) {
        return false;
    }
}

cv::Mat FaceRecognizer::preprocessFace(const cv::Mat& faceImage) {
    cv::Mat processed;
    cv::cvtColor(faceImage, processed, cv::COLOR_BGR2GRAY);
    cv::resize(processed, processed, cv::Size(100, 100));
    cv::equalizeHist(processed, processed);
    return processed;
} 