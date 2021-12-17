//
// Created by Aviv on 17/12/2021.
//

#ifndef SPRINGLOGGER_SPRINGLOGGER_H
#define SPRINGLOGGER_SPRINGLOGGER_H

#include <stdio.h>

#define SPRING_LOGGER_OK                    0

#define SPRING_LOGGER_LOG_LEVEL_VERBOSE     0
#define SPRING_LOGGER_LOG_LEVEL_INFO        1
#define SPRING_LOGGER_LOG_LEVEL_DEBUG       2
#define SPRING_LOGGER_LOG_LEVEL_WARNING     3
#define SPRING_LOGGER_LOG_LEVEL_ERROR       4

#ifndef SPRING_LOGGER_LOG_LEVEL
#define SPRING_LOGGER_LOG_LEVEL SPRING_LOGGER_LOG_LEVEL_VERBOSE
#endif

#ifndef SPRING_LOGGER_LOG_COLORS
#define SPRING_LOGGER_LOG_COLORS 1
#endif

#define SPRING_LOGGER_COLOR_RED     "\x1B[31m"
#define SPRING_LOGGER_COLOR_GREEN   "\x1B[32m"
#define SPRING_LOGGER_COLOR_YELLOW  "\x1B[33m"
#define SPRING_LOGGER_COLOR_BLUE    "\x1B[34m"
#define SPRING_LOGGER_COLOR_MAGENTA "\x1B[35m"
#define SPRING_LOGGER_COLOR_CYAN    "\x1B[36m"
#define SPRING_LOGGER_COLOR_WHITE   "\x1B[37m"
#define SPRING_LOGGER_COLOR_RESET   "\x1B[30m"

#define SPRING_LOGGER_LOG_LEVEL_VERBOSE_COLOR   SPRING_LOGGER_COLOR_GREEN
#define SPRING_LOGGER_LOG_LEVEL_INFO_COLOR      SPRING_LOGGER_COLOR_BLUE
#define SPRING_LOGGER_LOG_LEVEL_DEBUG_COLOR     SPRING_LOGGER_COLOR_MAGENTA
#define SPRING_LOGGER_LOG_LEVEL_WARNING_COLOR   SPRING_LOGGER_COLOR_YELLOW
#define SPRING_LOGGER_LOG_LEVEL_ERROR_COLOR     SPRING_LOGGER_COLOR_RED

/**
 * time - callback function, return current time as string
 * level - min log level to print, lower level wont be printed
 * colors - should use ansi colors
 * flush - should flush after every log
 */
struct SpringLogger_Settings_t{
    const char* (*time)();
    int level;
    int colors;
    int flush;
};

int SpringLogger_Init(FILE* io, const struct SpringLogger_Settings_t* settings);
void SpringLogger_LOGV(const char* tag, const char* format,...);
void SpringLogger_LOGI(const char* tag, const char* format,...);
void SpringLogger_LOGD(const char* tag, const char* format,...);
void SpringLogger_LOGW(const char* tag, const char* format,...);
void SpringLogger_LOGE(const char* tag, const char* format,...);

void SpringLogger_LOG_Location(int level, const char* tag, const char* message);

#endif //SPRINGLOGGER_SPRINGLOGGER_H
