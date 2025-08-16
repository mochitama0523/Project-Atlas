#pragma once
#include <cstdint>

//抽象化されたビューポートです。
//startに開始位置を、width,heightにサイズを指定します。

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