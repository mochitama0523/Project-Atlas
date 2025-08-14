#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>

namespace Atlas
{
	class Window
	{
	public:
		Window();
		virtual ~Window() = default;

		bool Create(int _width, int _height, std::wstring _windowTitle, std::wstring _className);
		void Show();
		void Show(int _nCmdShow);
		bool ProcessMessage();
		HWND GetWindowHandle() const;

		//派生クラスでカスタマイズ可能
		virtual LRESULT LocalWindowProc(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);

	protected:
		HWND m_hWnd;
		HINSTANCE m_hInstance;

	private:
		static LRESULT CALLBACK StaticWindowProc(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);
		void SetPointer(HWND _hWnd);
	};
}