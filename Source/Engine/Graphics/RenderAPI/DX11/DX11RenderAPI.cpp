#include "Engine\Graphics\RenderAPI\DX11\DX11RenderAPI.h"
#include "Engine/Common/IWindowHandle.h"
#include "Engine/Graphics/RenderAPI/DX11/DX11VertexBuffer.h"
#include "Engine/Graphics/RenderAPI/DX11/DX11IndexBuffer.h"
#include "Engine/Graphics/RenderAPI/IViewport.h"
#include <d3d11_4.h>
#include <winrt/base.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Core.h>
#include <cassert>
#include <stdexcept>
#include <string>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

using namespace Atlas;

namespace
{
	constexpr HRESULT E_UNSUPPORTED_PLATFORM =
		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x200);

	inline bool CheckHResult(HRESULT _hr, std::wstring _message)
	{
		if (FAILED(_hr))
		{
			MessageBox(NULL, _message.c_str(), NULL, MB_OK | MB_ICONERROR);
			return false;
		}

		return true;
	}

	std::wstring HResultToString(HRESULT hr)
	{
		wchar_t* msgBuffer = nullptr;
		FormatMessageW(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr,
			hr,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPWSTR)&msgBuffer,
			0,
			nullptr
		);
		std::wstring message = msgBuffer ? msgBuffer : L"Unknown error";
		if (msgBuffer) LocalFree(msgBuffer);
		return message;
	}

	template <typename T>
	T GetOrShowError(const std::expected<T, HRESULT>& result, const std::wstring& context = L"")
	{
		if (!result.has_value())
		{
			std::wstring msg = context.empty() ? L"" : context + L"\n";
			msg += L"HRESULT failed: " + HResultToString(result.error());
			MessageBoxW(nullptr, msg.c_str(), L"Error", MB_OK | MB_ICONERROR);
			// 必要なら例外投げたり、デフォルト値返すこともできる
			throw std::runtime_error("HRESULT error displayed in MessageBox.");
		}
		return result.value();
	}
}

bool DX11RenderAPI::Initialize(void* _hWnd, uint32_t _width, uint32_t _height)
{
	IWindowHandle iHWnd;
	iHWnd.SetWindowHandle(_hWnd);
	iHWnd.SetPlatform(IWindowHandle::Platform::Win32);

	return Initialize(iHWnd, _width, _height);
}

bool DX11RenderAPI::Initialize(IWindowHandle _window, uint32_t _width, uint32_t _height)
{
	//デバイス/コンテキストの作成、インターフェイスのアップグレードをしてメンバ変数へセット
	auto deviceAndContext = GetOrShowError(CreateDevice(), L"デバイスの作成に失敗しました。");
	auto deviceAndContext2 = GetOrShowError(UpgradeDeviceAndContext2(deviceAndContext), L"デバイスのアップグレードに失敗しました。");
	this->SetDeviceAndContext(deviceAndContext2);

	//スワップチェインを作成してメンバ変数へセット
	auto swapChain = GetOrShowError(CreateSwapChain(_width, _height, _window, m_device), L"スワップチェインの作成に失敗しました。");
	this->SetSwapChain(swapChain);

	//レンダーターゲットビューを作成し、メンバ配列へ追加
	auto renderTargetView = GetOrShowError(CreateRenderTargetView(m_device, m_swapChain), L"RTVの作成に失敗しました。");
	this->m_renderTargetViews.push_back(renderTargetView);

	//プリミティブを設定（他のAPIとの整合性を保つため、三角形リスト固定）
	m_deviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return true;
}

DX11RenderAPI::CreateDeviceResult DX11RenderAPI::CreateDevice() const
{
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> context;

	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_12_2,
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0
	};

	HRESULT hr = D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		creationFlags,
		featureLevels,
		ARRAYSIZE(featureLevels),
		D3D11_SDK_VERSION,
		device.ReleaseAndGetAddressOf(),
		nullptr,
		context.ReleaseAndGetAddressOf());

	if (FAILED(hr))
	{
		return std::unexpected(hr);
	}

	return DeviceAndContext({ device,context });
}

DX11RenderAPI::UpgradeDevice2Result
DX11RenderAPI::UpgradeDeviceAndContext2(const DeviceAndContext& _deviceAndContext) const
{
	ComPtr<ID3D11Device2> device2;
	ComPtr<ID3D11DeviceContext2> context2;

	HRESULT hr = S_OK;

	hr = _deviceAndContext.device.As(&device2);

	if (FAILED(hr))
	{
		return std::unexpected(hr);
	}

	hr = _deviceAndContext.context.As(&context2);

	if (FAILED(hr))
	{
		return std::unexpected(hr);
	}

	return DeviceAndContext2({ device2,context2 });
}

void DX11RenderAPI::SetDeviceAndContext(const DeviceAndContext2& _deviceAndContext)
{
	m_device = _deviceAndContext.device;
	m_deviceContext = _deviceAndContext.context;
}

ComPtr<IDXGIFactory2> DX11RenderAPI::GetFactoryFromDevice(const ComPtr<ID3D11Device>& _device) const
{
	ComPtr<IDXGIDevice> dxgiDevice;
	if (FAILED(_device.As(&dxgiDevice)))
	{
		return nullptr;
	}

	ComPtr<IDXGIAdapter> adapter;
	if (FAILED(dxgiDevice->GetAdapter(&adapter)))
	{
		return nullptr;
	}

	ComPtr<IDXGIFactory2> factory;
	if (FAILED(adapter->GetParent(IID_PPV_ARGS(&factory))))
	{
		return nullptr;
	}

	return factory;
}

DX11RenderAPI::SwapChainResult DX11RenderAPI::CreateSwapChain(
	uint32_t _width,
	uint32_t _height,
	const IWindowHandle& _window,
	const ComPtr<ID3D11Device>& _device) const
{
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc =
	{
		.Width = _width,
		.Height = _height,
		.Format = DXGI_FORMAT_B8G8R8A8_UNORM,
		.Stereo = false,
		.SampleDesc = {1,0},
		.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
		.BufferCount = 2,
		.Scaling = DXGI_SCALING_NONE,
		.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL,
		.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED,
		.Flags = NULL
	};

	//プラットフォームに適したスワップチェインを作成
	auto platform = _window.GetPlatform();

	switch (platform)
	{
	case IWindowHandle::Platform::Win32:
		return CreateSwapChainForHWND(_window, swapChainDesc, _device);
		break;
	case IWindowHandle::Platform::UWP:
		return CreateSwapChainForCoreWindow(_window, swapChainDesc, _device);
		break;
	default:
		return std::unexpected(E_UNSUPPORTED_PLATFORM);
		break;
	}
}

DX11RenderAPI::SwapChainResult DX11RenderAPI::CreateSwapChainForHWND(
	const IWindowHandle& _window,
	const DXGI_SWAP_CHAIN_DESC1& _desc,
	const ComPtr<ID3D11Device>& _device) const
{
	ComPtr<IDXGISwapChain1> swapChain;

	auto hwnd = reinterpret_cast<HWND>(_window.GetNativeWindowHandle());
	if (!hwnd) { return std::unexpected(E_POINTER); }

	//ファクトリは毎回デバイスから取得したほうが安全
	auto factory = GetFactoryFromDevice(_device);

	HRESULT hr = factory->CreateSwapChainForHwnd(
		_device.Get(),
		hwnd,
		&_desc,
		nullptr,
		nullptr,
		swapChain.GetAddressOf()
	);

	if (FAILED(hr))
	{
		return std::unexpected(hr);
	}

	return swapChain;
}

DX11RenderAPI::SwapChainResult DX11RenderAPI::CreateSwapChainForCoreWindow(
	const IWindowHandle& _window,
	const DXGI_SWAP_CHAIN_DESC1& _desc,
	const ComPtr<ID3D11Device>& _device) const
{
	ComPtr<IDXGISwapChain1> swapChain;

	auto coreWindow = reinterpret_cast<winrt::Windows::UI::Core::CoreWindow*>(_window.GetNativeWindowHandle());
	if (!coreWindow) { return std::unexpected(E_POINTER); }
	auto unknown = winrt::get_unknown(*coreWindow);

	auto factory = GetFactoryFromDevice(_device);

	HRESULT hr = factory->CreateSwapChainForCoreWindow(
		_device.Get(),
		unknown,
		&_desc,
		nullptr,
		swapChain.GetAddressOf()
	);

	//HWND版とコードが重複しているが、切り出すと可読性が下がるのでそのままにしている
	if (FAILED(hr))
	{
		return std::unexpected(hr);
	}

	return swapChain;
}

void DX11RenderAPI::SetSwapChain(const ComPtr<IDXGISwapChain1> _swapChain)
{
	m_swapChain = _swapChain;
}

DX11RenderAPI::RenderTargetViewResult DX11RenderAPI::CreateRenderTargetView(
	const ComPtr<ID3D11Device>& _device,
	const ComPtr<IDXGISwapChain1>& _swapChain) const
{
	HRESULT hr = S_OK;
	ComPtr<ID3D11Texture2D> backBuffer;
	ComPtr<ID3D11RenderTargetView> renderTargetView;

	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.ReleaseAndGetAddressOf());

	if (FAILED(hr))
	{
		return std::unexpected(hr);
	}

	hr = _device->CreateRenderTargetView(backBuffer.Get(), nullptr, renderTargetView.ReleaseAndGetAddressOf());

	if (FAILED(hr))
	{
		return std::unexpected(hr);
	}

	return renderTargetView;
}

void DX11RenderAPI::BeginFrame(float _r, float _g, float _b, float _a)
{
	const float clearColor[4] = { _r,_g,_b,_a };

	//ComPtrのvectorを生ポインタの配列に変換しなければAPIに渡せない
	std::vector<ID3D11RenderTargetView*> pointers;
	pointers.reserve(m_renderTargetViews.size());

	for (auto& rtv : m_renderTargetViews)
	{
		m_deviceContext->ClearRenderTargetView(
			rtv.Get(),
			clearColor
		);

		pointers.push_back(rtv.Get());
	}

	m_deviceContext->OMSetRenderTargets(
		pointers.size(),
		pointers.data(),
		nullptr	//後で変更
	);
}

void DX11RenderAPI::EndFrame(bool _enableVSync)
{
	m_swapChain->Present(_enableVSync ? 1 : 0, 0);
}

void DX11RenderAPI::BindVertexBuffer(const IVertexBuffer& _buffer)
{
	//間違ったAPIのバッファを渡せないように、IVertexBufferはIRenderAPI経由でしか
	//作成できないようにしているので、static_castしても問題ない。
	const auto& dx11Buf = static_cast<const DX11VertexBuffer&>(_buffer);
	const auto& dx11Impl = static_cast<const DX11VertexBuffer::Impl&>(*dx11Buf.m_impl);

	ID3D11Buffer* buffer = dx11Impl.m_buffer.Get();
	UINT strides = sizeof(Vertex);
	UINT offset = 0;

	m_deviceContext->IASetVertexBuffers(
		0,
		1,
		&buffer,
		&strides,
		&offset
	);
}

void DX11RenderAPI::BindIndexBuffer(const IIndexBuffer& _buffer)
{
	//間違ったAPIのバッファを渡せないように、IIndexBufferはIRenderAPI経由でしか
	//作成できないようにしているので、static_castしても問題ない。
	const auto& dx11Buf = static_cast<const DX11IndexBuffer&>(_buffer);
	const auto& dx11Impl = static_cast<const DX11IndexBuffer::Impl&>(*dx11Buf.m_impl);

	ID3D11Buffer* buffer = dx11Impl.m_buffer.Get();

	m_deviceContext->IASetIndexBuffer(
		buffer,
		DXGI_FORMAT_R32_UINT,
		0
	);
}

void DX11RenderAPI::BindViewport(const IViewport& _viewport)
{
	D3D11_VIEWPORT viewport =
	{
		.TopLeftX = static_cast<float>(_viewport.startX),
		.TopLeftY = static_cast<float>(_viewport.startY),
		.Width = static_cast<float>(_viewport.width),
		.Height = static_cast<float>(_viewport.height),
		.MinDepth = D3D11_MIN_DEPTH,
		.MaxDepth = D3D11_MAX_DEPTH
	};

	m_deviceContext->RSSetViewports(1, &viewport);
}