#include "Engine/Graphics/RenderAPI/DX11/DX11VertexBuffer.h"
#include <stdexcept>
#include <string>

using namespace Atlas;

DX11VertexBuffer::DX11VertexBuffer(
	const Vertices& _vertices,
	const ComPtr<ID3D11Device>& _device)
	:m_type(VertexType::Pos)
{
	uint32_t vertexStructSize = 0;
	uint32_t vertNums = 0;
	void* verticesData = nullptr;

	std::visit([&](auto&& val)
		{
			using Type = std::decay_t<decltype(val)>;

			if constexpr (std::is_same_v<Type, VertexPos>)
			{
				vertexStructSize = sizeof(VertexPos);
				verticesData = reinterpret_cast<const void*>(val.data());
				vertNums = val.size();
				m_type = VertexType::Pos;
			}

		}, _vertices);

	D3D11_BUFFER_DESC desc =
	{
		.ByteWidth = vertexStructSize * vertNums,
		.Usage = D3D11_USAGE_DEFAULT,
		.BindFlags = D3D11_BIND_VERTEX_BUFFER,
		.CPUAccessFlags = 0,
		.MiscFlags = 0,
		.StructureByteStride = vertexStructSize
	};

	D3D11_SUBRESOURCE_DATA subData =
	{
		.pSysMem = verticesData,
		.SysMemPitch = 0,
		.SysMemSlicePitch = 0
	};

	if (FAILED(_device->CreateBuffer(
		&desc,
		&subData,
		m_buffer.ReleaseAndGetAddressOf()
	)))
	{
		throw std::runtime_error("VertexBufferÇÃçÏê¨Ç…é∏îsÇµÇ‹ÇµÇΩÅB");
	}
}

ID3D11Buffer* DX11VertexBuffer::GetRawBuffer() const
{
	return m_buffer.Get();
}

VertexType DX11VertexBuffer::GetVertexType() const
{
	return m_type;
}