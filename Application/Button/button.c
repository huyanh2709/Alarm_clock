/*
 * button.c
 *
 *  Created on: May 7, 2024
 *      Author: phamh
 */

#include "button.h"
__weak void button_press_callback(Button_Handle_Typedef *ButtonX)
{

}

__weak void button_release_callback(Button_Handle_Typedef *ButtonX)
{

}

__weak void button_short_press_callback(Button_Handle_Typedef *ButtonX)
{

}

__weak void button_press_1sTimeout_callback(Button_Handle_Typedef *ButtonX)
{

}

__weak void button_press_2sTimeout_callback(Button_Handle_Typedef *ButtonX)
{

}

__weak void button_press_3sTimeout_callback(Button_Handle_Typedef *ButtonX)
{

}
void button_init(Button_Handle_Typedef *ButtonX,GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	ButtonX->GPIOx = GPIOx;
	ButtonX->GPIO_Pin = GPIO_Pin;
}

static void button_filter(Button_Handle_Typedef *ButtonX)
{

	uint8_t button_state = HAL_GPIO_ReadPin(ButtonX->GPIOx, ButtonX->GPIO_Pin);

	//đang nhiễu
	if(button_state != ButtonX->button_debounce)
	{
		ButtonX->is_debouncing = 1;
		ButtonX->button_debounce = button_state;
		ButtonX->t_debounce = HAL_GetTick();
	}
	//check nếu đang nhiễu và thời gian nhiễu gần nhất cách >= 15ms
	if(ButtonX->is_debouncing && (HAL_GetTick() - ButtonX->t_debounce >= 15))
	{
		ButtonX->is_debouncing= 0;
		ButtonX->key_current = button_state;
	}
}
uint8_t count;
void button_handle(Button_Handle_Typedef *ButtonX)
{
	button_filter(ButtonX);

	if(ButtonX->key_current != ButtonX->key_last)
	{
		//nhấn nút
		if(ButtonX->key_current == 0)
		{
			ButtonX->is_pressing = 1;
			ButtonX->t_press = HAL_GetTick();
			button_press_callback(ButtonX);
		}
		//nhả nút
		else
		{
			ButtonX->is_pressing = 0;
			//short press
			if(HAL_GetTick() - ButtonX->t_press <= 1000)
			{
				count++;
				button_short_press_callback(ButtonX);
			}
			button_release_callback(ButtonX);


		}
		ButtonX->key_last = ButtonX->key_current;
	}

	//long press
	if(ButtonX->is_pressing && HAL_GetTick() - ButtonX->t_press >= 3000)
	{
		button_press_3sTimeout_callback(ButtonX);
		ButtonX->is_pressing = 0;
	}
	else if(ButtonX->is_pressing && HAL_GetTick() - ButtonX->t_press >= 2000)
	{
		button_press_2sTimeout_callback(ButtonX);
	}
	else if(ButtonX->is_pressing && HAL_GetTick() - ButtonX->t_press >= 1000)
	{
		button_press_1sTimeout_callback(ButtonX);
	}
}





