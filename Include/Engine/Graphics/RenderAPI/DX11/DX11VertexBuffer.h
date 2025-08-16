#pragma once
#include "Engine/Graphics/RenderAPI/IVertexBuffer.h"
#include <vector>
#include <d3d11_4.h>
#include <wrl/client.h>

//抽象化されたDirectX11用の頂点バッファ

using Microsoft::WRL::ComPtr;

namespace Atlas
{
	class DX11RenderAPI;

	class DX11VertexBuffer : public IVertexBuffer
	{
	public:
		DX11VertexBuffer(const std::vector<Vertex>&, const ComPtr<ID3D11Device>&);

	private:
		struct Impl : IVertexBuffer::Impl
		{
			ComPtr<ID3D11Buffer> m_buffer;
		};

		friend DX11RenderAPI;
	};
}