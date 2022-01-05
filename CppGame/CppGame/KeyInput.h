#pragma once

class KeyInput
{
public:
	void SetKey(struct Room &rRoom, int myNumber);	//  rRoom에서 myNumber와 유저 번호가 같은 rRoom.user 구조체에 키입력 정보를 저장합니다. 
};

// 다른 플레이어 자동 키 입력
void otherSetKey(struct User &rUser);