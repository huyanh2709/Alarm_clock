/*
 * DS3231.c
 *
 *  Created on: May 7, 2024
 *      Author: phamh
 */

#include "DS3231.h"

uint8_t Dec2BCD(uint8_t time)
{
	return (time/10)<<4 | (time%10);
}

uint8_t BCD2Dec(uint8_t time)
{
	return (time>>4)*10 + (time & 0x0F);
}

void RTC_init(RTC_Handle_Typedef *hrtc, I2C_HandleTypeDef *hi2c)
{
	hrtc->RTC_I2C = hi2c;
	hrtc->backLight_state = 0;
}

void RTC_writeTime(RTC_Handle_Typedef *hrtc)
{
	uint8_t buff[8];
	buff[0] = 0x00;
	buff[1] = Dec2BCD(hrtc->td->second);
	buff[2] = Dec2BCD(hrtc->td->minute);
	buff[3] = Dec2BCD(hrtc->td->hour);
	buff[4] = Dec2BCD(hrtc->td->day);
	buff[5] = Dec2BCD(hrtc->td->date);
	buff[6] = Dec2BCD(hrtc->td->month);
	buff[7] = Dec2BCD(hrtc->td->year);

	HAL_I2C_Master_Transmit(hrtc->RTC_I2C, DS3231_ADRS, buff, 8, 100);
}

void RTC_readTime(RTC_Handle_Typedef *hrtc)
{
	uint8_t buff[7];
	uint8_t address_reg = 0x00;

	HAL_I2C_Master_Transmit(hrtc->RTC_I2C, DS3231_ADRS, &address_reg, 1, 100);
	HAL_I2C_Master_Receive(hrtc->RTC_I2C, DS3231_ADRS, buff, 7, 100);

	hrtc->td->second = BCD2Dec(buff[0]);
	hrtc->td->minute = BCD2Dec(buff[1]);
	hrtc->td->hour = BCD2Dec(buff[2]);
	hrtc->td->day = BCD2Dec(buff[3]);
	hrtc->td->date = BCD2Dec(buff[4]);
	hrtc->td->month = BCD2Dec(buff[5]);
	hrtc->td->year = BCD2Dec(buff[6]);

}

void RTC_setAlarm(RTC_Handle_Typedef *hrtc)
{
	uint8_t buff[4];
	buff[0] = 0x07;
	buff[1] = Dec2BCD(hrtc->alarm->second);
	buff[2] = Dec2BCD(hrtc->alarm->minute);
	buff[3] = Dec2BCD(hrtc->alarm->hour);

	HAL_I2C_Master_Transmit(hrtc->RTC_I2C, DS3231_ADRS, buff, 4, 100);
}

void RTC_readAlarm(RTC_Handle_Typedef *hrtc)
{
	uint8_t buff[3];
	uint8_t address_reg = 0x07;

	HAL_I2C_Master_Transmit(hrtc->RTC_I2C, DS3231_ADRS, &address_reg, 1, 100);
	HAL_I2C_Master_Receive(hrtc->RTC_I2C, DS3231_ADRS, buff, 3, 100);

	hrtc->alarm->second = BCD2Dec(buff[0]);
	hrtc->alarm->minute = BCD2Dec(buff[1]);
	hrtc->alarm->hour = BCD2Dec(buff[2]);
}



