#ifndef APP_CONFIG_H
#define APP_CONFIG_H

/* Application-level compile-time constants: GPIO pins, buffer sizes, periods, etc.
   Add #define macros here; every source file that uses them includes "app_config.h". */

/* Supported hardware baseline for this firmware. */
#define APP_BOARD_MODEL "ESP32-C3-DevKitM-1"
#define APP_POWER_INPUT "5 V micro-USB from regulated 12 V automotive adapter"

#define APP_DHT22_DATA_GPIO 2
#define APP_DHT22_SAMPLE_PERIOD_MS 2000U
#define APP_DHT22_RETRY_LIMIT 1U

#define APP_MPU6050_SCL_GPIO 8
#define APP_MPU6050_SDA_GPIO 9
#define APP_MPU6050_I2C_ADDRESS 0x68U
#define APP_MPU6050_I2C_FREQUENCY_HZ 100000U
#define APP_MPU6050_I2C_TIMEOUT_MS 100U
#define APP_MPU6050_SAMPLE_PERIOD_MS 100U
#define APP_MPU6050_RETRY_PERIOD_MS 1000U

#define APP_WIFI_SSID "GopalaSutharsan"
#define APP_WIFI_PASSWORD "123987456"
#define APP_MQTT_BROKER_HOST "broker.hivemq.com"
#define APP_MQTT_BROKER_PORT 1883U
#define APP_MQTT_TELEMETRY_TOPIC "vehicle/cabin/telemetry"
#define APP_MQTT_ALERT_TOPIC "vehicle/cabin/alert"
#define APP_TELEMETRY_PERIOD_MS 2000U
#define APP_NETWORK_RETRY_INITIAL_MS 1000U
#define APP_NETWORK_RETRY_MAX_MS 30000U
#define APP_MQTT_PUBLISH_TIMEOUT_MS 1000U
#define APP_MQTT_QUEUE_DEPTH 4U
#define APP_TEMPERATURE_ALERT_C 50.0F
#define APP_HUMIDITY_ALERT_PERCENT 80.0F
#define APP_G_FORCE_ALERT 1.5F

#endif /* APP_CONFIG_H */
