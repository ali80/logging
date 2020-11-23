#ifndef __LOGGING_H
#define __LOGGING_H

#ifndef LOG_LEVEL
#define LOG_LEVEL 5 // 0:off, 1:error, 2:warning, 3: info, 4: debug, 5:verbose
#endif

#ifndef LOGFILE_LEVEL
#define LOGFILE_LEVEL 5 // 0:off, 1:error, 2:warning, 3: info, 4: debug, 5:verbose
#endif

const char* _getFileName(const char* path);
#define _LOG_FORMAT(letter, format)                                                                                    \
    "[" #letter "][%s:%u] %s(): " format "\n", _getFileName(__FILE__), __LINE__, __FUNCTION__

#define _LOG_FORMAT_SHORT(letter, format) "[" #letter "]: " format "\n"

#define _LOG_FORMAT_SHORT_WITHTIME(letter, format) "%010u [" #letter "]: " format "\n", millis()

int log_f_raw(const char* filePath, const char* format, ...);

#if defined(ESP32)
#include <SPIFFS.h>

#ifndef LOG_FS_FILE_SIZE_MAX
#define LOG_FS_FILE_SIZE_MAX 20000
#endif

#define LOG_FS SPIFFS
#elif defined(ESP8266)

#else
#include <stdarg.h>
#define write_log(priority, format, args...)                                                                           \
    if (priority & PRIO_LOG) {                                                                                         \
                                                                                                                       \
        printf(format, ##args);                                                                                        \
    }
int log_printf1(const char* format, ...);
#define log_i(format, ...) log_printf1(format, ##__VA_ARGS__)
#define log_e(format, ...) log_printf1(format, ##__VA_ARGS__)
#define log_printf printf
#endif

/// long log
#define log_l(level, format, ...)                                                                                      \
    if (level <= LOG_LEVEL)                                                                                            \
    log_printf(_LOG_FORMAT(level, format), ##__VA_ARGS__)

/// short log
#define log_s(level, format, ...)                                                                                      \
    if (level <= LOG_LEVEL)                                                                                            \
    log_printf(_LOG_FORMAT_SHORT(level, format), ##__VA_ARGS__)

/// log to file
#define log_fl(level, filePath, format, ...)                                                                           \
    if (level <= LOGFILE_LEVEL)                                                                                        \
    log_f_raw(filePath, _LOG_FORMAT(level, format), ##__VA_ARGS__)

/// log to file
#define log_fs(level, filePath, format, ...)                                                                           \
    if (level <= LOGFILE_LEVEL)                                                                                        \
    log_f_raw(filePath, _LOG_FORMAT_SHORT(level, format), ##__VA_ARGS__)

/// short log with time tag
#define log_st(level, format, ...)                                                                                     \
    if (level <= LOG_LEVEL)                                                                                            \
    log_printf(_LOG_FORMAT_SHORT_WITHTIME(level, format), ##__VA_ARGS__)

#endif //__LOGGING_H