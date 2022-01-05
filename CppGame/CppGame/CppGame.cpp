// CppGame.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"

#include <thread>
#include <iostream>

#include "CppGame.h"
#include "KeyInput.h"
#include "GameProcess.h"
#include "GameGraphic.h"

int gameLoop = 1;
struct Room room;
struct Room beforeRoom;
struct User me;

void GameLoop(GameProcess &gameProcess, GameGraphic &gameGraphic, KeyInput &key)
{
	while (gameLoop)
	{
		key.SetKey(room, me.userNumber);
		otherSetKey(room.user[1]);		// �ٸ� �÷��̾� �ڵ� Ű �Է�
		if (room.playing == 0)
		{
			bool bRestart = TRUE;
			for (int i = 0; i < room.userCount; i++)
			{
				if (room.user[i].bRestart == FALSE)
				{
					bRestart = FALSE;
				}
			}

			if (bRestart == TRUE)
			{
				gameProcess.SetGame();
			}
		}

		beforeRoom = room;
		std::thread tProcess([&]() {gameProcess.Process(); });
		std::thread tGraphic([&]() {gameGraphic.Draw(); });

		tProcess.join();
		tGraphic.join();
	}
}

#define MAX_LOADSTRING 100

// ���� ����:
HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: ���⿡ �ڵ带 �Է��մϴ�.

	// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_CPPGAME, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);


	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
	// 
	// hWnd�� �ʿ��Ͽ� �Լ��� ���ο� �־����ϴ�.
	bool bInitInstance = TRUE;
	//BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
	//{
	hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		bInitInstance = FALSE;
		//return FALSE;
	}
	else
	{
		ShowWindow(hWnd, nCmdShow);
		UpdateWindow(hWnd);

		bInitInstance = TRUE;
		//return TRUE;
	}
	//}
	if (!bInitInstance) //!InitInstance(hInstance, nCmdShow)
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CPPGAME));

	MSG msg;

	// �ʱ� ����
	room.userCount = 2;			// �÷��̾� ��
	me.userNumber = 1;			// �÷��̾� �ĺ� ��ȣ
	room.user[0].userNumber = me.userNumber;

	KeyInput key;
	GameProcess gameProcess(&room);
	GameGraphic gameGraphic(hInst, hWnd, &beforeRoom, &me);

	// ���� â �̵� �߿��� ������ ��� ����ǵ��� �����带 �̿��մϴ�.
	std::thread gameLoopThread(&GameLoop, gameProcess, gameGraphic, key);

	bool loop = 1;
	while (loop)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//���� �޽����� ������ ������ Ż��, ���α׷� ����
			if (msg.message == WM_QUIT)
			{
				loop = FALSE;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	gameLoop = 0;
	gameLoopThread.join();

	return (int)msg.wParam;
}



//
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CPPGAME));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_CPPGAME);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����:  �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND  - ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT    - �� â�� �׸��ϴ�.
//  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{


	switch (message)
	{

	case WM_CREATE:
	{
		MoveWindow(hWnd, 0, 0, 1297, 780, TRUE);
	}
	break;
	case WM_GETMINMAXINFO:
	{
		((MINMAXINFO*)lParam)->ptMaxTrackSize.x = 1297;
		((MINMAXINFO*)lParam)->ptMaxTrackSize.y = 780;
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 1297;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 780;
	}
	break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// �޴� ������ ���� �м��մϴ�.
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
