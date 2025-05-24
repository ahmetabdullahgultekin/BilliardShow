#ifndef BILLIARDSHOW_LOGGER_H
#define BILLIARDSHOW_LOGGER_H

#include <string>
#include <iostream>
#include <fstream>
#include <mutex>
#include <chrono>
#include <ctime>
#include <iomanip>

class Logger {
public:
    enum Level {
        INFO,
        WARNING,
        ERROR
    };

    static void Log(const std::string& message, Level level = INFO);
    static void Info(const std::string& message);
    static void Warn(const std::string& message);
    static void Error(const std::string& message);
private:
    static std::string GetTimestamp();
    static std::ofstream& GetLogFile();
    static std::mutex logMutex;
};

#endif //BILLIARDSHOW_LOGGER_H
