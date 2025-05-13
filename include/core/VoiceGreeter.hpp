#ifndef VOICE_GREETER_HPP
#define VOICE_GREETER_HPP

#include <string>

class VoiceGreeter {
public:
    VoiceGreeter();
    ~VoiceGreeter() = default;

    // Initialize the voice system
    bool initialize();
    
    // Greet a person by name
    void greet(const std::string& name);
    
    // Set voice parameters
    void setVoiceSpeed(int speed);
    void setVoicePitch(int pitch);

private:
    int speed;
    int pitch;
    bool initialized;

    // Generate greeting message
    std::string generateGreeting(const std::string& name);
};

#endif // VOICE_GREETER_HPP 