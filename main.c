#include "SpringLogger.h"
#include <stdio.h>
int main() {
    struct SpringLogger_Settings_t settings;
    settings.level = SPRING_LOGGER_LOG_LEVEL_VERBOSE;
    settings.time = NULL; // use default time printing (epoch using time(NULL) function)
    settings.colors = 1;
    settings.flush = 1;
    SpringLogger_Init(stdout,&settings);
    const char* TAG = "Spring";
    int i = 0;
    SpringLogger_LOG_Location(SPRING_LOGGER_LOG_LEVEL_ERROR,TAG,"Unknown ERROR!");
    SpringLogger_LOGV(TAG,"Wallak: %d",++i);
    SpringLogger_LOGI(TAG,"Wallak: %d",++i);
    SpringLogger_LOGD(TAG,"Wallak: %d",++i);
    SpringLogger_LOGW(TAG,"Wallak: %d",++i);
    SpringLogger_LOGE(TAG,"Wallak: %d",++i);
    return 0;
}
