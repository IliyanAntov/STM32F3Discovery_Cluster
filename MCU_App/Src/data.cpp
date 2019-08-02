/*
 * Data.cpp
 *
 *  Created on: Jul 30, 2019
 *      Author: SoMe0nE
 */

#include <data.h>

Data* Data::instance = 0;

Data::Data() {
	dataPtr[eLed0] = &led0;
	dataPtr[eLed1] = &led1;
	dataPtr[eLed2] = &led2;
	dataPtr[eLed3] = &led3;
	dataPtr[eLed4] = &led4;
	dataPtr[eLed5] = &led5;
	dataPtr[eLed6] = &led6;
	dataPtr[eMotorSpeed] = &motorSpeed;
	led0 = defaultValues[eLed0];
	led1 = defaultValues[eLed1];
	led2 = defaultValues[eLed2];
	led3 = defaultValues[eLed3];
	led4 = defaultValues[eLed4];
	led5 = defaultValues[eLed5];
	led6 = defaultValues[eLed6];
	motorSpeed = defaultValues[eMotorSpeed];
}

Data* Data::getInstance(){
	if(instance == 0){
		instance = new Data();
	}
	return instance;
}

uint32_t Data::Read(Signals s) {
	return *(dataPtr[s]);
}

void Data::Write(Signals s, uint32_t value) {
	*dataPtr[s] = value;
}
