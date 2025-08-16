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

	//ウィンドウを作成します。classNameは他のウィンドウと重複しないようにしてください。
	bool Create(int width, int height, std::wstring windowTitle, std::wstring className);

	//ウィンドウを表示します。以下のオーバーロード版に、自動でSH_SHOWを渡して実行します。
	void Show();

	//Win32APIのShowWindow関数に渡すパラメーターを指定して、ウィンドウの表示を行います。
	void Show(int nCmdShow);

	//メッセージを処理します。
	bool ProcessMessage();

	//ウィンドウハンドルを取得します。
	HWND GetWindowHandle() const;

	//派生クラスでカスタマイズ可能なこのウィンドウに対するウィンドウプロシージャ。
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