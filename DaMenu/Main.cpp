// DaMenu.cpp : Defines the entry point for the console application.
//

#define NOMINMAX
#include <windows.h>
#include "DaMenu.h"

int main()
{
	DXTKRenderer* Renderer=new DXTKRenderer();
	if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
		return 0;

	if (FAILED(Renderer->InitWindow(L"Elemental", GetModuleHandle(NULL), Vector2f(800, 800))))
		return 0;

	ElementManager GUIManager(new WinAPIInputManager(Renderer->GetWindowHWND()),Renderer);

	WindowElement::Context Ctx;
	Ctx.m_FillColor = Color(.7f, .7f, .7f);
	Ctx.m_TitleFillColor = Color(.5f, .5f, .5f);
	Ctx.m_WindowName = "A Window";
	Ctx.m_Position = Vector2f(100, 100);
	Ctx.m_Size = Vector2f(400, 400);
	WindowElement* Window = new WindowElement(Ctx);
	GUIManager.AddElement(Window);

	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}else {
			GUIManager.UpdateInput();
			GUIManager.Render();
		}
	}
    return (int)msg.wParam;
}

