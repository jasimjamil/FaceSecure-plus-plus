cmake_minimum_required(VERSION 3.10)
project(FaceSecure++)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find required packages
find_package(OpenCV REQUIRED)
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

# Include directories
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add source files
file(GLOB SOURCES 
    "src/*.cpp"
    "src/core/*.cpp"
    "src/gui/*.cpp"
)

# Add header files
file(GLOB HEADERS 
    "include/*.hpp"
    "include/core/*.hpp"
    "include/gui/*.hpp"
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(${PROJECT_NAME} 
    ${OpenCV_LIBS}
    Qt5::Core
    Qt5::Widgets
    -lespeak
)

# Create necessary directories
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/faces)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/data) 