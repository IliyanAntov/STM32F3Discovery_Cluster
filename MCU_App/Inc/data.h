/*
 * data.h
 *
 *  Created on: Jul 30, 2019
 *      Author: SoMe0nE
 */

#ifndef DATA_H_
#define DATA_H_

#include <stdint.h>

class Data{
	public:
		enum Signals
		{
			eLed0 = 0,
			eLed1,
			eLed2,
			eLed3,
			eLed4,
			eLed5,
			eLed6,
			eMotorSpeed,
			eCount
		};

		static Data* getInstance();
		uint32_t Read(Signals s);
		void Write(Signals s, uint32_t value);

	private:
		Data();
		uint32_t led0;
		uint32_t led1;
		uint32_t led2;
		uint32_t led3;
		uint32_t led4;
		uint32_t led5;
		uint32_t led6;
		uint32_t motorSpeed;

		static Data* instance;
		uint32_t *dataPtr[eCount];
		static constexpr uint32_t defaultValues[eCount] = { 0, 0, 0, 0, 0, 0, 0, 0 };
};
#endif
