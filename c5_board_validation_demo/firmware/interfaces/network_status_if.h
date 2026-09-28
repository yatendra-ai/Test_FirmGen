#ifndef NETWORK_STATUS_IF_H
#define NETWORK_STATUS_IF_H

#include <stdbool.h>

void network_status_set_ready(bool ready);
bool network_status_is_ready(void);

#endif /* NETWORK_STATUS_IF_H */
