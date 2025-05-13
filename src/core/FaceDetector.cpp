#include "../../include/core/FaceDetector.hpp"
#include <opencv2/imgproc.hpp>

FaceDetector::FaceDetector() {}

bool FaceDetector::initialize(const std::string& cascadeFile) {
    return faceClassifier.load(cascadeFile);
}

std::vector<cv::Rect> FaceDetector::detectFaces(const cv::Mat& frame) {
    cv::Mat grayFrame;
    cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(grayFrame, grayFrame);
    
    frame.copyTo(currentFrame);
    faceClassifier.detectMultiScale(grayFrame, currentFaces, 
        1.1, 3, 0, cv::Size(30, 30));
    
    drawFaceRectangles();
    return currentFaces;
}

cv::Mat FaceDetector::getAnnotatedFrame() const {
    return currentFrame;
}

void FaceDetector::drawFaceRectangles() {
    for (const auto& face : currentFaces) {
        cv::rectangle(currentFrame, face, cv::Scalar(0, 255, 0), 2);
    }
} 