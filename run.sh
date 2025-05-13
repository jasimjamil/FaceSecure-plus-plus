#!/bin/bash

# Colors for formatted output
GREEN='\033[0;32m'
BLUE='\033[0;34m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# FaceSecure++ ASCII art splash screen
echo -e "${BLUE}"
echo "  ______              _____                         _      _"
echo " |  ____|            / ____|                       | |    | |"
echo " | |__ __ _  ___ ___| (___   ___  ___ _   _ _ __ ___| |_  | |"
echo " |  __/ _\` |/ __/ _ \\___ \\ / _ \\/ __| | | | '__/ _ \\ __| | |"
echo " | | | (_| | (_|  __/____) |  __/ (__| |_| | | |  __/ |_  |_|"
echo " |_|  \\__,_|\\___\\___|_____/ \\___|\\___|\\__,_|_|  \\___|\\__| (_)"
echo -e "${NC}"
echo -e "${GREEN}Advanced Face Recognition Attendance System${NC}"
echo -e "${YELLOW}Version 1.0${NC}\n"

# Check for required dependencies
echo -e "${BLUE}Checking dependencies...${NC}"

# Check for OpenCV
if ! pkg-config --exists opencv4; then
    echo -e "${RED}Error: OpenCV is not installed. Please install it using:${NC}"
    echo -e "sudo apt-get install libopencv-dev"
    exit 1
fi
echo -e "${GREEN}✓ OpenCV found.${NC}"

# Check for Qt5
if ! pkg-config --exists Qt5Core; then
    echo -e "${RED}Error: Qt5 is not installed. Please install it using:${NC}"
    echo -e "sudo apt-get install qtbase5-dev qt5-qmake"
    exit 1
fi
echo -e "${GREEN}✓ Qt5 found.${NC}"

# Check for espeak
if ! command -v espeak &> /dev/null; then
    echo -e "${RED}Error: espeak is not installed. Please install it using:${NC}"
    echo -e "sudo apt-get install espeak libespeak-dev"
    exit 1
fi
echo -e "${GREEN}✓ espeak found.${NC}"

# Check if build directory exists, if not create it
echo -e "${BLUE}Setting up directories...${NC}"
if [ ! -d "build" ]; then
  mkdir -p build
  echo -e "${GREEN}✓ Created build directory.${NC}"
else
  echo -e "${GREEN}✓ Build directory already exists.${NC}"
fi

# Create necessary data directories
mkdir -p build/data build/faces
echo -e "${GREEN}✓ Created data directories.${NC}"

# Copy cascade file if it exists
echo -e "${BLUE}Checking for face detection model...${NC}"
if [ -f "data/haarcascade_frontalface_default.xml" ]; then
  cp data/haarcascade_frontalface_default.xml build/data/
  echo -e "${GREEN}✓ Found and copied face detection model.${NC}"
else
  echo -e "${YELLOW}Warning: Face detection model not found. Downloading...${NC}"
  # Download the cascade file from OpenCV GitHub
  wget -P build/data/ https://raw.githubusercontent.com/opencv/opencv/master/data/haarcascades/haarcascade_frontalface_default.xml
  if [ $? -eq 0 ]; then
    echo -e "${GREEN}✓ Successfully downloaded face detection model.${NC}"
    # Keep a copy in the data directory
    cp build/data/haarcascade_frontalface_default.xml data/
  else
    echo -e "${RED}Failed to download face detection model.${NC}"
    exit 1
  fi
fi

# Go to build directory
cd build

# Check if executable exists, if not build it
echo -e "${BLUE}Checking for executable...${NC}"
if [ ! -f "FaceSecure++" ]; then
  echo -e "${YELLOW}Building FaceSecure++...${NC}"
  cmake ..
  if [ $? -ne 0 ]; then
    echo -e "${RED}CMake configuration failed.${NC}"
    exit 1
  fi
  
  make -j4
  if [ $? -ne 0 ]; then
    echo -e "${RED}Build failed.${NC}"
    exit 1
  fi
  echo -e "${GREEN}✓ Build successful.${NC}"
else
  echo -e "${GREEN}✓ Executable already exists.${NC}"
fi

# Run the application
echo -e "\n${BLUE}Starting FaceSecure++...${NC}"
echo -e "${YELLOW}Press Ctrl+C to exit the application.${NC}\n"
./FaceSecure++