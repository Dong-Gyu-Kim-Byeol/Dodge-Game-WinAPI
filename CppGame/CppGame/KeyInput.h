#pragma once

class KeyInput
{
public:
	void SetKey(struct Room &rRoom, int myNumber);	//  rRoom���� myNumber�� ���� ��ȣ�� ���� rRoom.user ����ü�� Ű�Է� ������ �����մϴ�. 
};

// �ٸ� �÷��̾� �ڵ� Ű �Է�
void otherSetKey(struct User &rUser);