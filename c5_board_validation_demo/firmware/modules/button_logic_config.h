#ifndef BUTTON_LOGIC_CONFIG_H
#define BUTTON_LOGIC_CONFIG_H

#include <stdbool.h>

#include "button_logic.h"

#define BUTTON_LOGIC_STATE_SETTER button_logic_set_shared_state
#define BUTTON_LOGIC_STATE_GETTER button_logic_get_shared_state
#define BUTTON_LOGIC_STATE_INITIALISER button_logic_initialise_shared_state

#endif /* BUTTON_LOGIC_CONFIG_H */
