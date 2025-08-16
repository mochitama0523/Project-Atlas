#pragma once
#include <memory>

//VertexBuffer�𒊏ۉ����AAPI�Ɉˑ����Ȃ��`�ɂ���C���^�[�t�F�C�X
//IRenderAPI�o�R�Ŏg�p���Ă���API�̎�ނɉ��������Ԃ��쐬���܂��B

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