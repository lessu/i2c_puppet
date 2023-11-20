#pragma once

#include <stdint.h>
#include <stdbool.h>

void backlight_sync(void);
void backlight_init(void);
void backlight_set_is_idle(bool is_idle);

void backlight_trigger_active(void);
void backlight_main10ms(void);


