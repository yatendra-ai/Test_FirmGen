#include "threshold_service.h"

#include <math.h>

#include "app_config.h"
#include "esp_log.h"
#include "nvs.h"
#include "nvs_flash.h"

#define THRESHOLD_NAMESPACE "thresholds"
#define THRESHOLD_VERSION_KEY "version"
#define THRESHOLD_TEMP_KEY "temp_c"
#define THRESHOLD_HUMIDITY_KEY "humidity"
#define THRESHOLD_G_FORCE_KEY "g_force"
#define THRESHOLD_VERSION 1U

static const char *TAG = "threshold";
static threshold_config_t g_active_config;

static bool threshold_values_valid(const threshold_config_t *config)
{
    return (config != NULL) && isfinite(config->temperature_c) &&
           isfinite(config->humidity_percent) && isfinite(config->g_force) &&
           (config->temperature_c > -40.0F) && (config->temperature_c < 125.0F) &&
           (config->humidity_percent >= 0.0F) && (config->humidity_percent <= 100.0F) &&
           (config->g_force > 0.0F) && (config->g_force <= 16.0F);
}

static threshold_config_t threshold_defaults(void)
{
    threshold_config_t config;

    config.temperature_c = APP_TEMPERATURE_ALERT_C;
    config.humidity_percent = APP_HUMIDITY_ALERT_PERCENT;
    config.g_force = APP_G_FORCE_ALERT;
    return config;
}

bool threshold_service_load(threshold_config_t *config)
{
    nvs_handle_t handle;
    uint32_t version;
    threshold_config_t loaded;

    if (config == NULL) {
        return false;
    }
    loaded = threshold_defaults();
    if (nvs_open(THRESHOLD_NAMESPACE, NVS_READONLY, &handle) == ESP_OK) {
        if ((nvs_get_u32(handle, THRESHOLD_VERSION_KEY, &version) != ESP_OK) ||
            (version != THRESHOLD_VERSION) ||
            (nvs_get_blob(handle, THRESHOLD_TEMP_KEY, &loaded.temperature_c, &(size_t){sizeof(float)}) != ESP_OK) ||
            (nvs_get_blob(handle, THRESHOLD_HUMIDITY_KEY, &loaded.humidity_percent, &(size_t){sizeof(float)}) != ESP_OK) ||
            (nvs_get_blob(handle, THRESHOLD_G_FORCE_KEY, &loaded.g_force, &(size_t){sizeof(float)}) != ESP_OK) ||
            !threshold_values_valid(&loaded)) {
            ESP_LOGW(TAG, "invalid threshold record; using defaults");
            loaded = threshold_defaults();
        }
        nvs_close(handle);
    }
    g_active_config = loaded;
    *config = loaded;
    return true;
}

bool threshold_service_update(const threshold_config_t *config)
{
    nvs_handle_t handle;
    esp_err_t result;
    threshold_config_t previous;
    size_t value_size;

    if (!threshold_values_valid(config)) {
        return false;
    }
    previous = g_active_config;
    result = nvs_open(THRESHOLD_NAMESPACE, NVS_READWRITE, &handle);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "threshold NVS open failed");
        return false;
    }
    result = nvs_set_u32(handle, THRESHOLD_VERSION_KEY, THRESHOLD_VERSION);
    value_size = sizeof(float);
    result = (result == ESP_OK) ? nvs_set_blob(handle, THRESHOLD_TEMP_KEY, &config->temperature_c, value_size) : result;
    result = (result == ESP_OK) ? nvs_set_blob(handle, THRESHOLD_HUMIDITY_KEY, &config->humidity_percent, value_size) : result;
    result = (result == ESP_OK) ? nvs_set_blob(handle, THRESHOLD_G_FORCE_KEY, &config->g_force, value_size) : result;
    result = (result == ESP_OK) ? nvs_commit(handle) : result;
    nvs_close(handle);
    if (result != ESP_OK) {
        g_active_config = previous;
        ESP_LOGE(TAG, "threshold persistence failed: %s", esp_err_to_name(result));
        return false;
    }
    g_active_config = *config;
    return true;
}

const threshold_config_t *threshold_service_active(void)
{
    return &g_active_config;
}
