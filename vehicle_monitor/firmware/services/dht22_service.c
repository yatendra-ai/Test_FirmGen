#include "dht22_service.h"

#include <stddef.h>

#include "app_config.h"
#include "dht22_port.h"

static dht22_sample_t g_dht22_sample;

static bool dht22_checksum_valid(const uint8_t frame[5])
{
    uint8_t checksum;

    checksum = (uint8_t)(frame[0] + frame[1] + frame[2] + frame[3]);
    return (checksum == frame[4]);
}

static void dht22_decode(const uint8_t frame[5], dht22_sample_t *sample)
{
    uint16_t humidity_raw;
    uint16_t temperature_raw;
    bool negative;

    humidity_raw = (uint16_t)(((uint16_t)frame[0] << 8U) | frame[1]);
    temperature_raw = (uint16_t)(((uint16_t)frame[2] << 8U) | frame[3]);
    negative = ((temperature_raw & 0x8000U) != 0U);
    temperature_raw = (uint16_t)(temperature_raw & 0x7FFFU);

    sample->humidity_tenths_percent = humidity_raw;
    sample->temperature_tenths_c = (int16_t)temperature_raw;
    if (negative) {
        sample->temperature_tenths_c = (int16_t)(-sample->temperature_tenths_c);
    }
    sample->valid = true;
}

static bool dht22_try_read(const dht22_port_t *port, uint8_t frame[5])
{
    bool read_ok;

    read_ok = port->read_frame(frame);
    return (read_ok && dht22_checksum_valid(frame));
}

void dht22_service_start(void)
{
    dht22_port_t port;
    uint8_t frame[5];

    g_dht22_sample.temperature_tenths_c = 0;
    g_dht22_sample.humidity_tenths_percent = 0U;
    g_dht22_sample.failure_count = 0U;
    g_dht22_sample.valid = false;

    if (!dht22_port_open(&port)) {
        g_dht22_sample.failure_count++;
        return;
    }

    for (;;) {
        if (dht22_try_read(&port, frame)) {
            dht22_decode(frame, &g_dht22_sample);
        } else if (APP_DHT22_RETRY_LIMIT > 0U) {
            if (dht22_try_read(&port, frame)) {
                dht22_decode(frame, &g_dht22_sample);
            } else {
                g_dht22_sample.failure_count++;
            }
        } else {
            g_dht22_sample.failure_count++;
        }

        port.delay_ms(APP_DHT22_SAMPLE_PERIOD_MS);
    }
}

const dht22_sample_t *dht22_service_sample(void)
{
    return &g_dht22_sample;
}
