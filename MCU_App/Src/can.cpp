/*
 * can.cpp
 *
 *  Created on: Jul 30, 2019
 *      Author: SoMe0nE
 */

#include "can.h"
#include <CANSPI.h>
#include "data.h"

Can::Can() {
	// TODO Auto-generated constructor stub

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

void SetSpeed(Data* dataClass, uint8_t newValue){
	dataClass->Write(dataClass->eMotorSpeed, newValue);
}

void SetLEDs(Data* dataClass, uint8_t newValue){
	for (int i = 0; i < 8; i++) {
		dataClass->Write((Data::Signals)i, (newValue & 1));
		newValue = newValue >> 1;
	}
}

void SetHorn(Data* dataClass, uint8_t newValue){
	//TODO
}

