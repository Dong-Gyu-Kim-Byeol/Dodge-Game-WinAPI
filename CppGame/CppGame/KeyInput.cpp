#include "stdafx.h"

#include <Windows.h>

#include "CppGame.h"
#include "KeyInput.h"


void KeyInput::SetKey(struct Room &rRoom, int myNumber)
{
	for (int i = 0; i < rRoom.userCount; i++)
	{
		if (rRoom.user[i].userNumber == myNumber)
		{
			if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
			{
				rRoom.user[i].bRightKey = TRUE;
			}
			if (GetAsyncKeyState(VK_LEFT) & 0x8000)
			{
				rRoom.user[i].bLeftKey = TRUE;
			}
			if (GetAsyncKeyState(VK_UP) & 0x8000)
			{
				rRoom.user[i].bUpKey = TRUE;
			}
			if (GetAsyncKeyState(VK_DOWN) & 0x8000)
			{
				rRoom.user[i].bDownKey = TRUE;
			}
			if (rRoom.playing == 0)
			{
				if (GetAsyncKeyState(VK_F5) & 0x8000)
				{
					rRoom.user[i].bRestart = TRUE;
				}
			}
			rRoom.user[i].bKeyInput = TRUE;
		}
	}
}


// 다른 플레이어 자동 키 입력
///*
#include <time.h>
void otherSetKey(struct User &rUser)
{
	srand((unsigned int)time(NULL));
	//if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	//{
	rUser.bRightKey = rand() % 2;
	//}
	//if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	//{
	rUser.bLeftKey = rand() % 2;
	//}
	//if (GetAsyncKeyState(VK_UP) & 0x8000)
	//{
	rUser.bUpKey = rand() % 2;
	//}
	//if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	//{
	rUser.bDownKey = rand() % 2;
	//}
	//if (GetAsyncKeyState(VK_F5) & 0x8000)
	//{
	rUser.bRestart = TRUE;
	//}
	rUser.bKeyInput = TRUE;
}
//*/