#include "adc_service_if.h"

#include "adc_platform_if.h"

bool adc_service_start(void)
{
    return adc_platform_start();
}
