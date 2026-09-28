#ifndef MQTT_PLATFORM_IF_H
#define MQTT_PLATFORM_IF_H

#include <stdbool.h>

void mqtt_platform_start(void);
bool mqtt_platform_is_connected(void);
void mqtt_platform_publish_state(bool state);

#endif /* MQTT_PLATFORM_IF_H */
