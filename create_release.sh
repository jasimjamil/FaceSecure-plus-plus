#!/bin/bash

# Colors for formatted output
GREEN='\033[0;32m'
BLUE='\033[0;34m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

VERSION="1.0.0"
RELEASE_DIR="releases"

echo -e "${BLUE}"
echo "  ______              _____                         _      _"
echo " |  ____|            / ____|                       | |    | |"
echo " | |__ __ _  ___ ___| (___   ___  ___ _   _ _ __ ___| |_  | |"
echo " |  __/ _\` |/ __/ _ \\___ \\ / _ \\/ __| | | | '__/ _ \\ __| | |"
echo " | | | (_| | (_|  __/____) |  __/ (__| |_| | | |  __/ |_  |_|"
echo " |_|  \\__,_|\\___\\___|_____/ \\___|\\___|\\__,_|_|  \\___|\\__| (_)"
echo -e "${NC}"
echo -e "${GREEN}Creating Release Packages for FaceSecure++${NC}"
echo -e "${YELLOW}Version $VERSION${NC}\n"

# Create release directory
mkdir -p $RELEASE_DIR

# Build the application
echo -e "${BLUE}Building application...${NC}"
mkdir -p build
cd build
cmake ..
make -j4
cd ..

# Create Linux package
echo -e "${BLUE}Creating Linux package...${NC}"
LINUX_PACKAGE="$RELEASE_DIR/FaceSecure-$VERSION-linux.tar.gz"

# Create temporary directory
TEMP_DIR="temp_linux"
mkdir -p $TEMP_DIR
mkdir -p $TEMP_DIR/bin
mkdir -p $TEMP_DIR/data
mkdir -p $TEMP_DIR/faces

# Copy files
cp build/FaceSecure++ $TEMP_DIR/bin/
cp run.sh $TEMP_DIR/bin/
chmod +x $TEMP_DIR/bin/run.sh
cp install.sh $TEMP_DIR/
chmod +x $TEMP_DIR/install.sh
cp data/haarcascade_frontalface_default.xml $TEMP_DIR/data/
cp README.md $TEMP_DIR/

# Create package
tar -czf $LINUX_PACKAGE -C $TEMP_DIR .
rm -rf $TEMP_DIR

echo -e "${GREEN}Linux package created: $LINUX_PACKAGE${NC}"

# Create Windows package (assuming cross-compilation or Windows build is available)
echo -e "${BLUE}Creating Windows package...${NC}"
WINDOWS_PACKAGE="$RELEASE_DIR/FaceSecure-$VERSION-windows.zip"

# Create temporary directory
TEMP_DIR="temp_windows"
mkdir -p $TEMP_DIR
mkdir -p $TEMP_DIR/bin
mkdir -p $TEMP_DIR/data
mkdir -p $TEMP_DIR/faces

# Copy files (note: replace with actual Windows binary if available)
cp build/FaceSecure++ $TEMP_DIR/bin/
cp install_windows.ps1 $TEMP_DIR/
cp data/haarcascade_frontalface_default.xml $TEMP_DIR/data/
cp README.md $TEMP_DIR/

# Create package
zip -r $WINDOWS_PACKAGE $TEMP_DIR/*
rm -rf $TEMP_DIR

echo -e "${GREEN}Windows package created: $WINDOWS_PACKAGE${NC}"
echo -e "${YELLOW}Note: For actual Windows deployment, you need to build on Windows or use cross-compilation.${NC}"

echo -e "\n${GREEN}Release packages created in $RELEASE_DIR directory.${NC}"
echo -e "${YELLOW}Don't forget to test these packages before distributing.${NC}" 