#pragma once
#include <memory>

//VertexBufferを抽象化し、APIに依存しない形にするインターフェイス
//IRenderAPI経由で使用しているAPIの種類に応じた実態を作成します。

namespace Atlas {

class Vertex;

class IVertexBuffer
{
protected:
	IVertexBuffer() = default;
	struct Impl {};
	std::unique_ptr<Impl> m_impl;

private:
	IVertexBuffer(const IVertexBuffer&) = delete;
	IVertexBuffer& operator=(const IVertexBuffer&) = delete;
	IVertexBuffer(IVertexBuffer&&) = delete;
	IVertexBuffer& operator=(IVertexBuffer&&) = delete;
};

} //namespace Atlas