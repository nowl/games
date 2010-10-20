#ifndef __LOG_H__
#define __LOG_H__

#include <cstdio>
#include <map>
#include <string>
#include <iostream>

#include <boost/shared_ptr.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace pt = boost::posix_time;

class Log {
public:
    enum Level {
        DEBUG,
        INFO,
        WARNING,
        ERROR};

    static void setLevel(int level) { mLevel = level; }
    static Log* getLogger(const std::string& name) {
        if(mLoggers.count(name) == 0) {
            mLoggers[name] = boost::shared_ptr<Log>(new Log(name));
        }
        return mLoggers[name].get();
    }

    void debug(const std::string& msg) {
        if(mLevel <= DEBUG) _outputLog("debug", msg);
    }

    void info(const std::string& msg) {
        if(mLevel <= INFO) _outputLog("info", msg);
    }

    void warn(const std::string& msg) {
        if(mLevel <= WARNING) _outputLog("warning", msg);
    }

    void error(const std::string& msg) {
        if(mLevel <= ERROR) _outputLog("error", msg);
    }

private:
    void _outputLog(const std::string& type, const std::string& msg) {
        std::string logMessage(mLoggerName + ": [" + type + "] " + msg); 
        if(mTerminalOutput)
            std::cout << logMessage << std::endl;
        if(mFileOutput) {
            pt::ptime time = pt::second_clock::local_time();
            logMessage = pt::to_simple_string(time) + " :: " + logMessage;
            FILE *f = fopen(mFileOutputName.c_str(), "a");
            fprintf(f, "%s\n", logMessage.c_str());
            fclose(f);
        }
    }

    static std::map<std::string, boost::shared_ptr<Log> > mLoggers;
    static int mLevel;
    static bool mFileOutput;
    static bool mTerminalOutput;
    static std::string mFileOutputName;
    std::string mLoggerName;

    Log(const std::string& name) : mLoggerName(name) {}
    
    Log(const Log&);
    Log& operator=(const Log&);
};

#endif  // __LOG_H__
