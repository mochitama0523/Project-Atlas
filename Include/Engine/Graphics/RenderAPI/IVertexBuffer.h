#pragma once

//VertexBufferを抽象化し、APIに依存しない形にするインターフェイス
//IRenderAPI経由で使用しているAPIの種類に応じた実態を作成します。
//IRenderAPI経由以外で作成できないのは、安全性を保つためです。

namespace Atlas {

class IVertexBuffer
{
protected:
	IVertexBuffer() = default;
	virtual ~IVertexBuffer() = default;

private:
	IVertexBuffer(const IVertexBuffer&) = delete;
	IVertexBuffer& operator=(const IVertexBuffer&) = delete;
	IVertexBuffer(IVertexBuffer&&) = delete;
	IVertexBuffer& operator=(IVertexBuffer&&) = delete;
};

} //namespace Atlas

/*
* Project Atlas Graphics Library
* Copyright (c) 2025 Haruki Kurokawa
*/

