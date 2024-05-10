/*
 * button.h
 *
 *  Created on: May 7, 2024
 *      Author: phamh
 */

#ifndef BUTTON_BUTTON_H_
#define BUTTON_BUTTON_H_

#include <stm32f1xx_hal.h>

typedef struct {
	uint8_t key_last;
	uint8_t key_current;

	uint8_t is_pressing;
	uint32_t t_press;

	uint8_t is_debouncing;
	uint32_t t_debounce;
	uint8_t button_debounce;
	uint8_t button_filter_state;

	GPIO_TypeDef *GPIOx;
	uint16_t GPIO_Pin;
}Button_Handle_Typedef;

void button_init(Button_Handle_Typedef *ButtonX,GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

void button_handle(Button_Handle_Typedef *ButtonX);

#endif /* BUTTON_BUTTON_H_ */
