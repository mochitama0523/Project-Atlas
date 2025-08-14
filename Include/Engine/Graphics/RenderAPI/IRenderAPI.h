#pragma once
#include <cstdint>

namespace Atlas
{
	class IWindowHandle;

	class IRenderAPI
	{
	public:
		virtual bool Initialize(IWindowHandle hWnd, uint32_t width, uint32_t height) = 0;
		virtual void BeginFrame(float r, float g, float b, float a) = 0;
		virtual void EndFrame(bool enableVSync) = 0;
	};
}