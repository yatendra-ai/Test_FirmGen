#include "mqtt_service_if.h"

#include <stdbool.h>

#include "mqtt_platform_if.h"

static bool g_application_state = false;

void mqtt_service_start(void)
{
    mqtt_platform_start();
}

void mqtt_service_set_application_state(bool state)
{
    g_application_state = state;
    mqtt_platform_publish_state(g_application_state);
}

bool mqtt_service_is_connected(void)
{
    return mqtt_platform_is_connected();
}
