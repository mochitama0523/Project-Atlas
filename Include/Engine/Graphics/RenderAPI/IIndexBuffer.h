#pragma once
#include <memory>

//IndexBuffer�𒊏ۉ����AAPI�Ɉˑ����Ȃ��`�ɂ���C���^�[�t�F�C�X
//IRenderAPI�o�R�Ŏg�p���Ă���API�̎�ނɉ��������Ԃ��쐬���܂��B

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