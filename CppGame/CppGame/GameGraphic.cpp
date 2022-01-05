#include "stdafx.h"

#pragma comment(lib, "msimg32.lib")
#include <time.h>
#include <string>

#include "GameGraphic.h"


GameGraphic::GameGraphic(HINSTANCE set_hInst, HWND set_hWnd, const struct Room *set_room, const struct User *set_me)
{
	hInst = set_hInst;
	hWnd = set_hWnd;
	room = set_room;
	me = set_me;

	setDC();
	setBitmap();
}

GameGraphic::~GameGraphic()
{
	deleteBitmap();
	deleteDC();
}

void GameGraphic::setDC()
{
	hdc = GetDC(hWnd);

	backBufferDC = CreateCompatibleDC(hdc);

	backgroundDC = CreateCompatibleDC(backBufferDC);
	playerDC = CreateCompatibleDC(backBufferDC);
	otherPlayerDC = CreateCompatibleDC(backBufferDC);
	bulletDC = CreateCompatibleDC(backBufferDC);

	SetTextColor(backBufferDC, RGB(0, 0, 0));
	SetBkColor(backBufferDC, RGB(255, 255, 255));
}

void GameGraphic::deleteDC()
{
	ReleaseDC(hWnd, hdc);

	DeleteDC(backBufferDC);

	DeleteDC(backgroundDC);
	DeleteDC(playerDC);
	DeleteDC(otherPlayerDC);
	DeleteDC(bulletDC);
}

void GameGraphic::setBitmap()
{
	hBitmapBackBuffer = CreateCompatibleBitmap(hdc, limitedClientScreenX, limitedClientScreenY);

	hBitmapBackground = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP5));
	hBitmapPlayer = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP3));
	hBitmapOtherPlayer = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
	hBitmapBullet = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP4));

	SelectObject(backBufferDC, hBitmapBackBuffer);

	SelectObject(backgroundDC, hBitmapBackground);
	SelectObject(playerDC, hBitmapPlayer);
	SelectObject(otherPlayerDC, hBitmapOtherPlayer);
	SelectObject(bulletDC, hBitmapBullet);
}

void GameGraphic::deleteBitmap()
{
	DeleteObject(hBitmapBackBuffer);

	DeleteObject(hBitmapBackground);
	DeleteObject(hBitmapPlayer);
	DeleteObject(hBitmapOtherPlayer);
	DeleteObject(hBitmapBullet);
}

void GameGraphic::Draw()
{
	BitBlt(backBufferDC, 0, 0, limitedClientScreenX, limitedClientScreenY, backgroundDC, 0, 0, SRCCOPY);

	for (int i = 0; i < room->bulletCount; i++)
	{
		TransparentBlt(backBufferDC, (int)room->bullet[i].position.x, (int)room->bullet[i].position.y, (int)bulletSize, (int)bulletSize,
			bulletDC, 0, 0, bulletBitmapSize, bulletBitmapSize, RGB(255, 255, 255));
	}

	for (int i = 0; i < room->userCount; i++)
	{
		if (!room->user[i].bCollision)
		{
			if (room->user[i].userNumber == me->userNumber)
			{
				TransparentBlt(backBufferDC, (int)room->user[i].position.x, (int)room->user[i].position.y, (int)playerSize, (int)playerSize,
					playerDC, 0, 0, playerBitmapSize, playerBitmapSize, RGB(96, 96, 64));
			}
			else
			{
				TransparentBlt(backBufferDC, (int)room->user[i].position.x, (int)room->user[i].position.y, (int)playerSize, (int)playerSize,
					otherPlayerDC, 0, 0, playerBitmapSize, playerBitmapSize, RGB(96, 96, 64));
			}
		}
	}

	for (int i = 0; i < room->userCount; i++)
	{
		if (room->user[i].bCollision && me->userNumber == room->user[i].userNumber)
		{
			std::wstring score = std::to_wstring(room->user[i].score);
			std::wstring statusTemp = L" 생존 시간 : " + score + L"    다시 시작하시려면 'F5'키를 눌러 주세요! ";

			wchar_t status[50];
			wcscpy_s(status, statusTemp.c_str());
			TextOutW(backBufferDC, 450, 300, status, wcslen(status));
		}
		else if(!room->user[i].bCollision)
		{
			std::wstring bulletCount = std::to_wstring(room->bulletCount);
			std::wstring score = std::to_wstring(room->scoreTemp);
			std::wstring statusTemp = L" 시간 : " + score + L", 장애물 개수 : " + bulletCount + L"  ";

			wchar_t status[50];
			wcscpy_s(status, statusTemp.c_str());
			TextOutW(backBufferDC, 500, 50, status, wcslen(status));
		}
	}
	BitBlt(hdc, 0, 0, limitedClientScreenX, limitedClientScreenY, backBufferDC, 0, 0, SRCCOPY);
}