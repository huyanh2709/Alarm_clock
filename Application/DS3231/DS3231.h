/*
 * DS3231.h
 *
 *  Created on: May 7, 2024
 *      Author: phamh
 */

#ifndef DS3231_DS3231_H_
#define DS3231_DS3231_H_

#include "stm32f1xx_hal.h"

#define DS3231_ADRS 0x68<<1

typedef struct{
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t day;
	uint8_t date;
	uint8_t month;
	uint8_t year;
}timedate;

typedef struct{
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t flag;
	uint8_t alarm_on_flag;
}alarmTime;

typedef struct
{
	I2C_HandleTypeDef *RTC_I2C;
	timedate *td;
	alarmTime *alarm;
	uint8_t backLight_state;
}RTC_Handle_Typedef;

uint8_t Dec2BCD(uint8_t time);

uint8_t BCD2Dec(uint8_t time);

void RTC_init(RTC_Handle_Typedef *hrtc, I2C_HandleTypeDef *hi2c);

void RTC_writeTime(RTC_Handle_Typedef *hrtc);

void RTC_readTime(RTC_Handle_Typedef *hrtc);

void RTC_setAlarm(RTC_Handle_Typedef *hrtc);

#endif /* DS3231_DS3231_H_ */
