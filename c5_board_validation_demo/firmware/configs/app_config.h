#ifndef APP_CONFIG_H
#define APP_CONFIG_H

/* Board: NUCLEO-C5A3ZG; raw HAL pin tokens are consumed only by platforms/stm32/. */
#define APP_LED_GPIO_PORT HAL_GPIOA
#define APP_LED_GPIO_PIN HAL_GPIO_PIN_5
#define APP_LD1_GPIO_PORT HAL_GPIOA
#define APP_LD1_GPIO_PIN HAL_GPIO_PIN_5
#define APP_LD2_GPIO_PORT HAL_GPIOG
#define APP_LD2_GPIO_PIN HAL_GPIO_PIN_1
#define APP_LD3_GPIO_PORT HAL_GPIOG
#define APP_LD3_GPIO_PIN HAL_GPIO_PIN_2
#define APP_USER_BUTTON_GPIO_PORT HAL_GPIOC
#define APP_USER_BUTTON_GPIO_PIN HAL_GPIO_PIN_13
#define APP_USER_BUTTON_ACTIVE_LEVEL HAL_GPIO_PIN_SET
#define APP_BUTTON_DEBOUNCE_MS 50U
#define APP_BUTTON_POLL_MS 10U
#define APP_HEALTH_REPORT_MS 5000U
#define APP_STATUS_BLINK_COUNT 3U
#define APP_STATUS_BLINK_MS 150U
#define APP_APP_TASK_STACK_BYTES 1024U
#define APP_APP_TASK_PRIORITY 2U
#define APP_MQTT_BROKER_HOST "broker.hivemq.com"
#define APP_MQTT_BROKER_PORT 1883U
#define APP_MQTT_CLIENT_ID "firmgen-c5-board-demo"
#define APP_MQTT_TELEMETRY_TOPIC "/firmgen/c5/telemetry"
#define APP_MQTT_CONTROL_TOPIC "/firmgen/c5/control"
#define APP_MQTT_TELEMETRY_PERIOD_MS 5000U
#define APP_MQTT_RETRY_PERIOD_MS 5000U
#define APP_MQTT_TASK_STACK_BYTES 1536U
#define APP_MQTT_TASK_PRIORITY 2U
#define APP_HTTP_WEATHER_HOST "api.open-meteo.com"
#define APP_HTTP_WEATHER_URI "/v1/forecast?latitude=12.9716&longitude=77.5946&current=temperature_2m,relative_humidity_2m,apparent_temperature,precipitation,weather_code,wind_speed_10m&timezone=Asia%2FKolkata"
#define APP_HTTP_WEATHER_LOCATION "Bengaluru, Karnataka"
#define APP_HTTP_TIME_HOST "worldtimeapi.org"
#define APP_HTTP_TIME_URI "/api/timezone/Asia/Kolkata"
#define APP_HTTP_RESPONSE_MAX_BYTES 1024U
#define APP_HTTP_TASK_STACK_BYTES 2048U
#define APP_HTTP_TASK_PRIORITY 2U
#define APP_HTTP_RETRY_PERIOD_MS 60000U
#define APP_ADC_TASK_STACK_BYTES 1024U
#define APP_ADC_TASK_PRIORITY 2U
#define APP_PWM_FREQUENCY_HZ 1000U
#define APP_PWM_DUTY_PERCENT 50U

#endif /* APP_CONFIG_H */
