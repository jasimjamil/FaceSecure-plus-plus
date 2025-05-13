# FaceSecure++

![FaceSecure++ Logo](https://img.shields.io/badge/FaceSecure++-v1.0-blue)
![C++](https://img.shields.io/badge/C++-17-red)
![OpenCV](https://img.shields.io/badge/OpenCV-4.2.0-green)
![Qt](https://img.shields.io/badge/Qt-5.12.8-blue)

An advanced real-time face recognition-based attendance system built using C++, OpenCV, and Qt.

![FaceSecure++ Screenshot](screenshot.png)

## Features

- **Real-time Face Detection & Recognition**: Identifies faces from webcam feed using OpenCV
- **Modern UI**: User-friendly Qt-based interface with dark theme and intuitive controls
- **Multi-Person Recognition**: Can recognize multiple people simultaneously
- **Voice Greetings**: Personalized audio greeting when a face is recognized
- **Customizable Settings**: Adjust voice parameters, recognition confidence thresholds
- **Attendance Management**: Comprehensive attendance tracking with search and filtering
- **Export Functionality**: Export attendance records to CSV files
- **Easy User Registration**: Simple process to register new faces with multiple samples

## System Requirements

- C++17 compatible compiler
- OpenCV 4.2.0 or newer
- Qt 5.12.8 or newer
- espeak (for voice synthesis)
- Webcam

## Installation

### Ubuntu/Debian

```bash
# Clone the repository
git clone https://github.com/jasimjamil/FaceSecure-plus-plus.git
cd FaceSecure-plus-plus

# Install dependencies
sudo apt update
sudo apt install build-essential cmake
sudo apt install qtbase5-dev qt5-qmake
sudo apt install libopencv-dev
sudo apt install espeak libespeak-dev

# Build and run
./run.sh
```

### Windows

1. Install [MSYS2](https://www.msys2.org/) or [MinGW](http://mingw-w64.org/doku.php)
2. Install [CMake](https://cmake.org/download/)
3. Install Qt5 from [Qt's website](https://www.qt.io/download)
4. Install OpenCV from [OpenCV's website](https://opencv.org/releases/)
5. Install espeak from [espeak's website](http://espeak.sourceforge.net/download.html)
6. Configure your paths and build:

```bash
mkdir build && cd build
cmake -G "MinGW Makefiles" ..
mingw32-make
```

## Usage

### Starting the Application

Run the application using the provided script:

```bash
./run.sh
```

### Recognition Tab

1. Click "Start Recognition" to begin face detection and recognition
2. The system will display detected faces with name labels
3. Recognition statistics are shown on the right panel
4. Recognized individuals will be automatically logged in the attendance system

### Registration Tab

1. Enter the person's name in the input field
2. Position their face in front of the camera
3. Click "Capture Face Images" to begin image capture
4. The system will capture 5 images for training
5. Wait for confirmation of successful registration

### Attendance Tab

1. View all attendance records in the table
2. Search for specific individuals using the search box
3. Filter records by date using the date picker
4. Export records to CSV by clicking "Export to CSV"
5. Clear the attendance log if needed (requires confirmation)

### Settings Tab

1. Adjust voice speed and pitch for greetings
2. Change recognition parameters
3. Set confidence thresholds
4. Click "Save Settings" to apply changes

## Project Structure

```
FaceSecure++/
├── src/                  # Source code
│   ├── core/             # Core functionality
│   │   ├── FaceDetector.cpp
│   │   ├── FaceRecognizer.cpp
│   │   ├── AttendanceLogger.cpp
│   │   └── VoiceGreeter.cpp
│   ├── gui/              # Qt GUI implementation
│   │   └── MainWindow.cpp
│   └── main.cpp          # Entry point
├── include/              # Header files
│   ├── core/             # Core headers
│   │   ├── FaceDetector.hpp
│   │   ├── FaceRecognizer.hpp
│   │   ├── AttendanceLogger.hpp
│   │   └── VoiceGreeter.hpp
│   └── gui/              # GUI headers
│       └── MainWindow.hpp
├── faces/                # Stored face images
├── data/                 # Model and attendance data
├── CMakeLists.txt        # Build configuration
├── run.sh                # Run script
└── README.md             # This file
```

## Troubleshooting

### Camera issues

If the system cannot detect your camera:

1. Ensure your webcam is properly connected
2. Check if other applications are using the camera
3. Try restarting the application

### Recognition issues

If faces are not being recognized properly:

1. Ensure good lighting conditions
2. Register the person again with better sample images
3. Adjust confidence threshold in Settings

### Build issues

If you encounter build errors:

1. Ensure all dependencies are correctly installed
2. Check CMake and compiler versions
3. Delete the build directory and try again

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgements

- OpenCV community for the computer vision libraries
- Qt team for the GUI framework
- espeak developers for text-to-speech capabilities
