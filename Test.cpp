#include "Engine/Platform/Win32/Window.h"
#include "Engine/Graphics/RenderAPI/IRenderAPI.h"

using namespace Atlas;

int main()
{
	Window window;
	window.Create(640, 480, TEXT("Test"), TEXT("Test"));
	window.Show();

	auto renderer = IRenderAPI::Create(BackEnd::DirectX11);
	renderer->Initialize(window.GetWindowHandle(), 640, 480);

	while (window.ProcessMessage())
	{
		renderer->BeginFrame(0.f, 1.f, 0.f, 1.f);
		renderer->EndFrame(true);
	}
}