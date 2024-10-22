#pragma once

#include <dInput.h>
#include <DirectXMath.h>
#include "Direct3D.h"

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dInput8.lib")

#define SAFE_RELEASE(p) if(p != nullptr){ p->Release(); p = nullptr;}
using namespace DirectX;

namespace Input
{
	void Initialize(HWND hWnd);
	void Update();
	bool IsKey(int keyCode);
	bool IsKeyDown(int keyCode);
	bool IsKeyUp(int keyCode);
	void Release();

	//�}�E�X
	bool IsMouseButton(int buttonCode);//�}�E�X�̃{�^����������Ă��邩���ׂ� //�����FbuttonCode ���ׂ����{�^���̔ԍ�
	bool IsMouseButtonDown(int buttonCode);//�}�E�X�̃{�^�����������������ׂ�i�������ςȂ��͖����j
	bool IsMouseButtonUp(int buttonCode);	//�}�E�X�̃{�^�����������������ׂ�
	XMFLOAT3 GetMousePosition();//�}�E�X�J�[�\���̈ʒu���擾
	XMFLOAT3 GetMouseMove();//���̃t���[���ł̃}�E�X�̈ړ��ʂ��擾
	void SetMousePosition(int x, int y);

};