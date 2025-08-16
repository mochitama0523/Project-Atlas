#pragma once
#include <memory>

//IndexBufferを抽象化し、APIに依存しない形にするインターフェイス
//IRenderAPI経由で使用しているAPIの種類に応じた実態を作成します。

namespace Atlas {

class IIndexBuffer
{
protected:
	IIndexBuffer() = default;
	struct Impl {};
	std::unique_ptr<Impl> m_impl;

private:
	IIndexBuffer(const IIndexBuffer&) = delete;
	IIndexBuffer& operator=(const IIndexBuffer&) = delete;
	IIndexBuffer(IIndexBuffer&&) = delete;
	IIndexBuffer& operator=(IIndexBuffer&&) = delete;
};

} //namespace Atlas