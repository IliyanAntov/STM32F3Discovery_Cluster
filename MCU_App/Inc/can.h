/*
 * can.h
 *
 *  Created on: Jul 30, 2019
 *      Author: SoMe0nE
 */

#ifndef CAN_H_
#define CAN_H_

#include <stdint.h>
#include "data.h"

class Can {
	public:
		Can();
		void Initialize();
		void ReadInput();
	private:
		void SetSpeed(Data* dataClass, uint8_t newValue);
		void SetLEDs(Data* dataClass, uint8_t newValue);
		void SetHorn(Data* dataClass, uint8_t newValue);
};

#endif /* CAN_H_ */
