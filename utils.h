#ifndef _UTILS
#define _UTILS

#include "types.h"


const std::string WHITESPACE = " \n\r\t\f\v";

/// take a screenshot
int screenshot(const char *destination_path, const char *gamename);

/// Initialize the log SYSTEM
void InitLog();

/** \brief
 * Log a message in the standard Logfile
 * \param const char * a_szLog
 * Message to log
 */
void Log(const std::string a_strLog);

const char * IDToString(ID a_id);

std::string ltrim(const std::string& s);
std::string rtrim(const std::string& s);
std::string trim(const std::string& s);

#endif
