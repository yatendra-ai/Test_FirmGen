#include "http_service_if.h"

#include <stdbool.h>

#include "http_platform_if.h"

void http_service_start(void)
{
    http_platform_start();
}

bool http_service_weather_ready(void)
{
    return http_platform_weather_ready();
}

bool http_service_time_ready(void)
{
    return http_platform_time_ready();
}
