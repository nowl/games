#include "Log.h"

std::map<std::string, boost::shared_ptr<Log> > Log::mLoggers;
int Log::mLevel = Log::INFO;
bool Log::mFileOutput = true;
bool Log::mTerminalOutput = true;
std::string Log::mFileOutputName("td.log");
