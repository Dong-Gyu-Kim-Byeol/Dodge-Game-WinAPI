// CppGame.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
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
		otherSetKey(room.user[1]);		// 다른 플레이어 자동 키 입력
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

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
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

	// TODO: 여기에 코드를 입력합니다.

	// 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_CPPGAME, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);


	// 응용 프로그램 초기화를 수행합니다.
	// 
	// hWnd가 필요하여 함수를 메인에 넣었습니다.
	bool bInitInstance = TRUE;
	//BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
	//{
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

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

	// 초기 세팅
	room.userCount = 2;			// 플레이어 수
	me.userNumber = 1;			// 플레이어 식별 번호
	room.user[0].userNumber = me.userNumber;

	KeyInput key;
	GameProcess gameProcess(&room);
	GameGraphic gameGraphic(hInst, hWnd, &beforeRoom, &me);

	// 게임 창 이동 중에도 게임이 계속 진행되도록 스레드를 이용합니다.
	std::thread gameLoopThread(&GameLoop, gameProcess, gameGraphic, key);

	bool loop = 1;
	while (loop)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//종료 메시지가 들어오면 루프를 탈출, 프로그램 종료
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
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
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
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

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
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
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
		// 메뉴 선택을 구문 분석합니다.
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

// 정보 대화 상자의 메시지 처리기입니다.
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
