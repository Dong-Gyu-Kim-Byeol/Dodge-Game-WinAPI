# Dodge-Game-WinAPI   
   
2019년 6월에 만든 닷지 게임의 모작입니다.
   
1. 클래스(함수) 설명   
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
}   
   
GameGraphic 생성자   
 : 필요한 데이터를 받고 setDC()와 setBitmap()를 실행시킵니다.   
   
~GameGraphic() 소멸자   
 : deleteBitmap()과 deleteDC()을 호출하여 사용한 DC와 비트맵을 제거합니다.   
   
void Draw()   
 : 1. 장애물들을 화면에 위치에 맞게 표시합니다.   
   2. 유저가 충돌하지 않았으면 유저를 위치에 맞게 표시합니다.   
   3. 모든 유저가 충돌되지 않았으면 화면 위에 상태표시줄을 표시합니다.   
      자신이 충돌되었으면 화면 중앙에 점수와 안내문을 표시합니다.   
   
void setDC()   
 : DC를 만들고 상태표시줄의 글자 색, 글자 배경색을 설정합니다.   
   
void setBitmap()   
 : 각 비트맵 파일들을 읽어서 각 DC에 복사합니다.   
    ex) 장애물 비트맵을 장애물 DC에 복사합니다.   
   
void deleteBitmap()   
 : 사용한 비트맵을 제거합니다.   
   
void deleteDC()   
: 사용한 DC를 제거합니다.   
   
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
    bool Process();   
    void SetGame();    
   
private:   
    void checkTime();   
    void shiftPlayer();   
    void shiftBullet();   
    void addBullet();   
    void checkCollision();   
    bool gameEnd();   
}   
   
GameProcess(struct Room *pointerRoom)   
 : 방 구조체의 주소를 받아서 pRoom에 저장하고 SetGame()을 호출합니다.   
   
bool Process()   
 : checkTime(), addBullet(), shiftPlayer(), shiftBullet(), checkCollision(), gameEnd()을   
   호출하여 다음 프레임의 물체의 위치, 충돌 여부를 결정하고 pRoom에 저장합니다.   
   게임이 끝나면 FALSE,   
   게임이 끝나지 않으면 “pRoom->playing = 0”을 실행한 후 TRUE를 반환합니다.   
   
void SetGame();   
 : 시작 시간, 물체의 시작 위치를 설정합니다.   
   
void checkTime()   
 : 이전 프레임으로부터 reScreenTimeMilliSec 만큼 시간이 흐를 때까지 대기합니다.   
   
void shiftPlayer()   
 : 각 방향키 마다 TRUE면 playerSpeed 만큼 이동합니다.   
   0 < x < playerMaxPositionX, 0 < y < playerMaxPositionY   
   
void shiftBullet()   
 : 장애물을 각 방향으로 bulletSpped 만큼 이동하고 창 경계에서 반사합니다.   
   0 < x < bulletMaxPositionX, 0 < y < bulletMaxPositionY   
   
void addBullet()   
 : addBulletTimeMilliSec마다 장애물을 추가할지 랜덤으로 결정하고   
   랜덤한 창의 꼭지점에서 랜덤한 방향으로 장애물을 추가합니다.   
   
void checkCollision()   
 : 장애물과 플레이어의 충돌을 결정하고   
   충돌했을 때의 시간을 충돌한 User 구조체의 score에 저장합니다.   
   
bool gameEnd()   
 : 모든 유저가 충돌하면 TRUE를 반환합니다.   
   
class KeyInput   
{   
public:   
    void SetKey(struct Room &rRoom, int myNumber);   
}   
   
void SetKey(struct Room &rRoom, int myNumber)   
 : rRoom에서 myNumber와 유저 번호가 같은 rRoom.user 구조체에   
  키입력 정보를 저장합니다.   
   
void GameLoop(GameProcess &gameProcess, GameGraphic &gameGraphic, KeyInput &key)   
 : 창을 종료할 때까지 반복되는 함수입니다.   
   GameProcess::Process()와 GameGraphic::Draw()를 호출하여 게임을 진행하고   
   모든 유저가 충돌하여 게임이 끝났을 때 모든 유저가 “F5"를 누르면   
   GameProcess::SetGame()을 호출하여 게임을 다시 설정합니다.   
   
   
2. 스크린샷  
↓처음 시작했을 때 게임 화면   
![1 (1)](./게임 캡처/1 (1).jpg)   
   
↓위쪽 방향키를 입력했을 때 다음 화면   
![1 (2)](./게임 캡처/1 (2).jpg)   
   
↓왼쪽 방향키를 입력했을 때 다음 화면   
![1 (3)](./게임 캡처/1 (3).jpg)   
   
↓아래쪽 방향키를 입력했을 때 다음 화면   
![1 (4)](./게임 캡처/1 (4).jpg)   
   
↓오른쪽 방향키를 입력했을 때 다음 화면   
![1 (5)](./게임 캡처/1 (5).jpg)   
   
↓위쪽 + 왼쪽 방향키를 입력했을 때 다음 화면   
![1 (6)](./게임 캡처/1 (6).jpg)   
   
↓위쪽 + 오른쪽 방향키를 입력했을 때 다음 화면   
![1 (7)](./게임 캡처/1 (7).jpg)   
   
↓아래쪽 + 왼쪽 방향키를 입력했을 때 다음 화면   
![1 (8)](./게임 캡처/1 (8).jpg)   
   
↓위쪽 + 왼쪽 + 오른쪽을 입력했을 때 다음 화면   
![1 (9)](./게임 캡처/1 (9).jpg)   