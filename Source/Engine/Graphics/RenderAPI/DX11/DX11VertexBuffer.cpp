#include "Engine/Graphics/RenderAPI/DX11/DX11VertexBuffer.h"
#include <stdexcept>
#include <string>

using namespace Atlas;

DX11VertexBuffer::DX11VertexBuffer(
	const std::vector<Vertex>& _vertices, 
	const ComPtr<ID3D11Device>& _device)
{
	auto impl = std::make_unique<Impl>();

	D3D11_BUFFER_DESC desc =
	{
		.ByteWidth = sizeof(Vertex) * _vertices.size(),
		.Usage = D3D11_USAGE_DEFAULT,
		.BindFlags = D3D11_BIND_VERTEX_BUFFER,
		.CPUAccessFlags = 0,
		.MiscFlags = 0,
		.StructureByteStride = sizeof(Vertex)
	};
	
	D3D11_SUBRESOURCE_DATA subData =
	{
		.pSysMem = _vertices.data(),
		.SysMemPitch = 0,
		.SysMemSlicePitch = 0
	};

	if (FAILED(_device->CreateBuffer(
		&desc,
		&subData,
		impl->m_buffer.ReleaseAndGetAddressOf()
	)))
	{
		throw std::runtime_error("VertexBuffer‚Ìì¬‚É¸”s‚µ‚Ü‚µ‚½B");
	}

	m_impl = std::move(impl);
}