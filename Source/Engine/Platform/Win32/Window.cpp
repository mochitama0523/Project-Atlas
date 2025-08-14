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
		this);	//�E�B���h�E�v���V�[�W���������o�֐��Ɏ����߂̃g���b�N

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

	//�N���C�A���g�̈���w��T�C�Y�ɏC��
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
	//WndProc�������o�֐��Ƃ��Ď����A�I�[�o�[���C�h�\�ɂ��邽�߂�HWND��Window�N���X�̃I�u�W�F�N�g��R�Â���
	SetWindowLongPtr(_hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	this->m_hWnd = _hWnd;
}

LRESULT CALLBACK Window::StaticWindowProc(
	HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
	//_hWnd�ɕR�Â��Ă���Window�N���X�̃I�u�W�F�N�g��T��
	Window* window = (Window*)(GetWindowLongPtr(_hWnd, GWLP_USERDATA));

	//����̃��b�Z�[�W�Ȃ�܂�HWND�ƃI�u�W�F�N�g���R�Â����Ă��Ȃ�
	if (window == nullptr)
	{
		//�����炭WM_CREATE�̍Œ��Ȃ̂�
		if (_msg == WM_CREATE)
		{
			//CreateWindowEX�̃p�����[�^�[����擾����
			window = (Window*)(((LPCREATESTRUCT)_lParam)->lpCreateParams);
		}

		//����������HWND�ƃI�u�W�F�N�g��R�Â���
		if (window != nullptr)
		{
			window->SetPointer(_hWnd);
		}
	}

	//HWND��Window�N���X�̃I�u�W�F�N�g���R�Â��Ă�Ȃ炻�̃I�u�W�F�N�g��WndProc�����s
	if (window != nullptr)
	{
		return window->LocalWindowProc(_hWnd, _msg, _wParam, _lParam);
	}
	else
	{
		//��O�𓊂��Ă��悢��������Ȃ�
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