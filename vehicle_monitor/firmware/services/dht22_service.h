#ifndef DHT22_SERVICE_H
#define DHT22_SERVICE_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    int16_t temperature_tenths_c;
    uint16_t humidity_tenths_percent;
    uint32_t failure_count;
    bool valid;
} dht22_sample_t;

void dht22_service_start(void);
const dht22_sample_t *dht22_service_sample(void);

#endif /* DHT22_SERVICE_H */
