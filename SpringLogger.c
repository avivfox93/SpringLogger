//
// Created by Aviv on 17/12/2021.
//

#include "SpringLogger.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

static FILE* Spring_Logger_IO;
static struct SpringLogger_Settings_t Spring_Logger_Settings;

static const char* SpringLogger_Time()
{
    static char buff[64];
    sprintf(buff,"%d", (int)time(NULL));
    return buff;
}

static void SpringLogger_fprintf(FILE* io, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    SpringLogger_vfprintf(io,fmt,args);
    va_end(args);
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

void SpringLogger_vLOG(int level, const char* tag, const char* format, va_list args)
{
    if(level < Spring_Logger_Settings.level) return;
    const char *name,*color;
    if(SpringLogger_GetLevel(level,&name,&color) != SPRING_LOGGER_OK) return;
    if(Spring_Logger_Settings.colors)
    {
        SpringLogger_fprintf(Spring_Logger_IO,"%s",color);
    }
    SpringLogger_fprintf(Spring_Logger_IO,"[%s]-[%-7s]-{%s}: ",Spring_Logger_Settings.time(),name,tag);
    vfprintf(Spring_Logger_IO,format,args);
    if(Spring_Logger_Settings.colors)
    {
        SpringLogger_fprintf(Spring_Logger_IO,"%s\n%s",SPRING_LOGGER_COLOR_RESET,SPRING_LOGGER_COLOR_WHITE);
    }else
    {
        SpringLogger_fprintf(Spring_Logger_IO,"\n");
    }
    if(Spring_Logger_Settings.flush)
    {
        SpringLogger_fflush(Spring_Logger_IO);
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
        memcpy(&Spring_Logger_Settings, settings, sizeof(Spring_Logger_Settings));
    }
    if(Spring_Logger_Settings.time == NULL)
    {
        Spring_Logger_Settings.time = SpringLogger_Time;
    }
    return SPRING_LOGGER_OK;
}

void SpringLogger_LOG(int level, const char* tag, const char* format,...)
{
    va_list args;
    va_start(args, format);
    SpringLogger_vLOG(level, tag, format, args);
    va_end(args);
}

void SpringLogger_LOG_DUMP(int level, const char* tag, const char* message, const uint8_t *data, size_t length, size_t width)
{
    const char *name,*color;
    if(SpringLogger_GetLevel(level,&name,&color) != SPRING_LOGGER_OK) return;
    SpringLogger_LOG(level,tag,"%s, Size: %d",message,length);
    if(Spring_Logger_Settings.colors)
    {
        SpringLogger_fprintf(Spring_Logger_IO,"%s",color);
    }
    for(size_t i = 0; i < length; i++)
    {
        if(i && (i % width == 0))
        {
            SpringLogger_fprintf(Spring_Logger_IO,"\n");
        }
        SpringLogger_fprintf(Spring_Logger_IO,"%02X ",data[i]);
    }
    SpringLogger_LOG(level,tag,"\n");
}

void SpringLogger_LOGV(const char* tag, const char* format,...)
{
    va_list args;
    va_start(args, format);
    SpringLogger_vLOG(SPRING_LOGGER_LOG_LEVEL_VERBOSE, tag, format, args);
    va_end(args);
}

void SpringLogger_LOGI(const char* tag, const char* format,...)
{
    va_list args;
    va_start(args, format);
    SpringLogger_vLOG(SPRING_LOGGER_LOG_LEVEL_INFO, tag, format, args);
    va_end(args);
}

void SpringLogger_LOGD(const char* tag, const char* format,...)
{
    va_list args;
    va_start(args, format);
    SpringLogger_vLOG(SPRING_LOGGER_LOG_LEVEL_DEBUG, tag, format, args);
    va_end(args);
}

void SpringLogger_LOGW(const char* tag, const char* format,...)
{
    va_list args;
    va_start(args, format);
    SpringLogger_vLOG(SPRING_LOGGER_LOG_LEVEL_WARNING, tag, format, args);
    va_end(args);
}

void SpringLogger_LOGE(const char* tag, const char* format,...)
{
    va_list args;
    va_start(args, format);
    SpringLogger_vLOG(SPRING_LOGGER_LOG_LEVEL_ERROR, tag, format, args);
    va_end(args);
}

void SpringLogger_Progress(const struct SpringLogger_Progress_t *progress, int value)
{
    if(!progress->new_line)
        SpringLogger_fprintf(Spring_Logger_IO,"%s",SPRING_LOGGER_CR);
    if(progress->color)
        SpringLogger_fprintf(Spring_Logger_IO,"%s",progress->color);
    SpringLogger_fprintf(Spring_Logger_IO,"[");
    for(int i = progress->min ; i < progress->max ; i++)
    {
        if(i < value)
            SpringLogger_fprintf(Spring_Logger_IO,"*");
        else
            SpringLogger_fprintf(Spring_Logger_IO," ");
    }
    SpringLogger_fprintf(Spring_Logger_IO,"] ");
    if(progress->postfix)
        SpringLogger_fprintf(Spring_Logger_IO,"%s",progress->postfix(value,progress));
    else
        SpringLogger_fprintf(Spring_Logger_IO,"%d%%",((value-progress->min)*100)/(progress->max-progress->min));
    if(progress->color)
        SpringLogger_fprintf(Spring_Logger_IO,"%s\n%s",SPRING_LOGGER_COLOR_RESET,SPRING_LOGGER_COLOR_WHITE);
    SpringLogger_fflush(Spring_Logger_IO);
}

void SpringLogger_SetOutput(FILE *fd)
{
    Spring_Logger_IO = fd;
}

__attribute__((weak)) void SpringLogger_vfprintf(FILE* io, const char* fmt, va_list args)
{
    vfprintf(io,fmt,args);
}

__attribute__((weak)) void SpringLogger_fflush(FILE* io)
{
    fflush(io);
}
