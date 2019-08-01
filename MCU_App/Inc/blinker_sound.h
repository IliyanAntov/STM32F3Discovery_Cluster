/*
 * blinker_sound.h
 *
 *  Created on: Jul 31, 2019
 *      Author: SoMe0nE
 */

#ifndef BLINKER_SOUND_H_
#define BLINKER_SOUND_H_

class BlinkerSound{
	public:
		static void BlinkerOnSound();
		static void BlinkerOffSound();
		static void InterruptCall();
	private:
		BlinkerSound();
		static void StartSound();
		static int iterationCounter;
		static int soundPitch;
		static int currentIndex;
		static constexpr int sineWave[200] = {50,52,53,55,56,58,59,61,62,64,65,67,68,70,71,73,74,75,77,78,79,81,82,83,84,85,86,88,89,90,90,91,92,93,94,95,95,96,96,97,98,98,98,99,99,99,100,100,100,100,100,100,100,100,100,99,99,99,98,98,98,97,96,96,95,95,94,93,92,91,90,90,89,88,86,85,84,83,82,81,79,78,77,75,74,73,71,70,68,67,65,64,62,61,59,58,56,55,53,52,50,48,47,45,44,42,41,39,38,36,35,33,32,30,29,27,26,25,23,22,21,19,18,17,16,15,14,12,11,10,10,9,8,7,6,5,5,4,4,3,2,2,2,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,2,2,2,3,4,4,5,5,6,7,8,9,10,10,11,12,14,15,16,17,18,19,21,22,23,25,26,27,29,30,32,33,35,36,38,39,41,42,44,45,47,48};
		static const int sineWaveArrayLength = 200;
};

#endif /* BLINKER_SOUND_H_ */
