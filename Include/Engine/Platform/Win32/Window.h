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

		bool Create(int width, int height, std::wstring windowTitle, std::wstring className);
		void Show();
		void Show(int nCmdShow);
		bool ProcessMessage();
		HWND GetWindowHandle() const;

		//派生クラスでカスタマイズ可能
		virtual LRESULT LocalWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	protected:
		HWND m_hWnd;
		HINSTANCE m_hInstance;

	private:
		static LRESULT CALLBACK StaticWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		void SetPointer(HWND hWnd);
	};
}