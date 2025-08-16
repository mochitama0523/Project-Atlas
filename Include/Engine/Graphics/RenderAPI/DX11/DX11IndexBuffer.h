#include "Engine/Graphics/RenderAPI/IIndexBuffer.h"
#include <vector>
#include <d3d11_4.h>
#include <wrl/client.h>

//抽象化されたDirectX11用のインデックスバッファ
//DX11RenderAPIクラスからのみ作成できる。

using Microsoft::WRL::ComPtr;

namespace Atlas {

class DX11RenderAPI;

class DX11IndexBuffer : public IIndexBuffer
{
private:
	friend DX11RenderAPI;

	DX11IndexBuffer(const std::vector<uint32_t>&, const ComPtr<ID3D11Device>&);
	ID3D11Buffer* GetRawBuffer() const;

	ComPtr<ID3D11Buffer> m_buffer;
};

} //namespace Atlas