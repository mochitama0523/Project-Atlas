#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>

namespace Atlas {

class Window
{
public:
	Window();
	virtual ~Window() = default;

	//�E�B���h�E���쐬���܂��BclassName�͑��̃E�B���h�E�Əd�����Ȃ��悤�ɂ��Ă��������B
	bool Create(int width, int height, std::wstring windowTitle, std::wstring className);

	//�E�B���h�E��\�����܂��B�ȉ��̃I�[�o�[���[�h�łɁA������SH_SHOW��n���Ď��s���܂��B
	void Show();

	//Win32API��ShowWindow�֐��ɓn���p�����[�^�[���w�肵�āA�E�B���h�E�̕\�����s���܂��B
	void Show(int nCmdShow);

	//���b�Z�[�W���������܂��B
	bool ProcessMessage();

	//�E�B���h�E�n���h�����擾���܂��B
	HWND GetWindowHandle() const;

	//�h���N���X�ŃJ�X�^�}�C�Y�\�Ȃ��̃E�B���h�E�ɑ΂���E�B���h�E�v���V�[�W���B
	virtual LRESULT LocalWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	HWND m_hWnd;
	HINSTANCE m_hInstance;

private:
	static LRESULT CALLBACK StaticWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void SetPointer(HWND hWnd);
};

}

/*
* Project Atlas Platform Utility Library
* Copyright (c) 2025 Haruki Kurokawa
*/