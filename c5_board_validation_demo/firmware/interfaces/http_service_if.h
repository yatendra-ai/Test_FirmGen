#ifndef HTTP_SERVICE_IF_H
#define HTTP_SERVICE_IF_H

#include <stdbool.h>

void http_service_start(void);
bool http_service_weather_ready(void);
bool http_service_time_ready(void);

#endif /* HTTP_SERVICE_IF_H */
