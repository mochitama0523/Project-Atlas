#pragma once
#include "Engine/Graphics/RenderAPI/IVertexBuffer.h"
#include "Engine/Common/Vertex.h"
#include <memory>
#include <d3d11_4.h>
#include <wrl/client.h>

//���ۉ����ꂽDirectX11�p�̒��_�o�b�t�@
//DX11RenderAPI�N���X����̂ݍ쐬�ł���B

using Microsoft::WRL::ComPtr;

namespace Atlas {

class DX11RenderAPI;

class DX11VertexBuffer : public IVertexBuffer
{
private:
	friend DX11RenderAPI;

	DX11VertexBuffer(const Vertices&, const ComPtr<ID3D11Device>&);
	ID3D11Buffer* GetRawBuffer() const;
	VertexType GetVertexType() const;

	ComPtr<ID3D11Buffer> m_buffer;
	VertexType m_type;
};

} //namespace Atlas