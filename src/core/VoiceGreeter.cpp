#include "../../include/core/VoiceGreeter.hpp"
#include <cstdlib>
#include <sstream>

VoiceGreeter::VoiceGreeter() : speed(150), pitch(50), initialized(false) {}

bool VoiceGreeter::initialize() {
    initialized = true;
    return true;
}

void VoiceGreeter::greet(const std::string& name) {
    if (!initialized) return;
    
    std::string greeting = generateGreeting(name);
    std::stringstream cmd;
    cmd << "espeak -s " << speed << " -p " << pitch << " \"" << greeting << "\" &";
    std::system(cmd.str().c_str());
}

void VoiceGreeter::setVoiceSpeed(int speed) {
    this->speed = speed;
}

void VoiceGreeter::setVoicePitch(int pitch) {
    this->pitch = pitch;
}

std::string VoiceGreeter::generateGreeting(const std::string& name) {
    return "Welcome, " + name;
} 