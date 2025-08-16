#include "Engine/Graphics/RenderAPI/IIndexBuffer.h"
#include <vector>
#include <d3d11_4.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

namespace Atlas {

class DX11RenderAPI;

class DX11IndexBuffer : public IIndexBuffer
{
public:
	DX11IndexBuffer(const std::vector<uint32_t>&, const ComPtr<ID3D11Device>&);

private:
	struct Impl : IIndexBuffer::Impl
	{
		ComPtr<ID3D11Buffer> m_buffer;
	};

	friend DX11RenderAPI;
};

} //namespace Atlas