#ifndef THRESHOLD_SERVICE_H
#define THRESHOLD_SERVICE_H

#include <stdbool.h>

typedef struct {
    float temperature_c;
    float humidity_percent;
    float g_force;
} threshold_config_t;

bool threshold_service_load(threshold_config_t *config);
bool threshold_service_update(const threshold_config_t *config);
const threshold_config_t *threshold_service_active(void);
bool threshold_service_trigger(const threshold_config_t *config,
                               float temperature_c,
                               float humidity_percent,
                               float lateral_g,
                               float longitudinal_g,
                               float vertical_g);

#endif /* THRESHOLD_SERVICE_H */
