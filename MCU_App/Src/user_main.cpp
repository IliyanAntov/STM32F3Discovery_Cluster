#include <user_main.h>

extern SPI_HandleTypeDef hspi1;

void user_main()
{
	uint8_t address = 0x29;
	uint8_t result = 1;
	//uint8_t data[4] = { 1, 2, 3, 4 };
	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 0)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, (GPIO_PinState)1);
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, (GPIO_PinState)1);

	} else {

		HAL_Delay(10);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, (GPIO_PinState)0);
		HAL_SPI_Transmit(&hspi1, &address, 1, 10);
		HAL_SPI_Receive(&hspi1, &result, 1, 10);
		if(result == 1){
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, (GPIO_PinState)1);
		}
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, (GPIO_PinState)0);
	}
}
