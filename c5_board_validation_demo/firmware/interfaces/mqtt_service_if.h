#ifndef MQTT_SERVICE_IF_H
#define MQTT_SERVICE_IF_H

#include <stdbool.h>

void mqtt_service_start(void);
void mqtt_service_set_application_state(bool state);
bool mqtt_service_is_connected(void);

#endif /* MQTT_SERVICE_IF_H */
