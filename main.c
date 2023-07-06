#include "SpringLogger.h"
#include <stdio.h>
#include <windows.h>

static const char* timestamp(){
    static char buffer[256];
    SYSTEMTIME lt;
    GetLocalTime(&lt);
    sprintf(buffer,"%2d/%02d/%02d %02d:%02d:%02d.%03d",
            lt.wDay,lt.wMonth,lt.wYear,lt.wHour,lt.wMinute,lt.wSecond,lt.wMilliseconds);
    return buffer;
}

static const char* progress_postfix(int value, const struct SpringLogger_Progress_t *settings)
{
    static char buffer[128];
    static DWORD lastTime = 0;
    DWORD now = GetTickCount64();
    DWORD diff = now - lastTime;
    lastTime = now;
    float speed = 1.0f/(float)settings->max;
    sprintf(buffer,"%3.02fGB/%.02fGB %.2fGbs",(float)value,(float)settings->max,speed);
    return buffer;
}

void SpringLogger_vfprintf(FILE* io, const char* fmt, va_list args)
{
    vfprintf(io,fmt,args);
}

void SpringLogger_fflush(FILE* io)
{
    fflush(io);
}

int main() {
    struct SpringLogger_Settings_t settings;
    settings.level = SPRING_LOGGER_LOG_LEVEL_VERBOSE;
    settings.time = timestamp;
    settings.colors = 1;
    settings.flush = 1;
    SpringLogger_Init(stdout,&settings);
    const char* TAG = "Spring";
    int i = 0;
    SpringLogger_LOG_Location(SPRING_LOGGER_LOG_LEVEL_ERROR,TAG,"Unknown ERROR! code: %d",-100);
    SpringLogger_LOGV(TAG,"Wallak: %d",++i);
    SpringLogger_LOGI(TAG,"Wallak: %d",++i);
    SpringLogger_LOGD(TAG,"Wallak: %d",++i);
    SpringLogger_LOGW(TAG,"Wallak: %d",++i);
    SpringLogger_LOGE(TAG,"Wallak: %d",++i);

    struct SpringLogger_Progress_t progress = {.new_line=false,.max=100,.min=0,.postfix=progress_postfix};
    for(int j = 0 ; j < 100 ; j++){
        SpringLogger_Progress(&progress,j);
        Sleep(100);
    }
    alloca(1);

    return 0;
}
