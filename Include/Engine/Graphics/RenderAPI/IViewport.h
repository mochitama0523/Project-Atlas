#pragma once
#include <cstdint>

//���ۉ����ꂽ�r���[�|�[�g�ł��B
//start�ɊJ�n�ʒu���Awidth,height�ɃT�C�Y���w�肵�܂��B

namespace Atlas {

struct IViewport
{
	uint32_t startX, startY;
	uint32_t width, height;
};

} //namespace Atlas

/*
* Project Atlas Graphics Library
* Copyright (c) 2025 Haruki Kurokawa
*/