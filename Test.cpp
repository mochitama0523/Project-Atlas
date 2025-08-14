#include "Engine\Platform\Win32\Window.h"
#include <iostream>

using namespace Atlas;

int main()
{
	Window window;
	window.Create(640, 480, TEXT("Test"), TEXT("Test"));
	window.Show();

	while(window.ProcessMessage())
	{
		Sleep(1000);
	}
}