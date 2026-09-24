#ifndef DHT22_PORT_H
#define DHT22_PORT_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    bool (*read_frame)(uint8_t frame[5]);
    void (*delay_ms)(uint32_t milliseconds);
} dht22_port_t;

bool dht22_port_open(dht22_port_t *port);

#endif /* DHT22_PORT_H */
