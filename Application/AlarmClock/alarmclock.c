/*
 * alarmclock.c
 *
 *  Created on: May 7, 2024
 *      Author: phamh
 */

#include "alarmclock.h"

static RTC_Handle_Typedef hrtc;
static LiquidCrystal_I2C hlcd;
static alarmClockState ac_state;
static setTimeState setTime_state;
static Button_Handle_Typedef start_button;
static Button_Handle_Typedef mode_button;
static Button_Handle_Typedef light_button;
static alarmTime alarm;
static stopWatch stop_watch;

static void alarmClock_NORMAL_display()
{
	lcd_set_cursor(&hlcd, 0, 4);
	lcd_printf(&hlcd,"%02d:%02d:%02d",hrtc.td->hour,hrtc.td->minute,hrtc.td->second);
	lcd_set_cursor(&hlcd, 1, 4);
	lcd_printf(&hlcd,"%02d/%02d/%02d",hrtc.td->date,hrtc.td->month,hrtc.td->year);
}

static void alarmClock_ALARM_display()
{
	lcd_set_cursor(&hlcd, 0, 4);
	lcd_printf(&hlcd,"SET ALARM");
	lcd_set_cursor(&hlcd, 1, 4);
	lcd_printf(&hlcd,"%02d:%02d:%02d",hrtc.alarm->hour,hrtc.alarm->minute,hrtc.alarm->second);
}

static void alarmClock_RUNNING_display()
{
	static int i;
	if(i>=16)
		i = 0;
	lcd_clear_display(&hlcd);
	HAL_Delay(1);
	lcd_set_cursor(&hlcd, 0, i);
	lcd_printf(&hlcd,"%02d:%02d:%02d",hrtc.td->hour,hrtc.td->minute,hrtc.td->second);
	lcd_set_cursor(&hlcd, 1, 4);
	lcd_printf(&hlcd,"%02d/%02d/%02d",hrtc.td->date,hrtc.td->month,hrtc.td->year);
	i++;
}

static void alarmClock_number_blink(uint8_t row, uint8_t col)
{
	if(HAL_GetTick()%600 == 0)
	{
		lcd_set_cursor(&hlcd,row,col);
		lcd_printf(&hlcd, "  ");
	}
	if(HAL_GetTick()%600 == 300)
	{
		if(ac_state == SET_TIME)
			alarmClock_NORMAL_display();
		if(ac_state == SET_ALARM)
			alarmClock_ALARM_display();

	}
}

static void alarmClock_SET_TIME_display()
{
	switch (setTime_state)
	{
		case hour:
			alarmClock_number_blink(0,4);
			break;
		case min:
			alarmClock_number_blink(0,7);
			break;
		case sec:
			alarmClock_number_blink(0,10);
			break;
		case date:
			alarmClock_number_blink(1,4);
			break;
		case month:
			alarmClock_number_blink(1,7);
			break;
		case year:
			alarmClock_number_blink(1,10);
			break;
		default: break;
	}
}

static void alarmClock_SET_ALARM_display()
{
	switch (setTime_state)
	{
		case hour:
			alarmClock_number_blink(1,4);
			break;
		case min:
			alarmClock_number_blink(1,7);
			break;
		case sec:
			alarmClock_number_blink(1,10);
			break;
		default: break;
	}
}

static void alarmClock_STOP_WATCH_display()
{
	lcd_set_cursor(&hlcd,0,4);
	lcd_printf(&hlcd,"STOP WATCH");
	lcd_set_cursor(&hlcd,1,2);
	lcd_printf(&hlcd,"%02d:%02d:%02d.%03d",stop_watch.hour,stop_watch.min,stop_watch.sec,stop_watch.ms);
}

static void setTime_button_handle()
{
	switch (setTime_state)
	{
		case hour:
			if(hrtc.td->hour < 23)
				hrtc.td->hour++;
			else
				hrtc.td->hour = 0;
			RTC_writeTime(&hrtc);
			break;
		case min:
			if(hrtc.td->minute < 59)
				hrtc.td->minute++;
			else
				hrtc.td->minute = 0;
			RTC_writeTime(&hrtc);
			break;
		case sec:
			if(hrtc.td->second < 59)
				hrtc.td->second++;
			else
				hrtc.td->second = 0;
			RTC_writeTime(&hrtc);
			break;
		case date:
			if(hrtc.td->date < 31)
				hrtc.td->date++;
			else
				hrtc.td->date = 1;
			RTC_writeTime(&hrtc);
			break;
		case month:
			if(hrtc.td->month < 12)
				hrtc.td->month++;
			else
				hrtc.td->month = 1;
			RTC_writeTime(&hrtc);
			break;
		case year:
			if(hrtc.td->year < 99)
				hrtc.td->year++;
			else
				hrtc.td->year = 0;
			RTC_writeTime(&hrtc);
			break;
		default: break;
	}
}

static void setAlarm_button_handle()
{
	switch (setTime_state)
	{
		case hour:
			if(hrtc.alarm->hour < 23)
				hrtc.alarm->hour++;
			else
				hrtc.alarm->hour = 0;
			break;
		case min:
			if(hrtc.alarm->minute < 59)
				hrtc.alarm->minute++;
			else
				hrtc.alarm->minute = 0;
			break;
		case sec:
			if(hrtc.alarm->second < 59)
				hrtc.alarm->second++;
			else
				hrtc.alarm->second = 0;
			break;
		default: break;
	}
}

static void start_button_short_press_handle()
{
	switch (ac_state)
	{
		case SET_TIME:
			setTime_button_handle();
			break;
		case SET_ALARM:
			setAlarm_button_handle();
			break;
		case STOP_WATCH:
			if(stop_watch.is_running)
				HAL_TIM_Base_Stop(stop_watch.htim);
			else
				HAL_TIM_Base_Start_IT(stop_watch.htim);
			stop_watch.is_running =! stop_watch.is_running;
			break;
		default: break;
	}
}

static void mode_button_short_press_handle()
{
	switch (ac_state)
	{
		case SET_TIME:
			ac_state = NORMAL;
			lcd_clear_display(&hlcd);
			HAL_Delay(1);
			break;
		case SET_ALARM:
			RTC_setAlarm(&hrtc);
			ac_state = NORMAL;
			lcd_clear_display(&hlcd);
			HAL_Delay(1);
			hrtc.alarm->alarm_on_flag = 1;
			break;
		case STOP_WATCH:
			ac_state = NORMAL;
			lcd_clear_display(&hlcd);
			HAL_Delay(1);
			break;
		default: break;
	}
}

static void light_button_short_press_handle()
{
	if(hrtc.alarm->flag == 0)
	{
		switch (ac_state)
		{
			case NORMAL:
				hrtc.backLight_state =! hrtc.backLight_state;
				break;
			case SET_TIME:
				switch(setTime_state)
				{
				case hour:
					setTime_state = min;
					break;
				case min:
					setTime_state = sec;
					break;
				case sec:
					setTime_state = date;
					break;
				case date:
					setTime_state = month;
					break;
				case month:
					setTime_state = year;
					break;
				case year:
					setTime_state = hour;
					break;
				default: break;
				}
				break;
			case SET_ALARM:
				switch(setTime_state)
				{
				case hour:
					setTime_state = min;
					break;
				case min:
					setTime_state = sec;
					break;
				case sec:
					setTime_state = hour;
					break;
				default: break;
				}
				break;
			case STOP_WATCH:
				HAL_TIM_Base_Stop(stop_watch.htim);
				stop_watch.htim->Instance->CNT = 0;

				stop_watch.hour = 0;
				stop_watch.min = 0;
				stop_watch.sec = 0;
				stop_watch.ms = 0;
				stop_watch.is_running = 0;
				break;
			default: break;

		}
	}
	else
	{
		hrtc.alarm->flag = 0;
		hrtc.alarm->alarm_on_flag = 0;
	}
}

void button_press_1sTimeout_callback(Button_Handle_Typedef *ButtonX)
{
	if(ButtonX == &mode_button)
	{
		ac_state = SET_TIME;
		setTime_state = hour;
		lcd_clear_display(&hlcd);
		HAL_Delay(1);
	}
}

void button_press_2sTimeout_callback(Button_Handle_Typedef *ButtonX)
{
	if(ButtonX == &mode_button)
	{
		ac_state = SET_ALARM;
		setTime_state = hour;
		lcd_clear_display(&hlcd);
		HAL_Delay(1);
	}
}

void button_press_3sTimeout_callback(Button_Handle_Typedef *ButtonX)
{
	if(ButtonX == &mode_button)
	{
		ac_state = STOP_WATCH;
		lcd_clear_display(&hlcd);
		HAL_Delay(1);
	}
}

void button_short_press_callback(Button_Handle_Typedef *ButtonX)
{
	if(ButtonX == &mode_button)
		mode_button_short_press_handle();
	if(ButtonX == &start_button)
		start_button_short_press_handle();
	if(ButtonX == &light_button)
		light_button_short_press_handle();
}

void alarmClock_init(I2C_HandleTypeDef *hi2c, TIM_HandleTypeDef *htim, timedate *td, GPIO_TypeDef *start_button_Port, uint16_t start_button_Pin,
		 GPIO_TypeDef *mode_button_Port, uint16_t mode_button_Pin, GPIO_TypeDef *light_button_Port, uint16_t light_button_Pin		)
{
	lcd_init(&hlcd, hi2c, LCD_ADDR_DEFAULT);
	RTC_init(&hrtc, hi2c);
	button_init(&start_button, start_button_Port, start_button_Pin);
	button_init(&mode_button, mode_button_Port, mode_button_Pin);
	button_init(&light_button, light_button_Port, light_button_Pin);

	hrtc.td = td;
	RTC_writeTime(&hrtc);

	alarm.hour = 0;
	alarm.minute = 0;
	alarm.second = 0;
	alarm.flag = 0;
	alarm.alarm_on_flag = 0;
	hrtc.alarm = &alarm;

	stop_watch.hour = 0;
	stop_watch.min = 0;
	stop_watch.sec = 0;
	stop_watch.ms = 0;
	stop_watch.is_running = 0;
	stop_watch.htim = htim;
}

void alarmClock_display_handle()
{
	RTC_readTime(&hrtc);
	switch (ac_state)
	{
	case NORMAL:
		if(alarm.flag != 1)
		{
			alarmClock_NORMAL_display();
			if(hrtc.backLight_state == 1)
				lcd_backlight_on(&hlcd);
			else
				lcd_backlight_off(&hlcd);
		}
		break;
	case SET_TIME:
		alarmClock_SET_TIME_display();
		break;
	case SET_ALARM:
		alarmClock_SET_ALARM_display();
		break;
	case STOP_WATCH:
		stop_watch.ms = (uint16_t)stop_watch.htim->Instance->CNT;
		alarmClock_STOP_WATCH_display();
		break;
	}
}

void alarmClock_buttons_handle()
{
	button_handle(&start_button);
	button_handle(&mode_button);
	button_handle(&light_button);
}

void alarmClock_alarm_handle()
{
	static uint32_t alarm_blink_t;
	if(!hrtc.alarm->flag && hrtc.alarm->alarm_on_flag)
	{
		if(hrtc.td->hour == hrtc.alarm->hour && hrtc.td->minute == hrtc.alarm->minute && hrtc.td->second == hrtc.alarm->second)
			hrtc.alarm->flag = 1;
	}
	if(hrtc.alarm->flag && hrtc.alarm->alarm_on_flag)
	{
//			RTC_alarm_callback(hrtc);
		if(HAL_GetTick() - alarm_blink_t >= 1000)
		{
			if(hrtc.backLight_state)
				lcd_backlight_off(&hlcd);
			else
				lcd_backlight_on(&hlcd);
			hrtc.backLight_state =! hrtc.backLight_state;
			alarm_blink_t = HAL_GetTick();
//			alarmClock_ALARM_RING_display();

		}

	}
}

void alarmClock_STOP_WATCH_TIM_IT_handle()
{
	if(stop_watch.sec < 59)
		stop_watch.sec++;
	else
	{
		stop_watch.sec = 0;
		if(stop_watch.min < 59)
		{
			stop_watch.min++;
		}
		else
		{
			stop_watch.min = 0;
			stop_watch.hour++;
		}
	}
}




