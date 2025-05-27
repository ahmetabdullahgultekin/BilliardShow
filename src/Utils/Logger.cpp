/**
 * @file Logger.cpp
 * @brief Implementation of the Logger class for logging messages in the BilliardShow application.
 * This class provides methods to log messages with different severity levels (INFO, WARNING, ERROR),
 * including timestamps and thread safety.
 * @author Ahmet Abdullah Gultekin
 * @date 2025-05-27
 * @version 1.0
 */
#include "Logger.h"

/* * @brief Static mutex for thread safety in logging.
 * This mutex ensures that log messages from different threads do not interleave,
 * maintaining the integrity of the log file and console output.
 */
std::mutex Logger::logMutex;

/**
 * @fn GetTimestamp
 * @brief Generates a timestamp string for log messages.
 * This function retrieves the current time, formats it as a string,
 * and returns it for use in log messages.
 * @return A string representing the current timestamp in "YYYY-MM-DD HH:MM:SS" format.
 */
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

/**
 * @fn GetLogFile
 * @brief Retrieves the log file stream for writing log messages.
 * This function opens the log file in append mode and returns a reference to the stream.
 * @return A reference to the std::ofstream used for logging.
 */
std::ofstream &Logger::GetLogFile() {
    static std::ofstream logFile("billiardshow.log", std::ios::app);
    return logFile;
}

/**
 * @fn Log
 * @brief Logs a message with a specified severity level.
 * This method formats the log message with a timestamp and severity level,
 * and writes it to the log file and console.
 * @param message The message to log.
 * @param level The severity level of the log message (INFO, WARNING, ERROR).
 */
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

/**
 * @fn Info
 * @brief Logs an informational message.
 * This method is a convenience function for logging messages with INFO severity.
 * @param message The informational message to log.
 */
void Logger::Info(const std::string &message) {
    Log(message, INFO);
}

/**
 * @fn Warn
 * @brief Logs a warning message.
 * This method is a convenience function for logging messages with WARNING severity.
 * @param message The warning message to log.
 */
void Logger::Warn(const std::string &message) {
    Log(message, WARNING);
}

/**
 * @fn Error
 * @brief Logs an error message.
 * This method is a convenience function for logging messages with ERROR severity.
 * @param message The error message to log.
 */
void Logger::Error(const std::string &message) {
    Log(message, ERROR);
}

