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
	bool Process();		// ���� �������� ��ü�� ��ġ, �浹 ���θ� �����ϰ� pRoom�� �����մϴ�.
	void SetGame();		// ���� �ð�, ��ü�� ���� ��ġ�� �����մϴ�.

private:
	void checkTime();
	void shiftPlayer();
	void shiftBullet();
	void addBullet();
	void checkCollision();
	bool gameEnd();
};