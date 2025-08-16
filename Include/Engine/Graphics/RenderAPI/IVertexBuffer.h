#pragma once

//VertexBuffer�𒊏ۉ����AAPI�Ɉˑ����Ȃ��`�ɂ���C���^�[�t�F�C�X
//IRenderAPI�o�R�Ŏg�p���Ă���API�̎�ނɉ��������Ԃ��쐬���܂��B
//IRenderAPI�o�R�ȊO�ō쐬�ł��Ȃ��̂́A���S����ۂ��߂ł��B

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

