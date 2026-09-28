#include "http_platform_if.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "lwip/sockets.h"
#include "lwip/inet.h"
#include "lwip/ip_addr.h"

#include "app_config.h"
#include "app_logger.h"
#include "diagnostics.h"
#include "network_status_if.h"
#include "app_lwip.h"
#include "example.h"

#define HTTP_RESPONSE_BUFFER_SIZE (APP_HTTP_RESPONSE_MAX_BYTES + 512U)
#define HTTP_SOCKET_TIMEOUT_MS 10000U

typedef enum http_request_kind_t
{
    HTTP_REQUEST_WEATHER = 0,
    HTTP_REQUEST_TIME
} http_request_kind_t;

static volatile bool g_weather_ready = false;
static volatile bool g_time_ready = false;
static TaskHandle_t g_http_task_handle = NULL;

static void http_platform_task(void *arg);
static bool http_platform_get(http_request_kind_t kind, const char *host, const char *uri);
static bool http_platform_parse_response(http_request_kind_t kind,
                                        const char *response,
                                        uint16_t response_length,
                                        char *body,
                                        size_t body_capacity,
                                        uint16_t *status_code);

void http_platform_start(void)
{
    BaseType_t task_result;

    if (g_http_task_handle == NULL)
    {
        task_result = xTaskCreate(http_platform_task,
                                  "http_services_task",
                                  (configSTACK_DEPTH_TYPE)(APP_HTTP_TASK_STACK_BYTES / sizeof(StackType_t)),
                                  NULL,
                                  APP_HTTP_TASK_PRIORITY,
                                  &g_http_task_handle);
        if (task_result != pdPASS)
        {
            g_http_task_handle = NULL;
            APP_LOG_ERROR("HTTP", "HTTP service task creation failed\n");
        }
        else
        {
            /* HTTP worker is running. */
        }
    }
}

bool http_platform_weather_ready(void)
{
    return g_weather_ready;
}

bool http_platform_time_ready(void)
{
    return g_time_ready;
}

static void http_platform_task(void *arg)
{
    (void)arg;

    while (true)
    {
        if (network_status_is_ready())
        {
            if (!g_weather_ready)
            {
                (void)http_platform_get(HTTP_REQUEST_WEATHER,
                                        APP_HTTP_WEATHER_HOST,
                                        APP_HTTP_WEATHER_URI);
            }
            else
            {
                /* Weather has been retrieved. */
            }

            if (!g_time_ready)
            {
                (void)http_platform_get(HTTP_REQUEST_TIME,
                                        APP_HTTP_TIME_HOST,
                                        APP_HTTP_TIME_URI);
            }
            else
            {
                /* Time has been retrieved. */
            }
        }
        else
        {
            APP_LOG_INFO("HTTP", "waiting for network before HTTP GET requests\n");
        }

        vTaskDelay(pdMS_TO_TICKS(APP_HTTP_RETRY_PERIOD_MS));
    }
}

static bool http_platform_get(http_request_kind_t kind, const char *host, const char *uri)
{
    ip_addr_t server_address;
    struct sockaddr_in socket_address;
    struct timeval timeout;
    char request[512];
    char response[HTTP_RESPONSE_BUFFER_SIZE];
    char body[APP_HTTP_RESPONSE_MAX_BYTES];
    int32_t socket_fd = -1;
    int32_t request_length;
    int32_t sent_length;
    int32_t received_length;
    uint16_t total_received = 0U;
    uint16_t response_status = 0U;
    bool b_success = false;

    (void)memset(response, 0, sizeof(response));
    (void)memset(body, 0, sizeof(body));
    diagnostics_log_status(DIAGNOSTICS_SUBSYSTEM_HTTP,
                           (kind == HTTP_REQUEST_WEATHER) ? "weather GET started" : "time GET started");
    APP_LOG_INFO("HTTP", "HTTP GET host=%s uri=%s location=%s\n",
                 host, uri, APP_HTTP_WEATHER_LOCATION);

    if (app_lwip_dns_resolve(host, &server_address, APP_MQTT_RETRY_PERIOD_MS) != EXEC_STATUS_OK)
    {
        APP_LOG_ERROR("HTTP", "DNS resolution failed for %s\n", host);
        return false;
    }

    socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_fd < 0)
    {
        APP_LOG_ERROR("HTTP", "TCP socket creation failed for %s\n", host);
        return false;
    }

    timeout.tv_sec = (long)(HTTP_SOCKET_TIMEOUT_MS / 1000U);
    timeout.tv_usec = (long)((HTTP_SOCKET_TIMEOUT_MS % 1000U) * 1000U);
    (void)setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    (void)setsockopt(socket_fd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));

    (void)memset(&socket_address, 0, sizeof(socket_address));
    socket_address.sin_family = AF_INET;
    socket_address.sin_port = htons(80U);
    socket_address.sin_addr.s_addr = ip_2_ip4(&server_address)->addr;

    if (connect(socket_fd, (struct sockaddr *)&socket_address, sizeof(socket_address)) != 0)
    {
        APP_LOG_ERROR("HTTP", "TCP connect failed for %s\n", host);
        (void)close(socket_fd);
        return false;
    }

    request_length = snprintf(request,
                              sizeof(request),
                              "GET %s HTTP/1.1\r\nHost: %s\r\nUser-Agent: FirmGen-C5\r\nAccept: application/json\r\nAccept-Encoding: identity\r\nConnection: close\r\n\r\n",
                              uri,
                              host);
    if ((request_length <= 0) || ((size_t)request_length >= sizeof(request)))
    {
        APP_LOG_ERROR("HTTP", "request formatting failed for %s\n", host);
        (void)close(socket_fd);
        return false;
    }

    sent_length = send(socket_fd, request, (size_t)request_length, 0);
    if (sent_length != request_length)
    {
        APP_LOG_ERROR("HTTP", "GET request send incomplete for %s\n", host);
        (void)close(socket_fd);
        return false;
    }

    while (total_received < (uint16_t)(sizeof(response) - 1U))
    {
        received_length = recv(socket_fd,
                               &response[total_received],
                               (size_t)(sizeof(response) - 1U - total_received),
                               0);
        if (received_length <= 0)
        {
            break;
        }

        total_received = (uint16_t)(total_received + received_length);
        response[total_received] = '\0';
    }

    (void)close(socket_fd);
    if (total_received > 0U)
    {
        b_success = http_platform_parse_response(kind,
                                                 response,
                                                 total_received,
                                                 body,
                                                 sizeof(body),
                                                 &response_status);
    }

    APP_LOG_INFO("HTTP", "GET result host=%s status=%u received=%u body=%s\n",
                 host,
                 (unsigned int)response_status,
                 (unsigned int)total_received,
                 (total_received > 0U) ? body : "empty");

    if (b_success)
    {
        if (kind == HTTP_REQUEST_WEATHER)
        {
            g_weather_ready = true;
            diagnostics_log_status(DIAGNOSTICS_SUBSYSTEM_HTTP,
                                   "Bengaluru weather response received");
        }
        else
        {
            g_time_ready = true;
            diagnostics_log_status(DIAGNOSTICS_SUBSYSTEM_HTTP,
                                   "HTTP time response received");
        }
    }
    else
    {
        diagnostics_log_status(DIAGNOSTICS_SUBSYSTEM_HTTP,
                               "HTTP GET failed or response invalid");
    }

    return b_success;
}

static bool http_platform_parse_response(http_request_kind_t kind,
                                        const char *response,
                                        uint16_t response_length,
                                        char *body,
                                        size_t body_capacity,
                                        uint16_t *status_code)
{
    const char *p_body = strstr(response, "\r\n\r\n");
    const char *p_status = strstr(response, "HTTP/1.");
    size_t body_length;
    bool b_valid = false;

    *status_code = 0U;
    if ((p_body != NULL) &&
        (p_status != NULL) &&
        (sscanf(p_status, "HTTP/1.%*u %hu", status_code) == 1))
    {
        p_body += 4;
        body_length = (size_t)(response + response_length - p_body);
        if ((body_length > 0U) && (body_length < body_capacity))
        {
            (void)memcpy(body, p_body, body_length);
            body[body_length] = '\0';

            if ((*status_code >= 200U) && (*status_code < 300U))
            {
                if (kind == HTTP_REQUEST_WEATHER)
                {
                    b_valid = ((strstr(body, "current") != NULL) &&
                               (strstr(body, "temperature_2m") != NULL));
                }
                else
                {
                    b_valid = ((strstr(body, "datetime") != NULL) ||
                               (strstr(body, "utc_datetime") != NULL));
                }
            }
        }
        else
        {
            /* Response body is empty or exceeds the bounded copy buffer. */
        }
    }
    else
    {
        /* Response is missing HTTP status or header/body separator. */
    }

    return b_valid;
}
