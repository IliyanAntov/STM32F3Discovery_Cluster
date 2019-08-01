/*
 * blinker_sound.cpp
 *
 *  Created on: Jul 31, 2019
 *      Author: SoMe0nE
 */

#include "blinker_sound.h"
#include <stm32f3xx_hal.h>

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim4;

BlinkerSound::BlinkerSound(){}

int BlinkerSound::currentIndex = 0;
int BlinkerSound::iterationCounter = 0;
int BlinkerSound::soundPitch = 1;
constexpr int BlinkerSound::sineWave[];

void BlinkerSound::BlinkerOnSound(){
	soundPitch = 1;
	StartSound();
}

void BlinkerSound::BlinkerOffSound(){
	soundPitch = 2;
	StartSound();
}

void BlinkerSound::StartSound(){
	currentIndex = 0;
	iterationCounter = 0;
}

void BlinkerSound::InterruptCall() {
	if(iterationCounter%soundPitch == 0){
		if (currentIndex <= sineWaveArrayLength)
		{
			htim4.Instance->CCR1 = sineWave[currentIndex];
			currentIndex += 1;
		}
		else {
			htim4.Instance->CCR1 = 50;
		}
	}
	iterationCounter++;
}


