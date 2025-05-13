#!/bin/bash

# Colors for formatted output
GREEN='\033[0;32m'
BLUE='\033[0;34m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${BLUE}"
echo "  ______              _____                         _      _"
echo " |  ____|            / ____|                       | |    | |"
echo " | |__ __ _  ___ ___| (___   ___  ___ _   _ _ __ ___| |_  | |"
echo " |  __/ _\` |/ __/ _ \\___ \\ / _ \\/ __| | | | '__/ _ \\ __| | |"
echo " | | | (_| | (_|  __/____) |  __/ (__| |_| | | |  __/ |_  |_|"
echo " |_|  \\__,_|\\___\\___|_____/ \\___|\\___|\\__,_|_|  \\___|\\__| (_)"
echo -e "${NC}"
echo -e "${GREEN}Advanced Face Recognition Attendance System - Installer${NC}"
echo -e "${YELLOW}Version 1.0${NC}\n"

# Check root permissions
if [ "$EUID" -ne 0 ]; then
  echo -e "${YELLOW}This installer works best with sudo privileges.${NC}"
  echo -e "${YELLOW}You may need to enter your password to install dependencies.${NC}"
fi

# Installation directory
INSTALL_DIR="/opt/facesecure"

echo -e "${BLUE}Installing FaceSecure++ to $INSTALL_DIR...${NC}"

# Install dependencies
echo -e "${BLUE}Installing required dependencies...${NC}"
sudo apt-get update
sudo apt-get install -y \
  build-essential \
  cmake \
  libopencv-dev \
  qtbase5-dev \
  qt5-qmake \
  espeak \
  libespeak-dev

# Create installation directory
sudo mkdir -p $INSTALL_DIR
sudo mkdir -p $INSTALL_DIR/bin
sudo mkdir -p $INSTALL_DIR/data
sudo mkdir -p $INSTALL_DIR/faces

# Build application
echo -e "${BLUE}Building application...${NC}"
mkdir -p build
cd build
cmake ..
make -j4

# Copy files
echo -e "${BLUE}Installing files...${NC}"
sudo cp FaceSecure++ $INSTALL_DIR/bin/
sudo cp -r ../data/* $INSTALL_DIR/data/
sudo cp ../run.sh $INSTALL_DIR/bin/
sudo chmod +x $INSTALL_DIR/bin/run.sh

# Create desktop entry
echo -e "${BLUE}Creating desktop shortcut...${NC}"
cat > facesecure.desktop << EOL
[Desktop Entry]
Name=FaceSecure++
Comment=Advanced Face Recognition Attendance System
Exec=$INSTALL_DIR/bin/run.sh
Terminal=false
Type=Application
Categories=Utility;
EOL

sudo cp facesecure.desktop /usr/share/applications/

# Create symlink
sudo ln -sf $INSTALL_DIR/bin/run.sh /usr/local/bin/facesecure

echo -e "${GREEN}Installation complete!${NC}"
echo -e "${YELLOW}FaceSecure++ is now installed.${NC}"
echo -e "${YELLOW}You can run it with the 'facesecure' command or find it in your applications menu.${NC}" 