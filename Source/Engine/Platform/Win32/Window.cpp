#include "Engine/Platform/Win32/Window.h"

using namespace Atlas;

Window::Window()
	:m_hInstance(GetModuleHandle(NULL)),
	m_hWnd(nullptr)
{

}

bool Window::Create(
	int _width, int _height, std::wstring _windowTitle, std::wstring _className)
{
	WNDCLASSEX windowClass =
	{
		sizeof(WNDCLASSEX),
		CS_HREDRAW | CS_VREDRAW,
		StaticWindowProc,
		0,
		0,
		m_hInstance,
		LoadIcon(NULL,IDI_APPLICATION),
		LoadCursor(NULL,IDC_ARROW),
		(HBRUSH)GetStockObject(BLACK_BRUSH),
		NULL,
		_className.c_str(),
		NULL
	};

	if (RegisterClassEx(&windowClass) == 0)
	{
		return false;
	}

	m_hWnd = CreateWindowEx(
		NULL,
		_className.c_str(),
		_windowTitle.c_str(),
		(WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX ^ WS_MINIMIZEBOX),
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		_width,
		_height,
		NULL,
		NULL,
		m_hInstance,
		this);	//ウィンドウプロシージャをメンバ関数に持つためのトリック

	if (m_hWnd == nullptr)
	{
		return false;
	}

	RECT windowRect, clientRect;

	if (GetWindowRect(m_hWnd, &windowRect) == false)
	{
		return false;
	}

	if (GetClientRect(m_hWnd, &clientRect) == false)
	{
		return false;
	}

	//クライアント領域を指定サイズに修正
	int frameSizeX = (windowRect.right - windowRect.left) - (clientRect.right - clientRect.left);
	int frameSizeY = (windowRect.bottom - windowRect.top) - (clientRect.bottom - clientRect.top);

	int resizeWidth = frameSizeX + _width;
	int resizeHeight = frameSizeY + _height;

	SetWindowPos(
		m_hWnd,
		NULL,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		resizeWidth,
		resizeHeight,
		SWP_NOMOVE
	);

	return true;
}

void Window::Show()
{
	this->Show(SW_SHOW);
}

void Window::Show(int _nCmdShow)
{
	ShowWindow(m_hWnd, _nCmdShow);
}

bool Window::ProcessMessage()
{
	MSG msg = {};

	while (PeekMessage(&msg, nullptr, NULL, NULL, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return false;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true;
}

HWND Window::GetWindowHandle() const
{
	return m_hWnd;
}

void Window::SetPointer(HWND _hWnd)
{
	//WndProcをメンバ関数として持ち、オーバーライド可能にするためにHWNDとWindowクラスのオブジェクトを紐づける
	SetWindowLongPtr(_hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	this->m_hWnd = _hWnd;
}

LRESULT CALLBACK Window::StaticWindowProc(
	HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
	//_hWndに紐づいているWindowクラスのオブジェクトを探す
	Window* window = (Window*)(GetWindowLongPtr(_hWnd, GWLP_USERDATA));

	//初回のメッセージならまだHWNDとオブジェクトが紐づけられていない
	if (window == nullptr)
	{
		//おそらくWM_CREATEの最中なので
		if (_msg == WM_CREATE)
		{
			//CreateWindowEXのパラメーターから取得する
			window = (Window*)(((LPCREATESTRUCT)_lParam)->lpCreateParams);
		}

		//見つかったらHWNDとオブジェクトを紐づける
		if (window != nullptr)
		{
			window->SetPointer(_hWnd);
		}
	}

	//HWNDとWindowクラスのオブジェクトが紐づいてるならそのオブジェクトのWndProcを実行
	if (window != nullptr)
	{
		return window->LocalWindowProc(_hWnd, _msg, _wParam, _lParam);
	}
	else
	{
		//例外を投げてもよいかもしれない
		return DefWindowProc(_hWnd, _msg, _wParam, _lParam);
	}
}

LRESULT Window::LocalWindowProc(
	HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
	switch (_msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		DestroyWindow(_hWnd);
		break;
	case WM_QUIT:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(_hWnd, _msg, _wParam, _lParam);
		break;
	}

	return 0;
}