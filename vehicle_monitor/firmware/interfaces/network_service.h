#ifndef NETWORK_SERVICE_H
#define NETWORK_SERVICE_H

#include <stdint.h>

void network_service_task(void *argument);
uint32_t network_service_publish_failures(void);

#endif /* NETWORK_SERVICE_H */
