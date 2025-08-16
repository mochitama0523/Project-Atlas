#pragma once
#include "Engine/Graphics/RenderAPI/BackEnd.h"
#include <cstdint>
#include <memory>

//DirectXなどのグラフィックAPIを抽象化し、APIを隠蔽するインターフェイス。
//CreateメソッドでAPIを選択し、インターフェイスを取得して使用を開始します。

namespace Atlas {

class IWindowHandle;
class IVertexBuffer;
class IIndexBuffer;
class IViewport;

class IRenderAPI
{
public:
	//使用するAPIを選択し、IRenderAPIのインターフェイスを取得します。
	static std::unique_ptr<IRenderAPI> Create(BackEnd api);

	//IRenderAPIを初期化します。void*版はHWNDを抽象化せずに渡して初期化することができます。
	virtual bool Initialize(IWindowHandle hWnd, uint32_t width, uint32_t height) = 0;
	virtual bool Initialize(void* hWnd, uint32_t width, uint32_t height) = 0;

	//フレームを開始します。指定された色で画面をクリアします。
	virtual void BeginFrame(float r, float g, float b, float a) = 0;

	//描画した内容を画面に表示します。垂直同期フラグを引数で指定します。
	virtual void EndFrame(bool enableVSync) = 0;

	//描画に使う頂点バッファをセットします。
	virtual void BindVertexBuffer(const IVertexBuffer& buffer) = 0;

	//描画に使うインデックスバッファをセットします。
	virtual void BindIndexBuffer(const IIndexBuffer& buffer) = 0;

	//描画に使うビューポートをセットします。
	virtual void BindViewport(const IViewport& viewport) = 0;
};

} //namespace Atlas