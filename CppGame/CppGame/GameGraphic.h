#pragma once

#include "CppGame.h"
//const int limitedClientScreenX = 1280;
//const int limitedClientScreenY = 720;
//const int playerBitmapSize = 58;
//const int bulletBitmapSize = 58;
//const float playerSize = playerBitmapSize * playerCorrection;
//const float bulletSize = bulletBitmapSize * bulletCorrection;

class GameGraphic
{
private:
	HBITMAP hBitmapBackBuffer, hBitmapBackground, hBitmapPlayer, hBitmapOtherPlayer, hBitmapBullet;
	HDC hdc;
	HDC backBufferDC, backgroundDC, playerDC, otherPlayerDC, bulletDC;
	HWND hWnd;
	HINSTANCE hInst;
	const struct Room *room;
	const struct User *me;

public:
	GameGraphic(HINSTANCE set_hInst, HWND set_hWnd, const struct Room *set_room, const struct User *set_me);
	~GameGraphic();
	void Draw();

private:
	void setDC();
	void setBitmap();
	void deleteBitmap();
	void deleteDC();
};