#include "diagnostics.h"

#include <stdio.h>

static const char *diagnostics_status_name(diagnostics_subsystem_t subsystem)
{
    const char *p_name = "unknown";

    switch (subsystem)
    {
        case DIAGNOSTICS_SUBSYSTEM_NETWORK:
            p_name = "network";
            break;
        case DIAGNOSTICS_SUBSYSTEM_HTTP:
            p_name = "HTTP";
            break;
        case DIAGNOSTICS_SUBSYSTEM_MQTT:
            p_name = "MQTT";
            break;
        case DIAGNOSTICS_SUBSYSTEM_ADC:
            p_name = "ADC";
            break;
        case DIAGNOSTICS_SUBSYSTEM_PWM:
            p_name = "PWM";
            break;
        case DIAGNOSTICS_SUBSYSTEM_BUTTON:
            p_name = "button";
            break;
        default:
            break;
    }

    return p_name;
}

void diagnostics_log_boot(const char *board_name)
{
    char line[128];

    (void)snprintf(line, sizeof(line), "boot status: started board=%s",
                   (board_name != NULL) ? board_name : "unknown");
    diagnostics_uart_write_line(line);
}

void diagnostics_log_status(diagnostics_subsystem_t subsystem, const char *status)
{
    char line[128];

    (void)snprintf(line, sizeof(line), "%s status: %s",
                   diagnostics_status_name(subsystem),
                   (status != NULL) ? status : "unknown");
    diagnostics_uart_write_line(line);
}

void diagnostics_log_network_address(const char *name, const char *address)
{
    char line[128];

    (void)snprintf(line, sizeof(line), "DHCP %s address=%s",
                   (name != NULL) ? name : "unknown",
                   (address != NULL) ? address : "unavailable");
    (void)diagnostics_uart_write_line(line);
}

void diagnostics_log_health(uint32_t sequence,
                            bool network_ready,
                            bool http_ready,
                            bool mqtt_ready,
                            bool adc_ready,
                            bool pwm_ready,
                            bool button_ready)
{
    char line[192];

    (void)snprintf(line, sizeof(line),
                   "board-health #%u network=%s HTTP=%s MQTT=%s ADC=%s PWM=%s button=%s",
                   (unsigned int)sequence,
                   network_ready ? "ready" : "not-ready",
                   http_ready ? "ready" : "not-ready",
                   mqtt_ready ? "ready" : "not-ready",
                   adc_ready ? "ready" : "not-ready",
                   pwm_ready ? "ready" : "not-ready",
                   button_ready ? "ready" : "not-ready");
    diagnostics_uart_write_line(line);
}
