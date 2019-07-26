
/* Includes ------------------------------------------------------------------*/

#ifndef USER_MAIN_H
#define USER_MAIN_H

	#ifdef __cplusplus
	extern "C" {
	#endif

		#include "stm32f303xc.h"
		#include "stm32f3xx_hal.h"
		#include "stm32f3xx_hal_spi.h"
		#include "stm32f3xx_hal_gpio.h"
		#include "CANSPI.h"
		#include "MCP2515.h"

		/* Includes End---------------------------------------------------------------*/

	void user_main();
	void interrupt();

	#ifdef __cplusplus
	};
	#endif

#endif
