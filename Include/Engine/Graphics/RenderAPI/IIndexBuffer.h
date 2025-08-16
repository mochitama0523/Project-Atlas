#pragma once

//IndexBuffer�𒊏ۉ����AAPI�Ɉˑ����Ȃ��`�ɂ���C���^�[�t�F�C�X
//IRenderAPI�o�R�Ŏg�p���Ă���API�̎�ނɉ��������Ԃ��쐬���܂��B
//IRenderAPI�o�R�ȊO�ō쐬�ł��Ȃ��̂́A���S����ۂ��߂ł��B

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