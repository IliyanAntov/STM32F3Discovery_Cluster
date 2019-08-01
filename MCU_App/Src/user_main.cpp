#include <user_main.h>
#include <data.h>

#include <stm32f3xx_hal.h>
#include <CANSPI.h>
#include <can.h>
#include <blinker_sound.h>

#include <ILI9341_STM32_Driver.h>
#include <ILI9341_GFX.h>

#include <snow_tiger.h>

extern SPI_HandleTypeDef hspi1;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
/*
 <Protocol>
 [b0][b1][b2][b3][b4][b5][b6][b7]

 [b0]:	LEDs state
 [0]: Left blinker LED (0 == off / 1 == on)
 [1]: Right blinker LED (0 == off / 1 == on)
 [2]: Engine cooling system LED (0 == off / 1 == on)
 [3]: Battery LED (0 == off / 1 == on)
 [4]: Headlights high beam LED (0 == off / 1 == on)
 [5]: Headlights low beam LED (0 == off / 1 == on)
 [6]: Battery LED (0 == off / 1 == on)
 [7]: Seatbelt LED (0 == off / 1 == on)
 [b1]: Speed
 [b2]: Horn
 */

#define speedometerDirectionPin GPIO_PIN_0
#define speedometerPulsePin GPIO_PIN_1

class DataHandler {
public:
	DataHandler();
	void ReadInput();
	void LightLeds();
	void SetSpeedometer();
	bool ledStates[8];

private:
	uint16_t stepsToGoal;
	uint16_t ledPins[8];
	float oldSpeed;
	float speed;
	uint8_t inputData[8];
	uint8_t messageId;
	uint32_t oldTime;
	void GetLedStates();
	void GetSpeed();
};

DataHandler dataHandler;

void user_setup(){
	Can* can = Can::getInstance();
	//can->Initialize();
	CANSPI_Initialize();
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_Base_Start_IT(&htim3);
	HAL_TIM_Base_Start_IT(&htim4);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
	ILI9341_Init();
}

void user_loop() {
	//Data data;
	//printf("%d", data.led0);

	//DataHandler dataHandler;
	dataHandler.ReadInput();
	dataHandler.LightLeds();

}

void interrupt()
{
	dataHandler.SetSpeedometer();
}

bool prevBlinker1 = dataHandler.ledStates[6];
bool prevBlinker2 = dataHandler.ledStates[7];

int f = 0;
float g = 0;
void BlinkerInterrupt()
{
	BlinkerSound::InterruptCall();
}

void tickHigh()
{
	int prevF = f;
	while(f - prevF < 4){
		if(f%2 == 0){
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);
		}
		else{
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_RESET);
		}
	}
}

void tickLow(){
	int prevF = f;
	while(f - prevF < 4){
			if(f%2 == 0){
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);
			}
			else{
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_RESET);
			}
		}
}

DataHandler::DataHandler() {
	this->messageId = 0;
	this->oldSpeed = 0;
	this->stepsToGoal = 0;
	this->speed = 0;
	this->oldSpeed = 0;
	this->oldTime = HAL_GetTick();
	this->ledPins[0] = GPIO_PIN_0;
	this->ledPins[1] = GPIO_PIN_1;
	this->ledPins[2] = GPIO_PIN_2;
	this->ledPins[3] = GPIO_PIN_3;
	this->ledPins[4] = GPIO_PIN_4;
	this->ledPins[5] = GPIO_PIN_5;
	this->ledPins[6] = GPIO_PIN_6;
	this->ledPins[7] = GPIO_PIN_7;
}

void DataHandler::ReadInput() {
	uCAN_MSG rxMessage;
	if (CANSPI_Receive(&rxMessage)) {
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);

		this->messageId = rxMessage.frame.id;
		if (this->messageId == 10 || this->messageId == 20
				|| this->messageId == 30) {
			this->inputData[0] = rxMessage.frame.data0;
			this->inputData[1] = rxMessage.frame.data1;
			this->inputData[2] = rxMessage.frame.data2;
			this->inputData[3] = rxMessage.frame.data3;
			this->inputData[4] = rxMessage.frame.data4;
			this->inputData[5] = rxMessage.frame.data5;
			this->inputData[6] = rxMessage.frame.data6;
			this->inputData[7] = rxMessage.frame.data7;
		}

		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_RESET);
	}
	if(		(dataHandler.ledStates[6] != prevBlinker1 && prevBlinker1 == 0) ||
			(dataHandler.ledStates[7] != prevBlinker2 && prevBlinker2 == 0)){
		BlinkerSound::BlinkerOnSound();
		prevBlinker1 = dataHandler.ledStates[6];
		prevBlinker2 = dataHandler.ledStates[7];
	}
	else if ((dataHandler.ledStates[6] != prevBlinker1 && prevBlinker1 == 1) ||
			(dataHandler.ledStates[7] != prevBlinker2 && prevBlinker2 == 1)){
		BlinkerSound::BlinkerOffSound();
		prevBlinker1 = dataHandler.ledStates[6];
		prevBlinker2 = dataHandler.ledStates[7];

	}

}

void DataHandler::LightLeds() {
	if (this->messageId == 30) {
		this->GetLedStates();
	}
	for (int i = 0; i < 8; i++) {
		if (this->ledStates[i]) {
			HAL_GPIO_WritePin(GPIOD, this->ledPins[i], GPIO_PIN_SET);
		} else {
			HAL_GPIO_WritePin(GPIOD, this->ledPins[i], GPIO_PIN_RESET);
		}
	}
}

void DataHandler::GetLedStates() {
	uint8_t input = this->inputData[0];
	for (int i = 0; i < 8; i++) {
		this->ledStates[i] = (input & 1);
		input = input >> 1;
	}
}

float map(float x, float in_min, float in_max, float out_min, float out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void DataHandler::GetSpeed() {
	this->speed = (int) map(this->inputData[0], 0, 250, 0, 310);
}

int j = 0;
void DataHandler::SetSpeedometer() {
	if (this->messageId == 10) {
		this->GetSpeed();
	}

	int stepsToGoal = (int) ((this->speed - this->oldSpeed) / 0.5);
	if (stepsToGoal < 0) {
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
	} else {
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
	}
	int rotationSpeed = 2;
	if (stepsToGoal == 0) {
		return;
	} else {
		rotationSpeed = (int) (1 / (float) stepsToGoal) * 1000 + 2;
	}

	if (j % rotationSpeed == 0) {
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);
		if (stepsToGoal > 0) {
			this->oldSpeed += 0.5;
		} else {
			this->oldSpeed -= 0.5;
		}
	} else {
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
	}
	j++;

}
