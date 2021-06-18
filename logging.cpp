#include <logging.h>

#if !defined(ESP32) && !defined(ESP8266)
#include <stdio.h>
#include <stdlib.h>
#else
#include <Arduino.h>
#include <TimeLib.h>
#endif

int log_printf1(const char* format, ...)
{
    constexpr int loc_buf_size = 64;
    static char loc_buf[loc_buf_size];
    char* temp = loc_buf;
    int len;
    va_list arg;
    va_list copy;
    va_start(arg, format);
    va_copy(copy, arg);
    len = vsnprintf(NULL, 0, format, arg);
    va_end(copy);
    if (len >= loc_buf_size) {
        temp = (char*)malloc(len + 1);
        if (temp == NULL) {
            return 0;
        }
    }
    vsnprintf(temp, len + 1, format, arg);
    printf("%s", temp);
    va_end(arg);
    if (len >= loc_buf_size) {
        free(temp);
    }
    return len;
}

const char* _getFileName(const char* path)
{
    size_t i = 0;
    size_t pos = 0;
    char* p = (char*)path;
    while (*p) {
        i++;
        if (*p == '/' || *p == '\\') {
            pos = i;
        }
        p++;
    }
    return path + pos;
}

#if defined(ESP32) || defined(ESP8266)
/// log to file
int log_f_raw(const char* filePath, const char* format, ...)
{
    uint32_t now = millis();

    File file;

    if (!LOG_FS.exists(filePath)) {
        file = LOG_FS.open(filePath, "w");
    } else {
        file = LOG_FS.open(filePath, "a");
        if (file.size() >= LOG_FS_FILE_SIZE_MAX) {
            log_raw(3, "[LOG]Rotating log files");
            char old_log_path[64];
            sprintf(old_log_path, "/old_%s", &filePath[1]);
            file.close();
            if (LOG_FS.exists(old_log_path))
                LOG_FS.remove(old_log_path);
            LOG_FS.rename(filePath, old_log_path);
            file = LOG_FS.open(filePath, "w");
        }
    }

    // if (TimeLib::timeStatus() == TimeLib::timeNotSet)
    TimeLib::tmElements_t tm;
    TimeLib::breakTime(TimeLib::now(), tm);

    // printf timestamp
    file.printf("%d/%02u/%02u %02u:%02u:%02u.%03d ", tm.Year + 1970, tm.Month, tm.Day, tm.Hour, tm.Minute, tm.Second,
        now % 1000);

    constexpr int loc_buf_size = 64;
    static char loc_buf[loc_buf_size];
    char* temp = loc_buf;
    int len;
    va_list arg;
    va_list copy;
    va_start(arg, format);
    va_copy(copy, arg);
    len = vsnprintf(NULL, 0, format, arg);
    va_end(copy);
    if (len >= loc_buf_size) {
        temp = (char*)malloc(len + 1);
        if (temp == NULL) {
            file.close();
            return 0;
        }
    }
    vsnprintf(temp, len + 1, format, arg);
    file.print(temp); // printf("%s\n", temp);
    va_end(arg);
    if (len >= loc_buf_size) {
        free(temp);
    }
    file.close();
    return len;
    return 0;
}
int log_f_raw2(const char* filePath, const char* format, ...)
{
    // auto now = millis();
    // TimeLib::tmElements_t tm;
    // TimeLib::breakTime(now, tm);

    File f;
    if (!LOG_FS.exists(filePath)) {
        f = LOG_FS.open(filePath, "w");
    } else {
        f = LOG_FS.open(filePath, "a");
        if (f.size() >= LOG_FS_FILE_SIZE_MAX) {
            log_s(3, "[LOG]Rotating log files");
            char old_log_path[64];
            sprintf(old_log_path, "/old_%s", &filePath[1]);
            f.close();
            if (LOG_FS.exists(old_log_path))
                LOG_FS.remove(old_log_path);
            LOG_FS.rename(filePath, old_log_path);
            f = LOG_FS.open(filePath, "w");
        }
    }
    // f.printf("%d/%02u/%02u %02u:%02u:%02u.%03d ", tm.Year + 1970, tm.Month, tm.Day, tm.Hour, tm.Minute, tm.Second,
    //     now % 1000);

    constexpr int loc_buf_size = 64;
    static char loc_buf[loc_buf_size];
    char* temp = loc_buf;
    int len;
    va_list arg;
    va_list copy;
    va_start(arg, format);
    va_copy(copy, arg);
    len = vsnprintf(NULL, 0, format, arg);
    va_end(copy);
    if (len >= loc_buf_size) {
        temp = (char*)malloc(len + 1);
        if (temp == NULL) {
            return 0;
        }
    }
    vsnprintf(temp, len + 1, format, arg);
    f.print(temp); // printf("%s\n", temp);
    va_end(arg);
    if (len >= loc_buf_size) {
        free(temp);
    }
    f.close();
    return 0;
}
bool log_clear(const char* filePath) { return LOG_FS.remove(filePath); }

#else
int log_f_raw(const char* filePath, const char* format, ...) { return 0; }
#endif
