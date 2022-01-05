#include "stdafx.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "GameProcess.h"

const float playerBitmapBlankSpace = 2.0;
const float playerBlankSpace = playerBitmapBlankSpace * playerCorrection;

const float bulletBitmapBlankSpace = 1.0 * bulletCorrection;
const float bulletBlankSpace = bulletBitmapBlankSpace * bulletCorrection;

static const double framePerSecond = 60.0;
static const double reScreenTimeMilliSec = 1000.0 / framePerSecond;
const int addBulletTimeMilliSec = 3000;
const int startBulletCount = 50;

const float playerSpeed = 5.0;
const float playerMaxPositionX = limitedClientScreenX - playerSize;
const float playerMaxPositionY = limitedClientScreenY - playerSize;

const float bulletSpped = 2.0;
const float bulletMaxPositionX = limitedClientScreenX - bulletSize;
const float bulletMaxPositionY = limitedClientScreenY - bulletSize;

const float safeZoneSize = 300.0;
const float safeZoneStartPsitionX = (limitedClientScreenX - safeZoneSize) / 2.0;
const float safeZoneStartPsitionY = (limitedClientScreenY - safeZoneSize) / 2.0;
const float safeZoneEndPsitionX = safeZoneStartPsitionX + safeZoneSize;
const float safeZoneEndPsitionY = safeZoneStartPsitionY + safeZoneSize;

const float f_PI = (float)M_PI;

GameProcess::GameProcess(struct Room *pointerRoom)
{
	pRoom = pointerRoom;
	SetGame();
}

void GameProcess::SetGame()
{
	startTime = std::chrono::system_clock::now();
	processLastTime = std::chrono::system_clock::now();
	addBulletLastTime = std::chrono::system_clock::now();

	pRoom->playing = 1;
	pRoom->bulletCount = startBulletCount;

	// set bullet
	srand((unsigned int)time(NULL));
	for (int i = 0; i < pRoom->bulletCount; i++)
	{
		pRoom->bullet[i].position.x = (float)rand() / RAND_MAX * bulletMaxPositionX;
		pRoom->bullet[i].position.y = (float)rand() / RAND_MAX * bulletMaxPositionY;
		pRoom->bullet[i].direction = (float)rand() / RAND_MAX * 2.0f * f_PI;

		// safe zone
		if (pRoom->bullet[i].position.x > safeZoneStartPsitionX && pRoom->bullet[i].position.x < safeZoneEndPsitionX)
		{
			int temp = rand() % 2;
			if (temp)
			{
				pRoom->bullet[i].position.x += safeZoneSize;
				if (pRoom->bullet[i].position.x > bulletMaxPositionX)
				{
					pRoom->bullet[i].position.x = bulletMaxPositionX;
				}
			}
			else
			{
				pRoom->bullet[i].position.x -= safeZoneSize;
				if (pRoom->bullet[i].position.x < 0)
				{
					pRoom->bullet[i].position.x = 0;
				}
			}
		}

		if (pRoom->bullet[i].position.y > safeZoneStartPsitionY && pRoom->bullet[i].position.y < safeZoneEndPsitionY)
		{
			int temp = rand() % 2;
			if (temp)
			{
				pRoom->bullet[i].position.y += safeZoneSize;
				if (pRoom->bullet[i].position.y > bulletMaxPositionY)
				{
					pRoom->bullet[i].position.y = bulletMaxPositionY;
				}
			}
			else
			{
				pRoom->bullet[i].position.y -= safeZoneSize;
				if (pRoom->bullet[i].position.y < 0)
				{
					pRoom->bullet[i].position.y = 0;
				}
			}
		}
	}

	// set player
	struct Position startPosition;
	startPosition.x = safeZoneStartPsitionX + safeZoneSize / 2.0f - (pRoom->userCount - 1) * playerSize;
	startPosition.y = safeZoneStartPsitionY + safeZoneSize / 2.0f - playerSize / 2.0f;
	for (int userTemp = 0; userTemp < pRoom->userCount; userTemp++)
	{
		pRoom->user[userTemp].screenNumber = 4;
		pRoom->user[userTemp].bRestart = FALSE;
		pRoom->user[userTemp].bCollision = FALSE;
		pRoom->user[userTemp].position.x = startPosition.x;
		pRoom->user[userTemp].position.y = startPosition.y;
		startPosition.x += playerSize;
	}
}

bool GameProcess::Process()
{
	checkTime();
	addBullet();
	shiftPlayer();
	shiftBullet();
	checkCollision();
	if (gameEnd())
	{
		for (int userTemp = 0; userTemp < pRoom->userCount; userTemp++)
		{
			pRoom->user[userTemp].screenNumber = 2;
		}
		pRoom->playing = 0;
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

// 이전 프레임으로부터 reScreenTimeMilliSec 만큼 시간이 흐를 때까지 대기합니다.
void GameProcess::checkTime()
{
	processLastTimeMilliSec = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - processLastTime);
	addBulletLastTimeMilliSec = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - addBulletLastTime);

	while (processLastTimeMilliSec.count() < reScreenTimeMilliSec)
	{
		processLastTimeMilliSec = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - processLastTime);
	}
	processLastTime = std::chrono::system_clock::now();

	scoreTimeMilliSec = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - startTime);
	pRoom->scoreTemp = scoreTimeMilliSec.count() / 1000.0f;
}

// 각 방향키 마다 TRUE면 playerSpeed 만큼 이동합니다.
// 0 < x < playerMaxPositionX, 0 < y < playerMaxPositionY
void GameProcess::shiftPlayer()
{
	for (int userTemp = 0; userTemp < pRoom->userCount; userTemp++)
	{
		if (!pRoom->user[userTemp].bCollision)
		{
			if (pRoom->user[userTemp].bKeyInput == TRUE)
			{
				if (pRoom->user[userTemp].bRightKey == TRUE)
				{
					pRoom->user[userTemp].position.x += playerSpeed;
					pRoom->user[userTemp].bRightKey = FALSE;
				}
				if (pRoom->user[userTemp].bLeftKey == TRUE)
				{
					pRoom->user[userTemp].position.x -= playerSpeed;
					pRoom->user[userTemp].bLeftKey = FALSE;
				}
				if (pRoom->user[userTemp].bUpKey == TRUE)
				{
					pRoom->user[userTemp].position.y -= playerSpeed;
					pRoom->user[userTemp].bUpKey = FALSE;
				}
				if (pRoom->user[userTemp].bDownKey == TRUE)
				{
					pRoom->user[userTemp].position.y += playerSpeed;
					pRoom->user[userTemp].bDownKey = FALSE;
				}
				pRoom->user[userTemp].bKeyInput = FALSE;
			}

			// limit
			if (pRoom->user[userTemp].position.x > playerMaxPositionX)
			{
				pRoom->user[userTemp].position.x = playerMaxPositionX;
			}
			else if (pRoom->user[userTemp].position.x < 0)
			{
				pRoom->user[userTemp].position.x = 0;
			}

			if (pRoom->user[userTemp].position.y > playerMaxPositionY)
			{
				pRoom->user[userTemp].position.y = playerMaxPositionY;
			}
			else if (pRoom->user[userTemp].position.y < 0)
			{
				pRoom->user[userTemp].position.y = 0;
			}
		}
	}
}

// 장애물을 각 방향으로 bulletSpped 만큼 이동하고 창 경계에서 반사합니다.
// 0 < x < bulletMaxPositionX, 0 < y < bulletMaxPositionY
void GameProcess::shiftBullet()
{
	for (int i = 0; i < pRoom->bulletCount; i++)
	{
		float xValue = cos(pRoom->bullet[i].direction) * bulletSpped;  // x 증가값
		float yValue = sin(pRoom->bullet[i].direction) * bulletSpped;  // y 증가값
		pRoom->bullet[i].position.x += xValue;
		pRoom->bullet[i].position.y -= yValue;

		if (pRoom->bullet[i].position.x > bulletMaxPositionX)
		{
			pRoom->bullet[i].position.x = bulletMaxPositionX;
			if (yValue > 0)
			{
				// 0 < direction < 90
				// 90 + 90 - 60(direction) = 120
				// M_PI / 2 + (pRoom->bullet[i].direction - M_PI / 2);
				pRoom->bullet[i].direction = f_PI - pRoom->bullet[i].direction;
			}
			else if (yValue == 0)
			{
				// direction == 0
				pRoom->bullet[i].direction = f_PI;
			}
			else
			{
				// 270 < direction < 360(0)
				// 270 - (300(direction) - 270) = 240
				// 3 * M_PI / 2 - (pRoom->bullet[i].direction - 3 * M_PI / 2);
				pRoom->bullet[i].direction = 3 * f_PI - pRoom->bullet[i].direction;
			}
		}
		else if (pRoom->bullet[i].position.x < 0)
		{
			pRoom->bullet[i].position.x = 0;
			if (yValue > 0)
			{
				// 90 < direction < 180
				// 90 - (120(direction) - 90) = 60
				pRoom->bullet[i].direction = f_PI - pRoom->bullet[i].direction;
			}
			else if (yValue == 0)
			{
				// direction == 180
				pRoom->bullet[i].direction = 0;
			}
			else
			{
				// 180 < direction < 270
				// 270 + (270 - 200(direction)) = 340
				pRoom->bullet[i].direction = 3 * f_PI - pRoom->bullet[i].direction;
			}
		}

		if (pRoom->bullet[i].position.y > bulletMaxPositionY)
		{
			pRoom->bullet[i].position.y = bulletMaxPositionY;
			if (xValue > 0)
			{
				// 270 < direction < 360
				// 360 - 330(direction) = 30
				pRoom->bullet[i].direction = 2 * f_PI - pRoom->bullet[i].direction;
			}
			else if (xValue == 0)
			{
				// direction == 270
				pRoom->bullet[i].direction = f_PI / 2;
			}
			else
			{
				// 180 < direction < 270
				// 180 - (200(direction) - 180) = 160
				pRoom->bullet[i].direction = 2 * f_PI - pRoom->bullet[i].direction;
			}
		}
		else if (pRoom->bullet[i].position.y < 0)
		{
			pRoom->bullet[i].position.y = 0;
			if (xValue > 0)
			{
				// 0 < direction < 90
				// 360 - 60(direction) = 300
				pRoom->bullet[i].direction = 2 * f_PI - pRoom->bullet[i].direction;
			}
			else if (xValue == 0)
			{
				// direction == 90
				pRoom->bullet[i].direction = 3 * f_PI / 2;
			}
			else
			{
				// 90 < direction < 180
				// 180 + (180 - 120(direction)) = 250
				pRoom->bullet[i].direction = 2 * f_PI - pRoom->bullet[i].direction;
			}
		}
	}
}

// addBulletTimeMilliSec 마다 장애물을 추가할지 랜덤으로 결정하고
// 랜덤한 창의 꼭지점에서 랜덤한 방향으로 장애물을 추가합니다.
void GameProcess::addBullet()
{
	srand((unsigned int)time(NULL));

	if (addBulletLastTimeMilliSec.count() > addBulletTimeMilliSec && pRoom->bulletCount < limitedBulletCount)
	{
		int i = rand() % 5;
		switch (i)
		{
		case 0:
			break;
		case 1:
			pRoom->bullet[pRoom->bulletCount].position.x = bulletMaxPositionX;
			pRoom->bullet[pRoom->bulletCount].position.y = 0;
			break;
		case 2:
			pRoom->bullet[pRoom->bulletCount].position.x = 0;
			pRoom->bullet[pRoom->bulletCount].position.y = 0;
			break;
		case 3:
			pRoom->bullet[pRoom->bulletCount].position.x = 0;
			pRoom->bullet[pRoom->bulletCount].position.y = bulletMaxPositionY;
			break;
		case 4:
			pRoom->bullet[pRoom->bulletCount].position.x = bulletMaxPositionX;
			pRoom->bullet[pRoom->bulletCount].position.y = bulletMaxPositionY;
			break;
		}
		if (i)
		{
			pRoom->bullet[pRoom->bulletCount].direction = (float)rand() / RAND_MAX * 2 * f_PI;
			pRoom->bulletCount++;
		}
		addBulletLastTime = std::chrono::system_clock::now();
	}
}

// 장애물과 플레이어의 충돌을 결정하고 충돌했을 때의 시간을 충돌한 User 구조체의 score에 저장합니다.
void GameProcess::checkCollision()
{
	float bulletRadiusCorrection = 1.0f * 1.0f / 4.0f;
	float playerRadiusCorrection = playerBlankSpace + 1.0f;
	float bulletRadius = bulletSize / 2.0f - bulletRadiusCorrection;
	float playerRadius = playerSize / 2.0f - playerRadiusCorrection;
	float R = bulletRadius + playerRadius;

	float bulletBitmapRadius = bulletSize / 2.0f;
	float playerBitmapRadius = playerSize / 2.0f;

	struct Position bulletCenter;
	struct Position playerCenter;

	for (int userTemp = 0; userTemp < pRoom->userCount; userTemp++)
	{
		if (!pRoom->user[userTemp].bCollision)
		{
			playerCenter.x = pRoom->user[userTemp].position.x + playerBitmapRadius;
			playerCenter.y = pRoom->user[userTemp].position.y + playerBitmapRadius;

			for (int bulletTemp = 0; bulletTemp < pRoom->bulletCount; bulletTemp++)
			{
				bulletCenter.x = pRoom->bullet[bulletTemp].position.x + bulletBitmapRadius;
				bulletCenter.y = pRoom->bullet[bulletTemp].position.y + bulletBitmapRadius;

				float distans = sqrt(powf(bulletCenter.x - playerCenter.x, 2.0f) + powf(bulletCenter.y - playerCenter.y, 2.0f));

				if (R >= distans)
				{
					pRoom->user[userTemp].bCollision = TRUE;

					scoreTimeMilliSec = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - startTime);
					pRoom->scoreTemp = scoreTimeMilliSec.count() / 1000.0f;
					pRoom->user[userTemp].score = pRoom->scoreTemp;
				}
			}

		}
	}

}

// 모든 유저가 충돌하면 TRUE를 반환합니다.
bool GameProcess::gameEnd()
{
	bool result = TRUE;
	for (int userTemp = 0; userTemp < pRoom->userCount; userTemp++)
	{
		if (pRoom->user[userTemp].bCollision == FALSE)
		{
			result = FALSE;
		}
	}
	
	return result;
}
