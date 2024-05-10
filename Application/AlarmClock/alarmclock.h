/*
 * alarmclock.h
 *
 *  Created on: May 7, 2024
 *      Author: phamh
 */

#ifndef ALARMCLOCK_ALARMCLOCK_H_
#define ALARMCLOCK_ALARMCLOCK_H_

#include "stm32f1xx_hal.h"
#include "DS3231.h"
#include "button.h"
#include "LiquidCrystal_I2C.h"

typedef enum{
	NORMAL,
	SET_TIME,
	SET_ALARM,
	STOP_WATCH
}alarmClockState;

typedef enum{
	hour,
	min,
	sec,
	date,
	month,
	year
}setTimeState;

typedef struct{
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
	uint16_t ms;
	uint8_t is_running;
	TIM_HandleTypeDef *htim;
}stopWatch;

void alarmClock_init(I2C_HandleTypeDef *hi2c, TIM_HandleTypeDef *htim, timedate *td, GPIO_TypeDef *start_button_Port, uint16_t start_button_Pin,
		 GPIO_TypeDef *mode_button_Port, uint16_t mode_button_Pin, GPIO_TypeDef *light_button_Port, uint16_t light_button_Pin);

void alarmClock_display_handle();

void alarmClock_alarm_handle();

void alarmClock_buttons_handle();

void alarmClock_STOP_WATCH_TIM_IT_handle();

#endif /* ALARMCLOCK_ALARMCLOCK_H_ */
