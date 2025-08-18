#pragma once
#include "Engine/Graphics/RenderAPI/IRenderAPI.h"
#include <wrl/client.h>
#include <variant>
#include <d3d11_4.h>
#include <expected>
#include <vector>
#include <utility>

using Microsoft::WRL::ComPtr;

namespace Atlas
{
	class DX11RenderAPI : public IRenderAPI
	{
	public:
		using SwapChainResult = std::expected<ComPtr<IDXGISwapChain1>, HRESULT>;
		using RenderTargetViewResult = std::expected<ComPtr<ID3D11RenderTargetView>, HRESULT>;

		struct DeviceAndContext
		{
			ComPtr<ID3D11Device> device;
			ComPtr<ID3D11DeviceContext> context;
		};

		struct DeviceAndContext2
		{
			ComPtr<ID3D11Device2> device;
			ComPtr<ID3D11DeviceContext2> context;
		};

		using CreateDeviceResult = std::expected<DeviceAndContext, HRESULT>;
		using UpgradeDevice2Result = std::expected<DeviceAndContext2, HRESULT>;

		bool Initialize(IWindowHandle hWnd, uint32_t width, uint32_t height) override;
		bool Initialize(void* hWnd, uint32_t width, uint32_t height) override;
		void BeginFrame(float r, float g, float b, float a) override;
		void EndFrame(bool enableVSync) override;
		
		void BindVertexBuffer(const IVertexBuffer& buffer) override;
		void BindIndexBuffer(const IIndexBuffer& buffer) override;
		void BindViewport(const IViewport& viewport) override;

		std::unique_ptr<IVertexBuffer> CreateVertexBuffer(const Vertices& vertices) override;
		std::unique_ptr<IIndexBuffer> CreateIndexBuffer(const std::vector<uint32_t>& indices) override;

		std::unique_ptr<IShader> CreateShader(ShaderType type, const std::string& filePath) override;
		void BindShader(const IShader& shader) override;

		std::unique_ptr<IConstantBuffer> CreateConstantBuffer(const std::vector<uint8_t>& data) override;
		void BindConstantBuffer(const IConstantBuffer& buffer, uint32_t slot) override;

		std::unique_ptr<ITexture> CreateTexture(const std::string& filePath) override;

		void BindTexture(const ITexture& texture, uint32_t slot) override;

	private:
		//D3D11�f�o�C�X�A�f�o�C�X�R���e�L�X�g�̍쐬
		CreateDeviceResult CreateDevice() const;

		//D3D11�f�o�C�X�A�f�o�C�X�R���e�L�X�g�̃C���^�[�t�F�C�X����ʂ�
		UpgradeDevice2Result UpgradeDeviceAndContext2(const DeviceAndContext&) const;

		//�����o�ϐ��ւ̃f�o�C�X�̃Z�b�g
		void SetDeviceAndContext(const DeviceAndContext2&);

		//�f�o�C�X����t�@�N�g�����擾
		ComPtr<IDXGIFactory2> GetFactoryFromDevice(const ComPtr<ID3D11Device>& device) const;

		//�X���b�v�`�F�C���̍쐬
		SwapChainResult CreateSwapChain(
			uint32_t width,
			uint32_t height,
			const IWindowHandle& window,
			const ComPtr<ID3D11Device>& device) const;

		//Win32�ŃX���b�v�`�F�C���쐬�iCreateSwapChain����Ăяo���j
		SwapChainResult CreateSwapChainForHWND(
			const IWindowHandle& window,
			const DXGI_SWAP_CHAIN_DESC1& desc,
			const ComPtr<ID3D11Device>& device) const;

		//UWP�ŃX���b�v�`�F�C���̍쐬�iCreateSwapChain����Ăяo���j
		SwapChainResult CreateSwapChainForCoreWindow(
			const IWindowHandle& window,
			const DXGI_SWAP_CHAIN_DESC1& desc,
			const ComPtr<ID3D11Device>& device) const;

		//�X���b�v�`�F�C���������o�ϐ��ɃZ�b�g
		void SetSwapChain(const ComPtr<IDXGISwapChain1> swapChain);

		//�����_�[�^�[�Q�b�g�r���[�̍쐬
		RenderTargetViewResult CreateRenderTargetView(
			const ComPtr<ID3D11Device>& device,
			const ComPtr<IDXGISwapChain1>& swapChain) const;

		ComPtr<ID3D11Device2> m_device;
		ComPtr<ID3D11DeviceContext2> m_deviceContext;
		ComPtr<IDXGISwapChain1> m_swapChain;
		std::vector<ComPtr<ID3D11RenderTargetView>> m_renderTargetViews;
	};
}