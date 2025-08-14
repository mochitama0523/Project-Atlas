#pragma once

namespace Atlas
{
	class IWindowHandle
	{
	public:
		enum class Platform
		{
			Win32,
			UWP
		};

		void* GetNativeWindowHandle() const { return m_nativeWindowHandle; }
		Platform GetPlatform() const { return m_platform; }

	private:
		void* m_nativeWindowHandle;
		Platform m_platform;
	};
}