#include "threshold_service.h"

#include <math.h>

bool threshold_service_trigger(const threshold_config_t *config,
                               float temperature_c,
                               float humidity_percent,
                               float lateral_g,
                               float longitudinal_g,
                               float vertical_g)
{
    float maximum_g;

    if ((config == NULL) || !isfinite(temperature_c) || !isfinite(humidity_percent) ||
        !isfinite(lateral_g) || !isfinite(longitudinal_g) || !isfinite(vertical_g)) {
        return false;
    }
    maximum_g = fmaxf(fabsf(lateral_g), fmaxf(fabsf(longitudinal_g), fabsf(vertical_g)));
    return (temperature_c >= config->temperature_c) ||
           (humidity_percent >= config->humidity_percent) ||
           (maximum_g >= config->g_force);
}
