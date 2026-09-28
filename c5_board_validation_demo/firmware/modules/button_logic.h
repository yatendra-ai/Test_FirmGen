#ifndef BUTTON_LOGIC_H
#define BUTTON_LOGIC_H

#include <stdbool.h>
#include <stdint.h>

typedef struct button_logic_t
{
    bool b_stable_pressed;
    bool b_candidate_pressed;
    uint32_t candidate_since_ms;
    bool b_initialized;
} button_logic_t;

void button_logic_init(button_logic_t *p_logic, bool initially_pressed, uint32_t now_ms);
bool button_logic_update(button_logic_t *p_logic, bool pressed, uint32_t now_ms, uint32_t debounce_ms);
bool button_logic_toggle_state(bool *p_state);
void button_logic_initialise_shared_state(bool initial_state);
bool button_logic_set_shared_state(bool state);
bool button_logic_get_shared_state(void);

#endif /* BUTTON_LOGIC_H */
