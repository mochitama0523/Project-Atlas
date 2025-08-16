#pragma once

//IndexBufferを抽象化し、APIに依存しない形にするインターフェイス
//IRenderAPI経由で使用しているAPIの種類に応じた実態を作成します。
//IRenderAPI経由以外で作成できないのは、安全性を保つためです。

namespace Atlas {

class IIndexBuffer
{
protected:
	IIndexBuffer() = default;
	virtual ~IIndexBuffer() = default;

private:
	IIndexBuffer(const IIndexBuffer&) = delete;
	IIndexBuffer& operator=(const IIndexBuffer&) = delete;
	IIndexBuffer(IIndexBuffer&&) = delete;
	IIndexBuffer& operator=(IIndexBuffer&&) = delete;
};

} //namespace Atlas

/*
* Project Atlas Graphics Library
* Copyright (c) 2025 Haruki Kurokawa
*/