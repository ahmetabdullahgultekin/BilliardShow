#include "Logger.h"

std::mutex Logger::logMutex;

std::string Logger::GetTimestamp() {
    using namespace std::chrono;
    auto now = system_clock::now();
    std::time_t now_c = system_clock::to_time_t(now);
    std::tm tm_buf;
#ifdef _WIN32
    localtime_s(&tm_buf, &now_c);
#else
    localtime_r(&now_c, &tm_buf);
#endif
    char buffer[32];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tm_buf);
    return std::string(buffer);
}

std::ofstream &Logger::GetLogFile() {
    static std::ofstream logFile("billiardshow.log", std::ios::app);
    return logFile;
}

void Logger::Log(const std::string &message, Level level) {
    std::lock_guard<std::mutex> lock(logMutex);
    std::string levelStr;
    switch (level) {
        case INFO:
            levelStr = "INFO";
            break;
        case WARNING:
            levelStr = "WARNING";
            break;
        case ERROR:
            levelStr = "ERROR";
            break;
    }
    std::string logMsg = "[" + GetTimestamp() + "] [" + levelStr + "] " + message;
    // Log to file
    GetLogFile() << logMsg << std::endl;
    // Also log to the console
    if (level == ERROR) {
        std::cerr << logMsg << std::endl;
    } else {
        std::cout << logMsg << std::endl;
    }
}

void Logger::Info(const std::string &message) {
    Log(message, INFO);
}

void Logger::Warn(const std::string &message) {
    Log(message, WARNING);
}

void Logger::Error(const std::string &message) {
    Log(message, ERROR);
}

