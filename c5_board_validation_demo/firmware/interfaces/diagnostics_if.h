#ifndef DIAGNOSTICS_IF_H
#define DIAGNOSTICS_IF_H

#include <stdbool.h>
#include <stdint.h>

typedef enum diagnostics_subsystem_t
{
    DIAGNOSTICS_SUBSYSTEM_NETWORK = 0,
    DIAGNOSTICS_SUBSYSTEM_HTTP,
    DIAGNOSTICS_SUBSYSTEM_MQTT,
    DIAGNOSTICS_SUBSYSTEM_ADC,
    DIAGNOSTICS_SUBSYSTEM_PWM,
    DIAGNOSTICS_SUBSYSTEM_BUTTON
} diagnostics_subsystem_t;

bool diagnostics_uart_write_line(const char *line);
void diagnostics_log_boot(const char *board_name);
void diagnostics_log_status(diagnostics_subsystem_t subsystem, const char *status);
void diagnostics_log_network_address(const char *name, const char *address);
void diagnostics_log_health(uint32_t sequence,
                            bool network_ready,
                            bool http_ready,
                            bool mqtt_ready,
                            bool adc_ready,
                            bool pwm_ready,
                            bool button_ready);

#endif /* DIAGNOSTICS_IF_H */
