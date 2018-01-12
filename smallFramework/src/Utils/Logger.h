
#pragma once
#include <string>
#include "sf_common.h"
namespace smallFrameWork
{
    /** Container class for logging messages. 
    *   To enable log messages, make sure _LOG_ENABLED is set to true in FalcorConfig.h.
    *   Messages are printed to a log file in the application directory. Using Logger#ShowBoxOnError() you can control if a message box will be shown as well.
    */
    class Logger
    {
    public:
        /** Log messages severity
        */
        enum class Level
        {
            Info = 0,       ///< Informative messages
            Warning = 1,    ///< Warning messages
            Error = 2,      ///< Error messages. Application might be able to continue running, but incorrectly.

            Disabled = -1
        };

        /** Initialize the logger. Has to be called once before logging is possible. This function will create the log file.
        */
        static void init();

        /** Shutdown the logger and close the log file.
        */
        static void shutdown();

        /** Controls weather or not to show message box on log messages.
            \param[in] showBox true to show a message box, false to disable it.
        */
        static void showBoxOnError(bool showBox) { sShowErrorBox = showBox; }

        /** Returns weather or not the message box is shown on log messages.
            returns true if a message box is shown, false otherwise.
        */
        static bool isBoxShownOnError() { return sShowErrorBox; }

        /** Check if the logger is enabled
        */
        static bool enabled() { return _LOG_ENABLED != 0; }

        /** Set the logger verbosity
        */
        static void setVerbosity(Level level) { sVerbosity = level; }

    private:
        friend void logInfo(const std::string& msg, bool forceMsgBox);
        friend void logWarning(const std::string& msg, bool forceMsgBox);
        friend void logError(const std::string& msg, bool forceMsgBox);
        friend void logErrorAndExit(const std::string& msg, bool forceMsgBox);
        static void log(Level L, const std::string& msg, bool forceMsgBox = false);

        Logger() = delete;
        static bool sShowErrorBox;
        static FILE* sLogFile;
        static bool sInit;
        static Level sVerbosity;
    };

    inline void logInfo(const std::string& msg, bool forceMsgBox = false) { Logger::log(Logger::Level::Info, msg, forceMsgBox); }
    inline void logWarning(const std::string& msg, bool forceMsgBox = false) { Logger::log(Logger::Level::Warning, msg, forceMsgBox); }
    inline void logError(const std::string& msg, bool forceMsgBox = false) { Logger::log(Logger::Level::Error, msg, forceMsgBox); }
    inline void logErrorAndExit(const std::string& msg, bool forceMsgBox = false) { Logger::log(Logger::Level::Error, msg + "\nTerminating...", forceMsgBox); exit(1); }
}
