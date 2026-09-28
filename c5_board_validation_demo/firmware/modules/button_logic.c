#include "button_logic.h"

#include <stddef.h>

static bool g_application_state = false;

void button_logic_init(button_logic_t *p_logic, bool initially_pressed, uint32_t now_ms)
{
    if (p_logic != NULL)
    {
        p_logic->b_stable_pressed = initially_pressed;
        p_logic->b_candidate_pressed = initially_pressed;
        p_logic->candidate_since_ms = now_ms;
        p_logic->b_initialized = true;
    }
}

bool button_logic_update(button_logic_t *p_logic, bool pressed, uint32_t now_ms, uint32_t debounce_ms)
{
    bool b_press_event = false;

    if ((p_logic == NULL) || (!p_logic->b_initialized))
    {
        return false;
    }

    if (pressed != p_logic->b_candidate_pressed)
    {
        p_logic->b_candidate_pressed = pressed;
        p_logic->candidate_since_ms = now_ms;
    }
    else if ((pressed != p_logic->b_stable_pressed) &&
             ((uint32_t)(now_ms - p_logic->candidate_since_ms) >= debounce_ms))
    {
        p_logic->b_stable_pressed = pressed;
        b_press_event = pressed;
    }
    else
    {
        /* No stable transition has completed. */
    }

    return b_press_event;
}

bool button_logic_toggle_state(bool *p_state)
{
    bool b_state = false;

    if (p_state != NULL)
    {
        *p_state = !(*p_state);
        b_state = *p_state;
    }
    else
    {
        /* Invalid state storage leaves the state unchanged. */
    }

    return b_state;
}

void button_logic_initialise_shared_state(bool initial_state)
{
    g_application_state = initial_state;
}

bool button_logic_set_shared_state(bool state)
{
    g_application_state = state;
    return g_application_state;
}

bool button_logic_get_shared_state(void)
{
    return g_application_state;
}
