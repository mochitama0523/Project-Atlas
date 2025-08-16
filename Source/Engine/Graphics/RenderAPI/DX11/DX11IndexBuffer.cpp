#include "Engine/Graphics/RenderAPI/DX11/DX11IndexBuffer.h"
#include <stdexcept>

using namespace Atlas;

DX11IndexBuffer::DX11IndexBuffer(
	const std::vector<uint32_t>& _indices,
	const ComPtr<ID3D11Device>& _device)
{
	auto impl = std::make_unique<Impl>();

	D3D11_BUFFER_DESC desc =
	{
		.ByteWidth = sizeof(uint32_t) * _indices.size(),
		.Usage = D3D11_USAGE_DEFAULT,
		.BindFlags = D3D11_BIND_INDEX_BUFFER,
		.CPUAccessFlags = 0,
		.MiscFlags = 0,
		.StructureByteStride = sizeof(uint32_t)
	};

	D3D11_SUBRESOURCE_DATA subData =
	{
		.pSysMem = _indices.data(),
		.SysMemPitch = 0,
		.SysMemSlicePitch = 0
	};

	if (FAILED(_device->CreateBuffer(
		&desc,
		&subData,
		impl->m_buffer.ReleaseAndGetAddressOf()
	)))
	{
		throw std::runtime_error("IndexBufferÇÃçÏê¨Ç…é∏îsÇµÇ‹ÇµÇΩÅB");
	}

	m_impl = std::move(impl);
}