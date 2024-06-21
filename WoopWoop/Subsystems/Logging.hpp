#ifndef LOGGING_HPP
#define LOGGING_HPP

#include "Util/Subsystem.hpp"
#include <vector>
#include <string>
#include <map>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <fstream>
#include "ECS/Component.hpp"

// Macros for logging messages with file and line information
#define LOG(...) wpwp::Logging::log(nullptr, wpwp::LogType::Message, __FILE__, __LINE__, __VA_ARGS__)
#define ERROR(...) wpwp::Logging::log(nullptr, wpwp::LogType::Error, __FILE__, __LINE__, __VA_ARGS__)
#define WARN(...) wpwp::Logging::log(nullptr, wpwp::LogType::Warning, __FILE__, __LINE__, __VA_ARGS__)

namespace wpwp
{
    namespace Editor
    {
        class Editor;
    };

    /**
     * @brief Enum defining the type of log message.
     */
    enum LogType
    {
        Message, // Regular message.
        Warning, // Warning message.
        Error    // Error message.
    };

    /**
     * @brief Structure representing a log entry.
     */
    struct Log
    {
        std::string prefix;  // Prefix for the log entry.
        std::string message; // Log message.
        LogType type;        // Type of the log message.

        /**
         * @brief Comparison operator for sorting logs.
         * @param other The log to compare against.
         * @return True if this log is less than the other log.
         */
        bool operator<(const Log &other) const;
    };

    /**
     * @brief Logging subsystem for handling application logs.
     */
    class Logging : public Subsystem
    {
    public:
        /**
         * @brief Initializes the Logging subsystem.
         */
        void init() override;

        /**
         * @brief Updates the Logging subsystem.
         */
        void update() override;

        /**
         * @brief Logs a message with a specified log type.
         * @param logType The type of the log message (Message, Warning, Error).
         * @param file The name of the file where the log is called.
         * @param line The line number where the log is called.
         * @param args Variadic arguments to log.
         */

        template <typename... Args>
        static void log(auto i, LogType logType, const char *file, int line, Args &&...args)
        {
            std::ostringstream os;
            // Combine all arguments into the ostringstream
            ((os << std::forward<Args>(args)), ...);

            const char *logTypeMessage = "";
            switch (logType)
            {
            case LogType::Error:
                logTypeMessage = "[Error]";
                break;
            case LogType::Warning:
                logTypeMessage = "[Warning]";
                break;
            default:
                break;
            }

            std::string time = getCurrentTime();
            std::string prefix = logTypeMessage;
            prefix += " (";
            prefix += time;
            prefix += ") from: ";
            prefix += file;
            prefix += ":";
            prefix += std::to_string(line);

            std::string message = os.str();
#ifdef DEBUG
            std::cout << prefix << " " << message << std::endl;
#endif
            Log newLog{prefix, message, logType};
            s_logs.push_back(newLog);
            s_logMapCount[newLog]++;

            std::ofstream stream;

            stream.open("./logs.log", std::ios_base::app);

            stream << newLog.prefix << " " << newLog.message << std::endl;

            stream.close();
        }

        static std::string getCurrentTime()
        {
            // Get current time as time_point
            auto now = std::chrono::system_clock::now();

            // Convert time_point to time_t
            std::time_t now_c = std::chrono::system_clock::to_time_t(now);

            // Convert to tm struct for local time
            std::tm localTime = *std::localtime(&now_c);

            // Format the time into a string
            std::ostringstream oss;
            oss << std::put_time(&localTime, "%H:%M:%S");

            return oss.str();
        }

        /**
         * @brief Renders the logs using ImGui.
         */
        static void renderLogs();

        /**
         * @brief Clears the logs and their counts.
         */
        static void clear();

        void onStop() override;

    private:
        static std::vector<Log> s_logs;          // Vector storing all log entries.
        static std::map<Log, int> s_logMapCount; // Map storing log entries and their counts.
        friend Editor::Editor;                   // Allows Editor class to access private members.
    };

}; // namespace wpwp

#endif // LOGGING_HPP
