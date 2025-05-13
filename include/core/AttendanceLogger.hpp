#ifndef ATTENDANCE_LOGGER_HPP
#define ATTENDANCE_LOGGER_HPP

#include <string>
#include <vector>
#include <map>
#include <chrono>

struct AttendanceRecord {
    std::string name;
    std::string date;
    std::string time;
};

class AttendanceLogger {
public:
    AttendanceLogger(const std::string& logFile = "data/attendance.csv");
    ~AttendanceLogger() = default;

    // Log attendance for a person
    bool logAttendance(const std::string& name);
    
    // Export attendance records to CSV
    bool exportToCSV(const std::string& filename);
    
    // Clear all attendance records
    void clearLog();
    
    // Get all attendance records
    std::vector<AttendanceRecord> getRecords() const;
    
    // Check if person already marked attendance today
    bool isAlreadyMarked(const std::string& name) const;

private:
    std::string logFile;
    std::vector<AttendanceRecord> records;
    std::map<std::string, std::chrono::system_clock::time_point> lastMarked;

    // Load existing records from file
    bool loadRecords();
    
    // Save records to file
    bool saveRecords();
    
    // Get current date/time as string
    std::string getCurrentDate() const;
    std::string getCurrentTime() const;
};

#endif // ATTENDANCE_LOGGER_HPP 