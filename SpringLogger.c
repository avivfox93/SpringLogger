//
// Created by Aviv on 17/12/2021.
//

#include "SpringLogger.h"
#include<stdarg.h>
#include "stdio.h"
#include "time.h"

static FILE* Spring_Logger_IO;
static struct SpringLogger_Settings_t Spring_Logger_Settings;

static const char* SpringLogger_Time()
{
    static char buff[64];
    sprintf_s(buff,64,"%d", (int)time(NULL));
    return buff;
}

static int SpringLogger_GetLevel(int level, const char** name, const char** color)
{
    switch(level)
    {
        case SPRING_LOGGER_LOG_LEVEL_VERBOSE:
            *name = "VERBOSE";
            *color = SPRING_LOGGER_LOG_LEVEL_VERBOSE_COLOR;
            break;
        case SPRING_LOGGER_LOG_LEVEL_INFO:
            *name = "INFO";
            *color = SPRING_LOGGER_LOG_LEVEL_INFO_COLOR;
            break;
        case SPRING_LOGGER_LOG_LEVEL_DEBUG:
            *name = "DEBUG";
            *color = SPRING_LOGGER_LOG_LEVEL_DEBUG_COLOR;
            break;
        case SPRING_LOGGER_LOG_LEVEL_WARNING:
            *name = "WARNING";
            *color = SPRING_LOGGER_LOG_LEVEL_WARNING_COLOR;
            break;
        case SPRING_LOGGER_LOG_LEVEL_ERROR:
            *name = "ERROR";
            *color = SPRING_LOGGER_LOG_LEVEL_ERROR_COLOR;
            break;
        default:
            return -1;
    }
    return SPRING_LOGGER_OK;
}

static void SpringLogger_LOG(int level, const char* tag, const char* format, va_list args)
{
    if(level < Spring_Logger_Settings.level) return;
    const char *name,*color;
    if(SpringLogger_GetLevel(level,&name,&color) != SPRING_LOGGER_OK) return;
    if(Spring_Logger_Settings.colors)
    {
        fprintf_s(Spring_Logger_IO,"%s",color);
    }
    fprintf_s(Spring_Logger_IO,"[%s] [%-7s] {%s}: ",Spring_Logger_Settings.time(),name,tag);
    vfprintf_s(Spring_Logger_IO,format,args);
    if(Spring_Logger_Settings.colors)
    {
        fprintf_s(Spring_Logger_IO,"%s\n%s",SPRING_LOGGER_COLOR_RESET,SPRING_LOGGER_COLOR_WHITE);
    }else
    {
        fprintf_s(Spring_Logger_IO,"\n");
    }
    if(Spring_Logger_Settings.flush)
    {
        fflush(Spring_Logger_IO);
    }
}

int SpringLogger_Init(FILE* io, const struct SpringLogger_Settings_t* settings)
{
    Spring_Logger_IO = io;
    if(settings == NULL)
    {
        Spring_Logger_Settings.level = SPRING_LOGGER_LOG_LEVEL;
        Spring_Logger_Settings.colors = SPRING_LOGGER_LOG_COLORS;
    }else
    {
        Spring_Logger_Settings = *settings;
    }
    if(Spring_Logger_Settings.time == NULL)
    {
        Spring_Logger_Settings.time = SpringLogger_Time;
    }
}

void SpringLogger_LOGV(const char* tag, const char* format,...)
{
    va_list args;
    va_start(args, format);
    SpringLogger_LOG(SPRING_LOGGER_LOG_LEVEL_VERBOSE,tag,format,args);
}

void SpringLogger_LOGI(const char* tag, const char* format,...)
{
    va_list args;
    va_start(args, format);
    SpringLogger_LOG(SPRING_LOGGER_LOG_LEVEL_INFO,tag,format,args);
}

void SpringLogger_LOGD(const char* tag, const char* format,...)
{
    va_list args;
    va_start(args, format);
    SpringLogger_LOG(SPRING_LOGGER_LOG_LEVEL_DEBUG,tag,format,args);
}

void SpringLogger_LOGW(const char* tag, const char* format,...)
{
    va_list args;
    va_start(args, format);
    SpringLogger_LOG(SPRING_LOGGER_LOG_LEVEL_WARNING,tag,format,args);
}

void SpringLogger_LOGE(const char* tag, const char* format,...)
{
    va_list args;
    va_start(args, format);
    SpringLogger_LOG(SPRING_LOGGER_LOG_LEVEL_ERROR,tag,format,args);
}

void SpringLogger_LOG_Location(int level, const char* tag, const char* message)
{
    switch(level)
    {
        case SPRING_LOGGER_LOG_LEVEL_DEBUG:
            SpringLogger_LOGD(tag,"%s%s:%d\n%s",__FILE__,__LINE__,message);
            break;
        case SPRING_LOGGER_LOG_LEVEL_ERROR:
            SpringLogger_LOGE(tag,"%s:%d\n%s",__FILE__,__LINE__,message);
            break;
        case SPRING_LOGGER_LOG_LEVEL_INFO:
            SpringLogger_LOGI(tag,"%s:%d\n%s",__FILE__,__LINE__,message);
            break;
        case SPRING_LOGGER_LOG_LEVEL_VERBOSE:
            SpringLogger_LOGV(tag,"%s:%d\n%s",__FILE__,__LINE__,message);
            break;
        case SPRING_LOGGER_LOG_LEVEL_WARNING:
            SpringLogger_LOGW(tag,"%s:%d\n%s",__FILE__,__LINE__,message);
            break;
        default:
            break;
    }
}
