#ifndef __LOGGING_H
#define __LOGGING_H

#define LOG_DEST_ALL

#ifdef _ENALBE_TELNET_LOGGING
#include <TelnetStream.h>
#endif // DEBUG

// #define LOG_DEST_SERIAL
// #define LOG_DEST_FLASH
// #define LOG_DEST_RAM

#ifndef LOG_LEVEL
#define LOG_LEVEL 5 // 0:off, 1:error, 2:warning, 3: info, 4: debug, 5:verbose
#endif

#ifndef LOGFILE_LEVEL
#define LOGFILE_LEVEL 5 // 0:off, 1:error, 2:warning, 3: info, 4: debug, 5:verbose
#endif

const char* _getFileName(const char* path);
#define _LOG_FORMAT(letter, format)                                                                                    \
    "[" #letter "][%s:%u] %s()" format "\n", _getFileName(__FILE__), __LINE__, __FUNCTION__

#define _LOG_FORMAT_SHORT(letter, format) "[" #letter "]" format "\n"

#define _LOG_FORMAT_SHORT_WITHTIME(letter, format) "%010lu [" #letter "]" format "\n", millis()

int log_f_raw(const char* filePath, const char* format, ...);
int log_f_raw2(const char* filePath, const char* format, ...);

#if defined(ESP32)
#include <SPIFFS.h>

#ifndef LOG_FS_FILE_SIZE_MAX
#define LOG_FS_FILE_SIZE_MAX 20000
#endif

#define LOG_FS SPIFFS
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <LittleFS.h>

#ifndef LOG_FS_FILE_SIZE_MAX
#define LOG_FS_FILE_SIZE_MAX 20000
#endif

#define LOG_FS LittleFS
// #define log_printf(format, ...) printf(format, ##__VA_ARGS__)
// #define log_printf(format, ...) printf_P(PSTR(format), ##__VA_ARGS__)
#ifdef _ENALBE_TELNET_LOGGING
#define log_printf(format, ...) printf(format, ##__VA_ARGS__)
#define log_printf2(format, ...) printf(format, ##__VA_ARGS__)
#else
#define log_printf(format, ...)                                                                                        \
    {                                                                                                                  \
        printf(format, ##__VA_ARGS__);                                                                           \
        TelnetStream.printf(format, ##__VA_ARGS__);                                                              \
    }
#define log_printf2(format, ...)                                                                                       \
    {                                                                                                                  \
        printf(format, ##__VA_ARGS__);                                                                                 \
        TelnetStream.printf(format, ##__VA_ARGS__);                                                                    \
    }
#endif
// #define log_printf(format, ...) printf_P(PSTR(format), ##__VA_ARGS__)

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
#define log_printf printf(format, #__VA_ARGS__)
#endif

/// raw log (mem: 0b)
#define log_raw(level, format, ...)                                                                                    \
    if (level <= LOG_LEVEL)                                                                                            \
    log_printf(format, ##__VA_ARGS__)

/// long log
#define log_l(level, format, ...)                                                                                      \
    if (level <= LOG_LEVEL)                                                                                            \
    log_printf2(_LOG_FORMAT(level, format), ##__VA_ARGS__)

/// short log (memory: 16 byte)
// #define log_s log_raw
#define log_s(level, format, ...)                                                                                      \
    if (level <= LOG_LEVEL) {                                                                                          \
        log_printf(_LOG_FORMAT_SHORT(level, format), ##__VA_ARGS__);                                                   \
    }

/// log to file long
#define log_fl(level, filePath, format, ...)                                                                           \
    if (level <= LOGFILE_LEVEL)                                                                                        \
    log_f_raw(filePath, _LOG_FORMAT(level, format), ##__VA_ARGS__)

/// log to file short
#define log_fs(level, filePath, format, ...)                                                                           \
    if (level <= LOGFILE_LEVEL)                                                                                        \
    log_f_raw(filePath, _LOG_FORMAT_SHORT(level, format), ##__VA_ARGS__)

/// short log with time tag
#define log_st(level, format, ...)                                                                                     \
    if (level <= LOG_LEVEL) {                                                                                          \
        log_raw(level, "%012lu ", millis());                                                                           \
        log_printf(_LOG_FORMAT_SHORT(level, format), ##__VA_ARGS__);                                                   \
    }

/// custom log, log to serial, flash or ram
#if defined(LOG_DEST_FLASH)
#define log_c(level, format, ...)                                                                                      \
    if (level <= LOGFILE_LEVEL)                                                                                        \
    log_fs(level, "/log.txt", format, ##__VA_ARGS__)
#elif defined(LOG_DEST_SERIAL)
#define log_c(level, format, ...)                                                                                      \
    if (level <= LOG_LEVEL)                                                                                            \
        log_s(level, format, ##__VA_ARGS__);
#elif defined(LOG_DEST_ALL)
#define log_c(level, format, ...)                                                                                      \
    if (level <= LOG_LEVEL) {                                                                                          \
        log_s(level, format, ##__VA_ARGS__);                                                                           \
        log_fs(level, "/log.txt", format, ##__VA_ARGS__);                                                              \
    }

#endif

#endif //__LOGGING_H