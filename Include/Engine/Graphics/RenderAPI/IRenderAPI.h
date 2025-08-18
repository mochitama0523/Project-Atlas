#pragma once
#include "Engine/Common/Vertex.h"
#include <memory>
#include <vector>
#include <string>

//DirectXなどのグラフィックAPIを抽象化し、APIを隠蔽するインターフェイス。
//CreateメソッドでAPIを選択し、インターフェイスを取得して使用を開始します。

namespace Atlas {

class IWindowHandle;
class IConstantBuffer;
class IVertexBuffer;
class IIndexBuffer;
class IViewport;
class ITexture;
class IShader;

enum class BackEnd
{
	DirectX11
};

enum class ShaderType
{
	Vertex,
	Pixel
};

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

	//頂点バッファを作成します。型はVertex.hで定義されているものの中から自由に選べます。
	virtual std::unique_ptr<IVertexBuffer> CreateVertexBuffer(const Vertices& vertices) = 0;

	//インデックスバッファを作成します。
	virtual std::unique_ptr<IIndexBuffer> CreateIndexBuffer(const std::vector<uint32_t>& indices) = 0;

	//描画に使う頂点バッファをセットします。
	virtual void BindVertexBuffer(const IVertexBuffer& buffer) = 0;

	//描画に使うインデックスバッファをセットします。
	virtual void BindIndexBuffer(const IIndexBuffer& buffer) = 0;

	//描画に使うビューポートをセットします。
	virtual void BindViewport(const IViewport& viewport) = 0;

	//シェーダーオブジェクトを作成します。　
	virtual std::unique_ptr<IShader> CreateShader(ShaderType type, const std::string& filePath) = 0;

	//描画に使うシェーダーをセットします。
	virtual void BindShader(const IShader& shader) = 0;

	//定数バッファを作成します。
	virtual std::unique_ptr<IConstantBuffer> CreateConstantBuffer(const std::vector<uint8_t>& data) = 0;

	//描画に使う定数バッファをセットします。
	virtual void BindConstantBuffer(const IConstantBuffer& buffer, uint32_t slot) = 0;

	//テクスチャを作成します。
	virtual std::unique_ptr<ITexture> CreateTexture(const std::string& filePath) = 0;

	//描画に使うテクスチャをセットします。
	virtual void BindTexture(const ITexture& texture, uint32_t slot) = 0;

	//セットされた情報をもとにインデックスバッファを使用せずに描画します。
	virtual void Draw(uint32_t vertexCount, uint32_t startLocation) = 0;

	//セットされた情報をもとにインデックスバッファを使用して描画します。
	virtual void DrawIndexed(uint32_t indexCount, uint32_t startIndexLocation, uint32_t baseVertexLocation) = 0;
};

} //namespace Atlas

/*
* Project Atlas Graphics Library
* Copyright (c) 2025 Haruki Kurokawa
*/

