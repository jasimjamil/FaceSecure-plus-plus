#ifndef FACE_DETECTOR_HPP
#define FACE_DETECTOR_HPP

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

class FaceDetector {
public:
    FaceDetector();
    ~FaceDetector() = default;

    // Initialize the face detector with cascade classifier
    bool initialize(const std::string& cascadeFile = "data/haarcascade_frontalface_default.xml");
    
    // Detect faces in the given frame
    std::vector<cv::Rect> detectFaces(const cv::Mat& frame);
    
    // Get the current frame with detected faces drawn
    cv::Mat getAnnotatedFrame() const;

private:
    cv::CascadeClassifier faceClassifier;
    cv::Mat currentFrame;
    std::vector<cv::Rect> currentFaces;
    
    // Draw rectangles around detected faces
    void drawFaceRectangles();
};

#endif // FACE_DETECTOR_HPP 