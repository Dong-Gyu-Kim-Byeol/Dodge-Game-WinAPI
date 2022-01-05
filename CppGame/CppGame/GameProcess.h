#pragma once

#include <chrono>

#include "CppGame.h"
//const int limitedClientScreenX = 1280;
//const int limitedClientScreenY = 720;
//const float playerCorrection = 3.0 / 4.0;
//const float bulletCorrection = 1.0 / 4.0;
//const float playerSize = playerBitmapSize * playerCorrection;
//const float bulletSize = bulletBitmapSize * bulletCorrection;
//const int limitedUserCount = 2;
//const int limitedBulletCount = 100;

class GameProcess
{
private:
	std::chrono::system_clock::time_point startTime;
	std::chrono::milliseconds scoreTimeMilliSec;

	std::chrono::system_clock::time_point processLastTime;
	std::chrono::milliseconds processLastTimeMilliSec;
	std::chrono::system_clock::time_point addBulletLastTime;
	std::chrono::milliseconds addBulletLastTimeMilliSec;

	struct Room *pRoom = NULL;

public:
	GameProcess(struct Room *pointerRoom);
	bool Process();		// 다음 프레임의 물체의 위치, 충돌 여부를 결정하고 pRoom에 저장합니다.
	void SetGame();		// 시작 시간, 물체의 시작 위치를 설정합니다.

private:
	void checkTime();
	void shiftPlayer();
	void shiftBullet();
	void addBullet();
	void checkCollision();
	bool gameEnd();
};