#include "../../include/core/AttendanceLogger.hpp"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>

AttendanceLogger::AttendanceLogger(const std::string& logFile) : logFile(logFile) {
    loadRecords();
}

bool AttendanceLogger::logAttendance(const std::string& name) {
    if (isAlreadyMarked(name)) {
        return false;
    }
    
    AttendanceRecord record;
    record.name = name;
    record.date = getCurrentDate();
    record.time = getCurrentTime();
    
    records.push_back(record);
    lastMarked[name] = std::chrono::system_clock::now();
    
    return saveRecords();
}

bool AttendanceLogger::exportToCSV(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    file << "Name,Date,Time\n";
    for (const auto& record : records) {
        file << record.name << "," << record.date << "," << record.time << "\n";
    }
    
    return true;
}

void AttendanceLogger::clearLog() {
    records.clear();
    lastMarked.clear();
    saveRecords();
}

std::vector<AttendanceRecord> AttendanceLogger::getRecords() const {
    return records;
}

bool AttendanceLogger::isAlreadyMarked(const std::string& name) const {
    auto it = lastMarked.find(name);
    if (it == lastMarked.end()) {
        return false;
    }
    
    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::hours>(now - it->second);
    return duration.count() < 24;
}

bool AttendanceLogger::loadRecords() {
    std::ifstream file(logFile);
    if (!file.is_open()) {
        return false;
    }
    
    records.clear();
    std::string line;
    // Skip header
    std::getline(file, line);
    
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        AttendanceRecord record;
        
        std::getline(ss, record.name, ',');
        std::getline(ss, record.date, ',');
        std::getline(ss, record.time, ',');
        
        records.push_back(record);
    }
    
    return true;
}

bool AttendanceLogger::saveRecords() {
    return exportToCSV(logFile);
}

std::string AttendanceLogger::getCurrentDate() const {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d");
    return ss.str();
}

std::string AttendanceLogger::getCurrentTime() const {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%H:%M:%S");
    return ss.str();
} 