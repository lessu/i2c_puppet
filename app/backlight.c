#include "backlight.h"
#include "reg.h"

#include <hardware/pwm.h>
#include <pico/stdlib.h>
static bool s_is_idle;
static uint32_t s_idle_count;
#define BACKLIGHT_MAIN_LOOP_INTERVAL_MS 30
#define BACKLIGHT_MS_TO_COUNT(ms)       (ms/BACKLIGHT_MAIN_LOOP_INTERVAL_MS)
#define BACKLIGHT_IDLE_TURNOFF_MS 	10000/**10s*/
void backlight_sync(void)
{
	if(s_is_idle){
		pwm_set_gpio_level(PIN_BKL, 0);
	}else{
		pwm_set_gpio_level(PIN_BKL, reg_get_value(REG_ID_BKL)  * 0x80);
	}
}

void backlight_init(void)
{
	s_is_idle = false;
	s_idle_count = 0;
	gpio_set_function(PIN_BKL, GPIO_FUNC_PWM);

	const uint slice_num = pwm_gpio_to_slice_num(PIN_BKL);

	pwm_config config = pwm_get_default_config();
	pwm_init(slice_num, &config, true);

	backlight_sync();
}
void backlight_set_is_idle(bool is_idle)
{
	if(s_is_idle == is_idle){
		return ;	
	}
	s_is_idle=is_idle;
	backlight_sync();
}

void backlight_main10ms(void)
{

	if(s_idle_count > BACKLIGHT_MS_TO_COUNT(BACKLIGHT_IDLE_TURNOFF_MS)){
		backlight_set_is_idle(true);
	}else{
		if(s_is_idle == true){
			backlight_set_is_idle(false);
		}
		s_idle_count ++;
	}
}

void backlight_trigger_active(void)
{
	s_idle_count = 0;
}

