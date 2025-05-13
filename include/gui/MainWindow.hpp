#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QTimer>
#include <QStatusBar>
#include <QThread>
#include <QApplication>
#include <QComboBox>
#include <QSlider>
#include <QCheckBox>
#include <QTabWidget>
#include <QDateEdit>
#include <QLineEdit>
#include <QProgressBar>
#include <QGroupBox>
#include <opencv2/opencv.hpp>
#include "../core/FaceDetector.hpp"
#include "../core/FaceRecognizer.hpp"
#include "../core/AttendanceLogger.hpp"
#include "../core/VoiceGreeter.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void startRecognition();
    void stopRecognition();
    void registerNewFace();
    void exportAttendance();
    void clearLog();
    void updateFrame();
    void updateAttendanceTable();
    void searchAttendance();
    void filterByDate();
    void adjustVoiceSettings();
    void changeRecognitionSettings();
    void aboutDialog();

private:
    // GUI Components
    QTabWidget* mainTabWidget;
    
    // Recognition Tab
    QWidget* recognitionTab;
    QLabel* cameraFeed;
    QPushButton* startButton;
    QProgressBar* confidenceBar;
    QLabel* currentPersonLabel;
    QGroupBox* statsGroup;
    QLabel* statsLabel;
    
    // Registration Tab
    QWidget* registrationTab;
    QPushButton* registerButton;
    QLabel* previewLabel;
    QLineEdit* nameInput;
    QPushButton* captureButton;
    QProgressBar* captureProgress;
    
    // Attendance Tab
    QWidget* attendanceTab;
    QTableWidget* attendanceTable;
    QPushButton* exportButton;
    QPushButton* clearButton;
    QLineEdit* searchBox;
    QPushButton* searchButton;
    QDateEdit* dateFilter;
    QPushButton* filterButton;
    
    // Settings Tab
    QWidget* settingsTab;
    QSlider* voiceSpeedSlider;
    QSlider* voicePitchSlider;
    QPushButton* testVoiceButton;
    QComboBox* recognizerTypeCombo;
    QSlider* confidenceThresholdSlider;
    QCheckBox* autoSaveCheckbox;
    QPushButton* saveSettingsButton;
    
    // Status and timer
    QStatusBar* statusBar;
    QTimer* timer;

    // Core Components
    FaceDetector faceDetector;
    FaceRecognizer faceRecognizer;
    AttendanceLogger attendanceLogger;
    VoiceGreeter voiceGreeter;

    // Video capture
    cv::VideoCapture capture;
    bool isCapturing;
    int recognitionCount;
    int totalDetections;

    // Setup functions
    void setupUI();
    void setupRecognitionTab();
    void setupRegistrationTab();
    void setupAttendanceTab();
    void setupSettingsTab();
    void setupConnections();
    void initializeComponents();
    void saveSettings();
    void loadSettings();

    // Helper functions
    void showMessage(const QString& message);
    QImage matToQImage(const cv::Mat& mat);
    void updateStats();
    void playGreeting(const std::string& name);
};

#endif // MAIN_WINDOW_HPP 