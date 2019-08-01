/*
 * can.cpp
 *
 *  Created on: Jul 30, 2019
 *      Author: SoMe0nE
 */

#include "can.h"
#include "CANSPI.h"
#include "data.h"

Can* Can::instance = 0;

Can::Can() { }

Can* Can::getInstance(){
	if(instance == 0){
		instance = new Can();
	}
	return instance;
}

void Can::Initialize(){
	CANSPI_Initialize();
}


void Can::ReadInput(){
	uCAN_MSG rxMessage;
	Data* data = Data::getInstance();
	if (CANSPI_Receive(&rxMessage)) {

		uint8_t messageId = rxMessage.frame.id;

		if(messageId == 10){
			SetSpeed(data, rxMessage.frame.data0);
		}
		else if(messageId == 20){
			SetLEDs(data, rxMessage.frame.data0);
		}
		else if(messageId == 30){
			SetHorn(data, rxMessage.frame.data0);
		}
	}

}

void Can::SetSpeed(Data* dataClass, uint8_t newValue){
	dataClass->Write(dataClass->eMotorSpeed, newValue);
}

void Can::SetLEDs(Data* dataClass, uint8_t newValue){
	for (int i = 0; i < 7; i++) {
		dataClass->Write((Data::Signals)i, (newValue & 1));
		newValue = newValue >> 1;
	}
}

void Can::SetHorn(Data* dataClass, uint8_t newValue){
	//TODO
}

