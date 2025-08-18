#pragma once
#include "Engine/Common/Vertex.h"
#include <memory>
#include <vector>
#include <string>

//DirectX�Ȃǂ̃O���t�B�b�NAPI�𒊏ۉ����AAPI���B������C���^�[�t�F�C�X�B
//Create���\�b�h��API��I�����A�C���^�[�t�F�C�X���擾���Ďg�p���J�n���܂��B

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
	//�g�p����API��I�����AIRenderAPI�̃C���^�[�t�F�C�X���擾���܂��B
	static std::unique_ptr<IRenderAPI> Create(BackEnd api);

	//IRenderAPI�����������܂��Bvoid*�ł�HWND�𒊏ۉ������ɓn���ď��������邱�Ƃ��ł��܂��B
	virtual bool Initialize(IWindowHandle hWnd, uint32_t width, uint32_t height) = 0;
	virtual bool Initialize(void* hWnd, uint32_t width, uint32_t height) = 0;

	//�t���[�����J�n���܂��B�w�肳�ꂽ�F�ŉ�ʂ��N���A���܂��B
	virtual void BeginFrame(float r, float g, float b, float a) = 0;

	//�`�悵�����e����ʂɕ\�����܂��B���������t���O�������Ŏw�肵�܂��B
	virtual void EndFrame(bool enableVSync) = 0;

	//���_�o�b�t�@���쐬���܂��B�^��Vertex.h�Œ�`����Ă�����̂̒����玩�R�ɑI�ׂ܂��B
	virtual std::unique_ptr<IVertexBuffer> CreateVertexBuffer(const Vertices& vertices) = 0;

	//�C���f�b�N�X�o�b�t�@���쐬���܂��B
	virtual std::unique_ptr<IIndexBuffer> CreateIndexBuffer(const std::vector<uint32_t>& indices) = 0;

	//�`��Ɏg�����_�o�b�t�@���Z�b�g���܂��B
	virtual void BindVertexBuffer(const IVertexBuffer& buffer) = 0;

	//�`��Ɏg���C���f�b�N�X�o�b�t�@���Z�b�g���܂��B
	virtual void BindIndexBuffer(const IIndexBuffer& buffer) = 0;

	//�`��Ɏg���r���[�|�[�g���Z�b�g���܂��B
	virtual void BindViewport(const IViewport& viewport) = 0;

	//�V�F�[�_�[�I�u�W�F�N�g���쐬���܂��B�@
	virtual std::unique_ptr<IShader> CreateShader(ShaderType type, const std::string& filePath) = 0;

	//�`��Ɏg���V�F�[�_�[���Z�b�g���܂��B
	virtual void BindShader(const IShader& shader) = 0;

	//�萔�o�b�t�@���쐬���܂��B
	virtual std::unique_ptr<IConstantBuffer> CreateConstantBuffer(const std::vector<uint8_t>& data) = 0;

	//�`��Ɏg���萔�o�b�t�@���Z�b�g���܂��B
	virtual void BindConstantBuffer(const IConstantBuffer& buffer, uint32_t slot) = 0;

	//�e�N�X�`�����쐬���܂��B
	virtual std::unique_ptr<ITexture> CreateTexture(const std::string& filePath) = 0;

	//�`��Ɏg���e�N�X�`�����Z�b�g���܂��B
	virtual void BindTexture(const ITexture& texture, uint32_t slot) = 0;

	//�Z�b�g���ꂽ�������ƂɃC���f�b�N�X�o�b�t�@���g�p�����ɕ`�悵�܂��B
	virtual void Draw(uint32_t vertexCount, uint32_t startLocation) = 0;

	//�Z�b�g���ꂽ�������ƂɃC���f�b�N�X�o�b�t�@���g�p���ĕ`�悵�܂��B
	virtual void DrawIndexed(uint32_t indexCount, uint32_t startIndexLocation, uint32_t baseVertexLocation) = 0;
};

} //namespace Atlas

/*
* Project Atlas Graphics Library
* Copyright (c) 2025 Haruki Kurokawa
*/

