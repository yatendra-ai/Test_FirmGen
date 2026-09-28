#ifndef HTTP_PLATFORM_IF_H
#define HTTP_PLATFORM_IF_H

#include <stdbool.h>

void http_platform_start(void);
bool http_platform_weather_ready(void);
bool http_platform_time_ready(void);

#endif /* HTTP_PLATFORM_IF_H */
