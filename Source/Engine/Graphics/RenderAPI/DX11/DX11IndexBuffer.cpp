#include "Engine/Graphics/RenderAPI/DX11/DX11IndexBuffer.h"
#include <stdexcept>

using namespace Atlas;

DX11IndexBuffer::DX11IndexBuffer(
	const std::vector<uint32_t>& _indices,
	const ComPtr<ID3D11Device>& _device)
{
	D3D11_BUFFER_DESC desc =
	{
		.ByteWidth = sizeof(uint32_t) * static_cast<uint32_t>(_indices.size()),
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
		m_buffer.ReleaseAndGetAddressOf()
	)))
	{
		throw std::runtime_error("IndexBuffer�̍쐬�Ɏ��s���܂����B");
	}
}

ID3D11Buffer* DX11IndexBuffer::GetRawBuffer() const
{
	return m_buffer.Get();
}