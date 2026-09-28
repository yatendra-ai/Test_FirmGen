#include "network_status_if.h"

static bool g_network_ready = false;

void network_status_set_ready(bool ready)
{
    g_network_ready = ready;
}

bool network_status_is_ready(void)
{
    return g_network_ready;
}
