#pragma once

#include "resource.h"

const int limitedClientScreenX = 1280;
const int limitedClientScreenY = 720;

const int playerBitmapSize = 58;
const int bulletBitmapSize = 58;
const float playerCorrection = 3.0 / 4.0;
const float bulletCorrection = 1.0 / 4.0;
const float playerSize = playerBitmapSize * playerCorrection;
const float bulletSize = bulletBitmapSize * bulletCorrection;

const int limitedUserCount = 2;
const int limitedBulletCount = 100;

const int userNameLength = 15;

struct Position
{
	float x;
	float y;
};

typedef struct User
{
	char userName[userNameLength];
	int userNumber;
	int userIP;
	char hopeMode;
	int roomNumder;
	struct Position position;
	bool bCollision;
	float score;
	bool bKeyInput;
	bool bUpKey;
	bool bDownKey;
	bool bLeftKey;
	bool bRightKey;
	bool bRestart;
	char screenNumber;
}User;

typedef struct Bullet
{
	struct Position position;
	float direction;
}Bullet;

typedef struct Room
{
	int number;
	char playing = 0;
	int userCount = 0;
	float scoreTemp;
	struct User user[limitedUserCount];
	int bulletCount;
	struct Bullet bullet[limitedBulletCount];
}Room;

typedef struct UserScore
{
	int userNumber;
	float score;
	char userName[userNameLength];
}UserScore;

typedef struct Rank
{
	struct UserScore userScore[10];
}Rank;