#include "../../include/gui/MainWindow.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QHeaderView>
#include <QInputDialog>
#include <QSettings>
#include <QDateTime>
#include <QStyleFactory>
#include <QDesktopWidget>
#include <QScreen>
#include <QFont>

MainWindow::MainWindow(QWidget *parent) 
    : QMainWindow(parent), 
      isCapturing(false), 
      recognitionCount(0),
      totalDetections(0) {
    
    // Set modern style
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    
    // Create color palette
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
    QApplication::setPalette(darkPalette);
    
    // Load custom font
    QFont appFont("Arial", 10);
    QApplication::setFont(appFont);
    
    setupUI();
    setupConnections();
    initializeComponents();
    loadSettings();
}

MainWindow::~MainWindow() {
    if (isCapturing) {
        stopRecognition();
    }
    saveSettings();
}

void MainWindow::setupUI() {
    // Create central widget
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    
    // Create tab widget
    mainTabWidget = new QTabWidget(this);
    mainLayout->addWidget(mainTabWidget);
    
    // Setup individual tabs
    setupRecognitionTab();
    setupRegistrationTab();
    setupAttendanceTab();
    setupSettingsTab();
    
    // Create status bar
    statusBar = new QStatusBar(this);
    setStatusBar(statusBar);
    
    // Set window properties
    setWindowTitle("FaceSecure++ | Advanced Face Recognition System");
    
    // Center window on screen and set appropriate size
    QScreen* screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int width = screenGeometry.width() * 0.8;
    int height = screenGeometry.height() * 0.8;
    resize(width, height);
    move((screenGeometry.width() - width) / 2, (screenGeometry.height() - height) / 2);
    
    setCentralWidget(centralWidget);
}

void MainWindow::setupRecognitionTab() {
    recognitionTab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(recognitionTab);
    
    // Title label
    QLabel* titleLabel = new QLabel("Real-time Face Recognition");
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #2a82da; margin-bottom: 10px;");
    layout->addWidget(titleLabel);
    
    // Main content layout
    QHBoxLayout* contentLayout = new QHBoxLayout();
    
    // Left side - Camera feed
    QVBoxLayout* cameraLayout = new QVBoxLayout();
    cameraFeed = new QLabel();
    cameraFeed->setMinimumSize(640, 480);
    cameraFeed->setAlignment(Qt::AlignCenter);
    cameraFeed->setStyleSheet("background-color: #222; border: 1px solid #444; border-radius: 5px;");
    cameraLayout->addWidget(cameraFeed);
    
    // Start/Stop button
    startButton = new QPushButton("Start Recognition");
    startButton->setMinimumHeight(50);
    startButton->setStyleSheet("background-color: #2a82da; color: white; font-weight: bold; border-radius: 5px;");
    cameraLayout->addWidget(startButton);
    
    contentLayout->addLayout(cameraLayout, 3);
    
    // Right side - Recognition info and stats
    QVBoxLayout* infoLayout = new QVBoxLayout();
    
    // Current person info
    QGroupBox* personGroup = new QGroupBox("Recognition Results");
    QVBoxLayout* personLayout = new QVBoxLayout(personGroup);
    
    currentPersonLabel = new QLabel("No face detected");
    currentPersonLabel->setAlignment(Qt::AlignCenter);
    QFont personFont = currentPersonLabel->font();
    personFont.setPointSize(14);
    currentPersonLabel->setFont(personFont);
    personLayout->addWidget(currentPersonLabel);
    
    QLabel* confidenceLabel = new QLabel("Confidence Level:");
    personLayout->addWidget(confidenceLabel);
    
    confidenceBar = new QProgressBar();
    confidenceBar->setRange(0, 100);
    confidenceBar->setValue(0);
    confidenceBar->setTextVisible(true);
    confidenceBar->setFormat("%v%");
    confidenceBar->setStyleSheet("QProgressBar { border: 1px solid #444; border-radius: 5px; text-align: center; } QProgressBar::chunk { background-color: #2a82da; }");
    personLayout->addWidget(confidenceBar);
    
    infoLayout->addWidget(personGroup);
    
    // Statistics info
    statsGroup = new QGroupBox("Session Statistics");
    QVBoxLayout* statsLayout = new QVBoxLayout(statsGroup);
    
    statsLabel = new QLabel("Recognition started: No\nRecognitions: 0\nTotal detections: 0\nSuccess rate: 0%");
    statsLayout->addWidget(statsLabel);
    
    infoLayout->addWidget(statsGroup);
    infoLayout->addStretch();
    
    contentLayout->addLayout(infoLayout, 1);
    layout->addLayout(contentLayout);
    
    mainTabWidget->addTab(recognitionTab, QIcon::fromTheme("camera-video"), "Recognition");
}

void MainWindow::setupConnections() {
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateFrame);
    
    connect(startButton, &QPushButton::clicked, this, [this]() {
        if (!isCapturing) {
            startRecognition();
        } else {
            stopRecognition();
        }
    });
    
    // Make sure all UI elements are initialized before connecting
    if (registerButton) {
        connect(registerButton, &QPushButton::clicked, this, &MainWindow::registerNewFace);
    }
    
    if (captureButton && captureProgress) {
        connect(captureButton, &QPushButton::clicked, this, [this]() {
            captureProgress->setValue(0);
            registerNewFace();
        });
    }
    
    if (exportButton) {
        connect(exportButton, &QPushButton::clicked, this, &MainWindow::exportAttendance);
    }
    
    if (clearButton) {
        connect(clearButton, &QPushButton::clicked, this, &MainWindow::clearLog);
    }
    
    if (searchButton) {
        connect(searchButton, &QPushButton::clicked, this, &MainWindow::searchAttendance);
    }
    
    if (filterButton) {
        connect(filterButton, &QPushButton::clicked, this, &MainWindow::filterByDate);
    }
    
    if (testVoiceButton) {
        connect(testVoiceButton, &QPushButton::clicked, this, [this]() {
            voiceGreeter.setVoiceSpeed(voiceSpeedSlider->value());
            voiceGreeter.setVoicePitch(voicePitchSlider->value());
            voiceGreeter.greet("Test User");
            showMessage("Voice test completed.");
        });
    }
    
    if (saveSettingsButton) {
        connect(saveSettingsButton, &QPushButton::clicked, this, &MainWindow::saveSettings);
    }
}

void MainWindow::updateStats() {
    QString status = isCapturing ? "Yes" : "No";
    float successRate = totalDetections > 0 ? 
        static_cast<float>(recognitionCount) / totalDetections * 100.0f : 0.0f;
    
    statsLabel->setText(
        QString("Recognition started: %1\nRecognitions: %2\nTotal detections: %3\nSuccess rate: %4%")
        .arg(status)
        .arg(recognitionCount)
        .arg(totalDetections)
        .arg(successRate, 0, 'f', 1)
    );
}

void MainWindow::startRecognition() {
    if (!capture.open(0)) {
        QMessageBox::critical(this, "Error", "Failed to open camera. Please check your camera connection.");
        return;
    }
    
    isCapturing = true;
    startButton->setText("Stop Recognition");
    startButton->setStyleSheet("background-color: #d9534f; color: white; font-weight: bold; border-radius: 5px;");
    timer->start(30); // ~30 FPS
    updateStats();
    showMessage("Recognition started");
}

void MainWindow::stopRecognition() {
    timer->stop();
    capture.release();
    isCapturing = false;
    startButton->setText("Start Recognition");
    startButton->setStyleSheet("background-color: #2a82da; color: white; font-weight: bold; border-radius: 5px;");
    cameraFeed->clear();
    currentPersonLabel->setText("No face detected");
    confidenceBar->setValue(0);
    updateStats();
    showMessage("Recognition stopped");
}

void MainWindow::setupRegistrationTab() {
    registrationTab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(registrationTab);
    
    // Title label
    QLabel* titleLabel = new QLabel("Register New Face");
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #2a82da; margin-bottom: 10px;");
    layout->addWidget(titleLabel);
    
    // Name input
    QHBoxLayout* nameLayout = new QHBoxLayout();
    QLabel* nameLabel = new QLabel("Name:");
    nameInput = new QLineEdit();
    nameInput->setPlaceholderText("Enter person's name");
    nameLayout->addWidget(nameLabel);
    nameLayout->addWidget(nameInput);
    layout->addLayout(nameLayout);
    
    // Camera preview
    previewLabel = new QLabel("Camera preview will be shown here");
    previewLabel->setMinimumSize(400, 300);
    previewLabel->setAlignment(Qt::AlignCenter);
    previewLabel->setStyleSheet("background-color: #222; border: 1px solid #444; border-radius: 5px;");
    layout->addWidget(previewLabel);
    
    // Capture buttons
    captureButton = new QPushButton("Capture Face Images");
    captureButton->setMinimumHeight(40);
    captureButton->setStyleSheet("background-color: #2a82da; color: white; font-weight: bold; border-radius: 5px;");
    layout->addWidget(captureButton);
    
    // Progress bar
    captureProgress = new QProgressBar();
    captureProgress->setRange(0, 5);
    captureProgress->setValue(0);
    captureProgress->setFormat("Capture progress: %v/5");
    captureProgress->setStyleSheet("QProgressBar { border: 1px solid #444; border-radius: 5px; text-align: center; } QProgressBar::chunk { background-color: #2a82da; }");
    layout->addWidget(captureProgress);
    
    // Register button
    registerButton = new QPushButton("Complete Registration");
    registerButton->setMinimumHeight(40);
    registerButton->setStyleSheet("background-color: #28a745; color: white; font-weight: bold; border-radius: 5px;");
    layout->addWidget(registerButton);
    
    // Add some spacing
    layout->addStretch();
    
    mainTabWidget->addTab(registrationTab, QIcon::fromTheme("contact-new"), "Registration");
}

void MainWindow::setupAttendanceTab() {
    attendanceTab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(attendanceTab);
    
    // Title label
    QLabel* titleLabel = new QLabel("Attendance Records");
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #2a82da; margin-bottom: 10px;");
    layout->addWidget(titleLabel);
    
    // Search and filter controls
    QHBoxLayout* controlsLayout = new QHBoxLayout();
    
    // Search controls
    searchBox = new QLineEdit();
    searchBox->setPlaceholderText("Search by name");
    searchButton = new QPushButton("Search");
    searchButton->setStyleSheet("background-color: #2a82da; color: white; font-weight: bold; border-radius: 5px;");
    
    // Date filter
    QLabel* dateLabel = new QLabel("Filter by date:");
    dateFilter = new QDateEdit(QDate::currentDate());
    dateFilter->setCalendarPopup(true);
    filterButton = new QPushButton("Apply Filter");
    filterButton->setStyleSheet("background-color: #2a82da; color: white; font-weight: bold; border-radius: 5px;");
    
    controlsLayout->addWidget(searchBox);
    controlsLayout->addWidget(searchButton);
    controlsLayout->addSpacing(20);
    controlsLayout->addWidget(dateLabel);
    controlsLayout->addWidget(dateFilter);
    controlsLayout->addWidget(filterButton);
    
    layout->addLayout(controlsLayout);
    
    // Attendance table
    attendanceTable = new QTableWidget();
    attendanceTable->setColumnCount(3);
    attendanceTable->setHorizontalHeaderLabels({"Name", "Date", "Time"});
    attendanceTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    attendanceTable->setAlternatingRowColors(true);
    attendanceTable->setStyleSheet("QTableWidget { border: 1px solid #444; border-radius: 5px; alternate-background-color: #333; }");
    
    layout->addWidget(attendanceTable, 1);
    
    // Action buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    exportButton = new QPushButton("Export to CSV");
    exportButton->setMinimumHeight(40);
    exportButton->setStyleSheet("background-color: #28a745; color: white; font-weight: bold; border-radius: 5px;");
    
    clearButton = new QPushButton("Clear Log");
    clearButton->setMinimumHeight(40);
    clearButton->setStyleSheet("background-color: #dc3545; color: white; font-weight: bold; border-radius: 5px;");
    
    buttonLayout->addWidget(exportButton);
    buttonLayout->addWidget(clearButton);
    layout->addLayout(buttonLayout);
    
    mainTabWidget->addTab(attendanceTab, QIcon::fromTheme("x-office-calendar"), "Attendance");
}

void MainWindow::setupSettingsTab() {
    settingsTab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(settingsTab);
    
    // Title label
    QLabel* titleLabel = new QLabel("Application Settings");
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #2a82da; margin-bottom: 10px;");
    layout->addWidget(titleLabel);
    
    // Voice settings
    QGroupBox* voiceGroup = new QGroupBox("Voice Settings");
    QVBoxLayout* voiceLayout = new QVBoxLayout(voiceGroup);
    
    QLabel* speedLabel = new QLabel("Voice Speed:");
    voiceSpeedSlider = new QSlider(Qt::Horizontal);
    voiceSpeedSlider->setRange(80, 220);
    voiceSpeedSlider->setValue(150);
    voiceSpeedSlider->setTickPosition(QSlider::TicksBelow);
    voiceSpeedSlider->setTickInterval(20);
    
    QLabel* pitchLabel = new QLabel("Voice Pitch:");
    voicePitchSlider = new QSlider(Qt::Horizontal);
    voicePitchSlider->setRange(0, 100);
    voicePitchSlider->setValue(50);
    voicePitchSlider->setTickPosition(QSlider::TicksBelow);
    voicePitchSlider->setTickInterval(10);
    
    testVoiceButton = new QPushButton("Test Voice");
    testVoiceButton->setStyleSheet("background-color: #2a82da; color: white; font-weight: bold; border-radius: 5px;");
    
    voiceLayout->addWidget(speedLabel);
    voiceLayout->addWidget(voiceSpeedSlider);
    voiceLayout->addWidget(pitchLabel);
    voiceLayout->addWidget(voicePitchSlider);
    voiceLayout->addWidget(testVoiceButton);
    
    // Recognition settings
    QGroupBox* recognitionGroup = new QGroupBox("Recognition Settings");
    QVBoxLayout* recognitionLayout = new QVBoxLayout(recognitionGroup);
    
    QLabel* typeLabel = new QLabel("Recognizer Type:");
    recognizerTypeCombo = new QComboBox();
    recognizerTypeCombo->addItems({"LBPH", "Eigenfaces", "Fisherfaces"});
    
    QLabel* thresholdLabel = new QLabel("Confidence Threshold:");
    confidenceThresholdSlider = new QSlider(Qt::Horizontal);
    confidenceThresholdSlider->setRange(0, 100);
    confidenceThresholdSlider->setValue(70);
    confidenceThresholdSlider->setTickPosition(QSlider::TicksBelow);
    confidenceThresholdSlider->setTickInterval(10);
    
    autoSaveCheckbox = new QCheckBox("Auto-save attendance logs");
    autoSaveCheckbox->setChecked(true);
    
    recognitionLayout->addWidget(typeLabel);
    recognitionLayout->addWidget(recognizerTypeCombo);
    recognitionLayout->addWidget(thresholdLabel);
    recognitionLayout->addWidget(confidenceThresholdSlider);
    recognitionLayout->addWidget(autoSaveCheckbox);
    
    // Add groups to main layout
    layout->addWidget(voiceGroup);
    layout->addWidget(recognitionGroup);
    
    // Save settings button
    saveSettingsButton = new QPushButton("Save Settings");
    saveSettingsButton->setMinimumHeight(40);
    saveSettingsButton->setStyleSheet("background-color: #28a745; color: white; font-weight: bold; border-radius: 5px;");
    layout->addWidget(saveSettingsButton);
    
    // Add some spacing
    layout->addStretch();
    
    mainTabWidget->addTab(settingsTab, QIcon::fromTheme("preferences-system"), "Settings");
}

void MainWindow::initializeComponents() {
    // Initialize face detector
    if (!faceDetector.initialize()) {
        showMessage("Failed to initialize face detector");
        QMessageBox::critical(this, "Initialization Error", "Failed to initialize face detector. Please check if the cascade file exists.");
        return;
    }
    
    // Initialize face recognizer
    if (!faceRecognizer.initialize()) {
        showMessage("Failed to initialize face recognizer");
        QMessageBox::critical(this, "Initialization Error", "Failed to initialize face recognizer.");
        return;
    }
    
    // Initialize voice greeter
    if (!voiceGreeter.initialize()) {
        showMessage("Failed to initialize voice system");
        QMessageBox::warning(this, "Initialization Warning", "Failed to initialize voice system. Voice greetings may not work.");
    }
    
    // Try to load existing model
    if (faceRecognizer.loadModel()) {
        showMessage("Recognition model loaded successfully");
    }
    
    updateAttendanceTable();
    showMessage("System initialized successfully");
}

void MainWindow::registerNewFace() {
    if (nameInput->text().isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a name for the person to register.");
        mainTabWidget->setCurrentWidget(registrationTab);
        nameInput->setFocus();
        return;
    }
    
    QString name = nameInput->text();
    
    if (!capture.isOpened()) {
        if (!capture.open(0)) {
            QMessageBox::critical(this, "Camera Error", "Failed to open camera. Please check your camera connection.");
            return;
        }
    }
    
    std::vector<cv::Mat> faceImages;
    int imagesNeeded = 5;
    captureProgress->setMaximum(imagesNeeded);
    captureProgress->setValue(0);
    
    while (imagesNeeded > 0) {
        cv::Mat frame;
        capture >> frame;
        if (frame.empty()) continue;
        
        auto faces = faceDetector.detectFaces(frame);
        
        // Show preview
        QImage previewImg = matToQImage(faceDetector.getAnnotatedFrame());
        previewLabel->setPixmap(QPixmap::fromImage(previewImg).scaled(previewLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        
        if (faces.size() == 1) {
            cv::Mat faceROI = frame(faces[0]);
            faceImages.push_back(faceROI.clone());
            imagesNeeded--;
            captureProgress->setValue(5 - imagesNeeded);
            showMessage("Captured face " + QString::number(5 - imagesNeeded) + "/5");
            QThread::msleep(500);
        }
        
        QApplication::processEvents();
    }
    
    if (!isCapturing) {
        capture.release();
    }
    
    if (faceRecognizer.train(name.toStdString(), faceImages)) {
        faceRecognizer.saveModel();
        QMessageBox::information(this, "Registration Successful", 
                                "Successfully registered " + name + ".\nThe system can now recognize this person.");
        nameInput->clear();
        previewLabel->setText("Preview will be shown here");
        mainTabWidget->setCurrentWidget(recognitionTab);
    } else {
        QMessageBox::critical(this, "Registration Failed", 
                             "Failed to register " + name + ". Please try again.");
    }
}

void MainWindow::updateFrame() {
    cv::Mat frame;
    capture >> frame;
    if (frame.empty()) return;
    
    auto faces = faceDetector.detectFaces(frame);
    totalDetections += faces.size();
    
    for (const auto& face : faces) {
        cv::Mat faceROI = frame(face);
        double confidence;
        std::string name = faceRecognizer.recognize(faceROI, confidence);
        
        if (name != "Unknown") {
            confidenceBar->setValue(static_cast<int>(100.0 - confidence));
            currentPersonLabel->setText(QString::fromStdString(name));
            
            cv::putText(frame, name, cv::Point(face.x, face.y - 10),
                cv::FONT_HERSHEY_SIMPLEX, 0.9, cv::Scalar(0, 255, 0), 2);
            
            if (attendanceLogger.logAttendance(name)) {
                voiceGreeter.greet(name);
                updateAttendanceTable();
                recognitionCount++;
            }
        } else {
            confidenceBar->setValue(0);
            currentPersonLabel->setText("Unknown Person");
            
            cv::putText(frame, "Unknown", cv::Point(face.x, face.y - 10),
                cv::FONT_HERSHEY_SIMPLEX, 0.9, cv::Scalar(0, 0, 255), 2);
        }
    }
    
    if (faces.empty()) {
        confidenceBar->setValue(0);
        currentPersonLabel->setText("No face detected");
    }
    
    cameraFeed->setPixmap(QPixmap::fromImage(matToQImage(frame)).scaled(cameraFeed->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    updateStats();
}

void MainWindow::searchAttendance() {
    QString searchTerm = searchBox->text().trimmed();
    if (searchTerm.isEmpty()) {
        updateAttendanceTable();
        return;
    }
    
    auto records = attendanceLogger.getRecords();
    attendanceTable->setRowCount(0);
    
    int row = 0;
    for (size_t i = 0; i < records.size(); ++i) {
        QString name = QString::fromStdString(records[i].name);
        if (name.contains(searchTerm, Qt::CaseInsensitive)) {
            attendanceTable->insertRow(row);
            attendanceTable->setItem(row, 0, new QTableWidgetItem(name));
            attendanceTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(records[i].date)));
            attendanceTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(records[i].time)));
            row++;
        }
    }
    
    if (row == 0) {
        showMessage("No matching records found");
    } else {
        showMessage(QString("Found %1 matching records").arg(row));
    }
}

void MainWindow::filterByDate() {
    QDate selectedDate = dateFilter->date();
    QString dateStr = selectedDate.toString("yyyy-MM-dd");
    
    auto records = attendanceLogger.getRecords();
    attendanceTable->setRowCount(0);
    
    int row = 0;
    for (size_t i = 0; i < records.size(); ++i) {
        QString recordDate = QString::fromStdString(records[i].date);
        if (recordDate == dateStr) {
            attendanceTable->insertRow(row);
            attendanceTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(records[i].name)));
            attendanceTable->setItem(row, 1, new QTableWidgetItem(recordDate));
            attendanceTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(records[i].time)));
            row++;
        }
    }
    
    if (row == 0) {
        showMessage(QString("No attendance records found for %1").arg(dateStr));
    } else {
        showMessage(QString("Found %1 records on %2").arg(row).arg(dateStr));
    }
}

void MainWindow::saveSettings() {
    QSettings settings("FaceSecure", "FaceSecure++");
    
    // Voice settings
    settings.setValue("voice/speed", voiceSpeedSlider->value());
    settings.setValue("voice/pitch", voicePitchSlider->value());
    
    // Recognition settings
    settings.setValue("recognition/type", recognizerTypeCombo->currentIndex());
    settings.setValue("recognition/threshold", confidenceThresholdSlider->value());
    settings.setValue("recognition/autoSave", autoSaveCheckbox->isChecked());
    
    // Apply voice settings
    voiceGreeter.setVoiceSpeed(voiceSpeedSlider->value());
    voiceGreeter.setVoicePitch(voicePitchSlider->value());
    
    showMessage("Settings saved successfully");
}

void MainWindow::loadSettings() {
    QSettings settings("FaceSecure", "FaceSecure++");
    
    // Voice settings
    voiceSpeedSlider->setValue(settings.value("voice/speed", 150).toInt());
    voicePitchSlider->setValue(settings.value("voice/pitch", 50).toInt());
    
    // Recognition settings
    recognizerTypeCombo->setCurrentIndex(settings.value("recognition/type", 0).toInt());
    confidenceThresholdSlider->setValue(settings.value("recognition/threshold", 70).toInt());
    autoSaveCheckbox->setChecked(settings.value("recognition/autoSave", true).toBool());
    
    // Apply voice settings
    voiceGreeter.setVoiceSpeed(voiceSpeedSlider->value());
    voiceGreeter.setVoicePitch(voicePitchSlider->value());
}

void MainWindow::playGreeting(const std::string& name) {
    voiceGreeter.greet(name);
}

void MainWindow::aboutDialog() {
    QMessageBox::about(this, "About FaceSecure++",
        "<h2>FaceSecure++ v1.0</h2>"
        "<p>An advanced face recognition-based attendance system</p>"
        "<p><b>Features:</b></p>"
        "<ul>"
        "<li>Real-time face detection and recognition</li>"
        "<li>Attendance tracking with date and time</li>"
        "<li>Voice greetings for recognized users</li>"
        "<li>User-friendly interface with modern design</li>"
        "</ul>"
        "<p>&copy; 2023 All Rights Reserved</p>");
}

void MainWindow::exportAttendance() {
    QString filename = QFileDialog::getSaveFileName(this,
        "Export Attendance", "", "CSV Files (*.csv)");
    
    if (filename.isEmpty()) {
        return;
    }
    
    if (attendanceLogger.exportToCSV(filename.toStdString())) {
        QMessageBox::information(this, "Export Successful", "Attendance records exported successfully to:\n" + filename);
    } else {
        QMessageBox::critical(this, "Export Failed", "Failed to export attendance records to:\n" + filename);
    }
}

void MainWindow::clearLog() {
    if (QMessageBox::question(this, "Confirm Clear Log",
        "Are you sure you want to clear all attendance records?\nThis action cannot be undone.",
        QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        attendanceLogger.clearLog();
        updateAttendanceTable();
        showMessage("Attendance log cleared");
    }
}

void MainWindow::updateAttendanceTable() {
    if (!attendanceTable) return;
    
    auto records = attendanceLogger.getRecords();
    attendanceTable->setRowCount(records.size());
    
    for (size_t i = 0; i < records.size(); ++i) {
        attendanceTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(records[i].name)));
        attendanceTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(records[i].date)));
        attendanceTable->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(records[i].time)));
    }
}

void MainWindow::showMessage(const QString& message) {
    statusBar->showMessage(message, 3000);
}

QImage MainWindow::matToQImage(const cv::Mat& mat) {
    if (mat.empty()) return QImage();
    
    if (mat.type() == CV_8UC3) {
        cv::Mat rgb;
        cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
        return QImage((uchar*)rgb.data, rgb.cols, rgb.rows,
            rgb.step, QImage::Format_RGB888).copy();
    }
    
    return QImage((uchar*)mat.data, mat.cols, mat.rows,
        mat.step, QImage::Format_Grayscale8).copy();
}

void MainWindow::adjustVoiceSettings() {
    voiceGreeter.setVoiceSpeed(voiceSpeedSlider->value());
    voiceGreeter.setVoicePitch(voicePitchSlider->value());
    showMessage(QString("Voice settings updated: Speed=%1, Pitch=%2")
                .arg(voiceSpeedSlider->value())
                .arg(voicePitchSlider->value()));
}

void MainWindow::changeRecognitionSettings() {
    // This would update recognition parameters based on UI settings
    int threshold = confidenceThresholdSlider->value();
    int recognizerType = recognizerTypeCombo->currentIndex();
    
    // In a real implementation, this would modify the face recognizer settings
    showMessage(QString("Recognition settings updated: Type=%1, Threshold=%2")
                .arg(recognizerTypeCombo->currentText())
                .arg(threshold));
} 