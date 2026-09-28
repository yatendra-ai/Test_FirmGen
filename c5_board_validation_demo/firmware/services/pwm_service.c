#include "pwm_service_if.h"

#include "pwm_platform_if.h"

bool pwm_service_start(void)
{
    return pwm_platform_start();
}
